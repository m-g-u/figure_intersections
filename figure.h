#pragma once
#include <string>
#include <set>
#include <map>
#include "utils.h"
#include "json_node.h"

typedef unsigned int uint;
typedef std::set<int> i_set;
typedef std::map<int, int> ii_map;
typedef std::map<int, ii_map> iii_map;
typedef std::map<int, iii_map> iiii_map;
typedef std::set<std::string> s_set;
typedef std::map<std::string, std::string> ss_map;

class Figure
{
public:
	uint id;
	virtual ~Figure() {};
	virtual std::string figureName() const = 0;
	virtual std::string location() const = 0;
	std::string description() const { return capitalize(figureName()) + " " + location(); };
	virtual void toPoints(iiii_map &points) = 0;
};

class Rectangle : public Figure
{
private:
	int x;
	int y;
	uint w;
	uint h;
public:
	Rectangle(const JsonNode &json);
	virtual ~Rectangle() {}
	std::string figureName() const { return "rectangle"; };
	static std::string location(int a, int b, int c, int d) { return format("at (%i,%i), w=%i, h=%i", a, b, c, d); };
	std::string location() const { return location(x, y, w, h); };
	void toPoints(iiii_map &points);
};

// Add more figures below.