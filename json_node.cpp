#include <ctype.h>
#include "json_node.h"

using namespace std;

void JsonNode::parseJson(const string &json, size_t &pos)
{
	string buffer;
	char scopeChar = 0, quoteChar, c = 0, prev;
	bool quoteOn = false;

	for (pos = pos; pos < json.length(); pos++)
	{
		prev = c;
		c = json[pos];

		if (prev != '\\' && (c == '"' || c == '\'') && (!quoteOn || c == quoteChar))
		{
			quoteOn = !quoteOn;
			quoteChar = quoteOn ? c : 0;
		}
		else if (quoteOn)
		{
			buffer += c;
		}
		else if (isspace(c))
		{
			continue;
		}
		else if (!scopeChar)
		{
			if (c == '{' || c == '[' || c == ':')
				scopeChar = c;
		}
		else if (c == '{' || c == '[' || c == ':')
		{
			JsonNode node(json, pos);
			node.data = buffer;	// buffer: object name
			buffer = "";
			if (c == '[' && scopeChar == ':')
				children = node.children;
			else
				children.push_back(node);
		}
		else if (c == ',' || c == '}' || c == ']')
		{
			if (c == ',' && scopeChar == '[')
				scopeChar = ',';
			if (!buffer.empty() || scopeChar == ',')
			{
				JsonNode node;
				node.data = buffer;	// buffer: object value
				buffer = "";
				children.push_back(node);
			}
			if (scopeChar != ',')
				return;
		}
		else if (scopeChar != '{' || isalnum(c) || c == '_')
		{
			buffer += c;
		}
	}
}
