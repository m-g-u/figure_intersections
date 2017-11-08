#pragma once
#include <vector>

struct JsonNode
{
	std::string data;
	std::vector<JsonNode> children;

	JsonNode() {}
	JsonNode(const std::string &json, std::size_t &pos)
	{
		parseJson(json, pos);
	}
	JsonNode(const std::string &json)
	{
		std::size_t pos = 0;
		parseJson(json, pos);
	}
	void parseJson(const std::string &json, std::size_t &pos);
};
