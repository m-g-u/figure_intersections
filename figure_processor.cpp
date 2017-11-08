#include "figure_processor.h"

using namespace std;

void Overlap::addPoint(int row, int col, int till_col)
{
	iii_map &olRows = points;
	ii_map &olCols = olRows[row];
	olCols[col] = till_col;

	yMin = min(yMin, row);
	yMax = max(yMax, row);
	xMin = min(xMin, col);
	xMax = max(xMax, till_col);

	if (olRows.size() < 2)
			return;

	// Calculate square.
	// Add to square only the points that have a point above;
	const ii_map &colsAbove = olRows[row - 1];	// Columns in the row above (the rows have consecutive values).
	ii_map::const_iterator itColAbove;
	itColAbove = colsAbove.lower_bound(col);	// A column above the previous one (same col value or greater).
	if (itColAbove == colsAbove.end())
		return;

	int currCol = max(col, itColAbove->first);
	int tillCol = min(till_col, itColAbove->second);
	square += tillCol - currCol;
}

const std::string Overlap::location() const
{
	if (xMin == xMax && yMin == yMax) {}		// point
	else if (xMin == xMax || yMin == yMax) {}	// line
	else
	{
		const iii_map &rows = points;

		const ii_map &minColSet = rows.find(yMin)->second;
		int topLeft = minColSet.begin()->first == xMin;
		int bottomLeft = minColSet.rbegin()->second == xMax;

		const ii_map &maxColSet = rows.find(yMax)->second;
		int topRight = maxColSet.begin()->first == xMin;
		int bottomRight = maxColSet.rbegin()->second == xMax;

		if (topLeft && bottomLeft && topRight && bottomRight)	// All the right angles are present.
		{
			int width = xMax - xMin;
			int height = yMax - yMin;
			if (square == width * height)	// The border doesn't curve inside, nor the figure has holes.
				return Rectangle::location(xMin, yMin, width, height); // rectangle (or a square)
		}
	}

	return format("at somewhere inside (%i,%i), (%i,%i), (%i,%i), (%i,%i)",
		xMin, yMin, xMin, yMax, xMax, yMin, xMax, yMax);
}

// Parse figures from JSON.
void FigureProcessor::parse(const JsonNode &json)
{
	for (size_t i = 0; i < json.children.size(); i++)
	{
		const JsonNode &node = json.children[i];
		string figureType = node.data;
		if (figureType != "rects")
			continue;

		for(vector<JsonNode>::const_iterator it = node.children.begin(); it != node.children.end(); ++it)
		{
			Figure *figure = NULL;
			if (figureType == "rects")
				figure = new Rectangle(*it);
			if (!figure)
				continue;
			figures.push_back(figure);
			figure->id = (uint)figures.size();
			figure->toPoints(points);
		}
	}
}

// Create or get from cache all the possible combinations of the provided ID-s.
s_set FigureProcessor::idCombinations(const ii_map &colIdSet)
{
	string ids = joinKeys(colIdSet);
	s_set point_ids = idsCache[ids];

	if (point_ids.size())
		return point_ids;

	typedef set<set<int>> i_set2;
	i_set2 point_idSet, tmp_set;
	i_set2::const_iterator i2_it;

	for(ii_map::const_iterator ii_it = colIdSet.begin(); ii_it != colIdSet.end(); ++ii_it)
	{
		for(i2_it = tmp_set.begin(); i2_it != tmp_set.end(); ++i2_it)
		{
			i_set si = *i2_it;
			si.insert(ii_it->first);
			point_idSet.insert(si);
		}
		i_set si;
		si.insert(ii_it->first);
		point_idSet.insert(si);
		tmp_set = point_idSet;
	}

	// Add to overlaps and cache only the combinations with 2+ ID-s.
	for(i2_it = point_idSet.begin(); i2_it != point_idSet.end(); ++i2_it)
	{
		i_set si = *i2_it;
		if (si.size() < 2)
			continue;
		string ids = join(si);
		point_ids.insert(ids);
		overlaps[ids].ids.insert(si.begin(), si.end());
	}

	idsCache[ids] = point_ids;

	return point_ids;
}

void FigureProcessor::spotOverlaps()
{
	ii_map colIdSet;
	ii_map::const_iterator ii_it;

	for(iiii_map::const_iterator iii_it = points.begin(); iii_it != points.end(); ++iii_it)
	{
		const int row = iii_it->first;
		const iii_map &cols = iii_it->second;
		colIdSet.clear();

		for(iii_map::const_iterator iii_it = cols.begin(); iii_it != cols.end(); ++iii_it)
		{
			const int col = iii_it->first;
			const ii_map &idSet = iii_it->second;

			ii_map::iterator it = colIdSet.begin();
			while(it != colIdSet.end())
			{
				if (it->second <= col)
					colIdSet.erase(it++);	// End figure scope.
				else
					++it;
			}

			colIdSet.insert(idSet.begin(), idSet.end());

			if (colIdSet.size() < 2)
				continue;

			// Get all the possible combinations of the figures' ID-s for this column.
			s_set point_ids = idCombinations(colIdSet);

			// Add the points to all the overlaps containing the ID-s,
			// which are bound to this column.
			for(s_set::const_iterator s_it = point_ids.begin(); s_it != point_ids.end(); ++s_it)
			{
				Overlap &ol = overlaps[*s_it];
				int colRelatedToOverlap = 0;
				int till_col = INT_MAX;

				for(i_set::const_iterator i_it = ol.ids.begin(); i_it != ol.ids.end(); ++i_it)
				{
					int id = *i_it;
					if (idSet.count(id))
						colRelatedToOverlap = 1;
					ii_it = colIdSet.find(id);
					if (ii_it != colIdSet.end())
						till_col = min(till_col, ii_it->second);	
				}

				// If the ID-s bound to this column have at least 1 match to the figures' ID-s of the current overlap.
				if (colRelatedToOverlap)
					ol.addPoint(row, col, till_col);	
			}
		}
	}
}

// Make a list of intersections.
void FigureProcessor::listIntersections()
{
	map<string, Overlap>::const_iterator oit;

	for(oit = overlaps.begin(); oit != overlaps.end(); ++oit)
	{
		const string ids = oit->first;
		const Overlap &ol = oit->second;
		const i_set &idSet = ol.ids;
		vector<string> figureNamesIds;

		for(i_set::const_iterator i_it = idSet.begin(); i_it != idSet.end(); ++i_it)
		{
			const int id = *i_it;
			const string figName = figures[id - 1]->figureName();
			figureNamesIds.push_back(format("%s %d", figName.c_str(), id));
		}

		string order = concat(idSet.size(), "," + ids);
		string location = ol.location();
		intersections[order] = format("Between %s %s", join(figureNamesIds).c_str(), location.c_str());
	}
}

FigureProcessor::~FigureProcessor()
{
	for(size_t i = 0; i < figures.size(); i++)
        delete figures[i];
}

void FigureProcessor::run(const JsonNode &json)
{
	// Parse figures from JSON.
	trace("Parse figures from JSON.");
	parse(json);

	// Spot overlaps.
	trace("Spot overlaps.");
	spotOverlaps();

	// Make a list of intersections.
	trace("Make a list of intersections.");
	listIntersections();

	trace(""); 
}
