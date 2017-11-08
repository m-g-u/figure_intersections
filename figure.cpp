#include "figure.h"

using namespace std;

Rectangle::Rectangle(const JsonNode &json)
{
	vector<JsonNode>::const_iterator it;
	for(it = json.children.begin(); it != json.children.end(); ++it)
	{
		string data = it->data;
		int value = atoi(it->children[0].data.c_str());
		if (data == "x")
			x = value;
		else if (data == "y")
			y = value;
		else if (data == "w")
			w = value;
		else if (data == "h")
			h = value;
	}
}

void Rectangle::toPoints(iiii_map &points)
{
	int xMax = x + w;
	int yMax = y + h;

	// Store the points[row][col][figure ID, till col] of the figure's border.
	for (int r = y; r <= yMax; r++)
		points[r][x][id] = xMax;	 
}

