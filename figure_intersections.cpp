#pragma once
#include <iostream>
#include "figure_processor.h"

using namespace std; 
int TRACE = 0;	// 1: trace processing time.

int main(int argc, char *argv[])
{
	// Check for the correct arguments' count.
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " <filename>" << endl;
		return -1;
	}

	try
	{
		trace("Read the file and parse JSON from the file content.");
		// Read the file.
		string fcontent = readFile(argv[1]);
		if (fcontent.empty()) {
			printf("Could not open file '%s'.\n", argv[1]);
			return -1;
		}

		// Parse JSON from the file content.
		JsonNode node(fcontent);
		trace("");
		// Process JSON to obtain figures and their intersections. 
		FigureProcessor ff;
		ff.run(node);
		// Output result.
		const vector<Figure*> figures = ff.getFigures();
		cout << "Input:" << endl;
		for(size_t i = 0; i < figures.size(); i++)
			cout << format("\t%d: ", i + 1) << figures[i]->description() + "." << endl;
		cout << endl;

		const ss_map intersections = ff.getIntersections();
		ss_map::const_iterator sit;
		cout << "Intersections:" << endl;
		for(sit = intersections.begin(); sit != intersections.end(); ++sit)
			cout << "\t" << sit->second + "." << endl;

		trace("", 1);
	}
	catch(exception const &e)
	{
		cout << e.what();
		return -1;
	}
	catch(...)   
	{
		cout << "General error\n";
		return -1;
	}

	return 0;
}

