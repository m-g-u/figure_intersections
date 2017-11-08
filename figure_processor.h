#pragma once
#include "figure.h"

class FigureProcessor;

class Overlap
{
friend FigureProcessor;
private:
	int xMin, xMax, yMin, yMax;
	int square; // Square helps determine the figure type of the intersection.
	i_set ids;
	iii_map points;

	void addPoint(int row, int col, int till_col);
	const std::string location() const;
public:
	Overlap() : xMin(INT_MAX), xMax(INT_MIN), yMin(INT_MAX), yMax(INT_MIN), square(0) {};
};

class FigureProcessor
{
private:
	std::vector<Figure*> figures;
	iiii_map points;
	std::map<std::string, Overlap> overlaps;
	std::map<std::string, s_set> idsCache;
	ss_map intersections;

	void parse(const JsonNode &json);
	void spotOverlaps();
	void listIntersections();
	s_set FigureProcessor::idCombinations(const ii_map &colIdSet);
public:
	~FigureProcessor();
	void run(const JsonNode &json);
	std::vector<Figure*> getFigures() const { return figures; }
	ss_map getIntersections() const { return intersections; }
};
