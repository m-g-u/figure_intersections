#pragma once

public class JsonParser
{
public:
	string name;
	JsonParser[] values;
	size_t Parse(const string &json);
	JsonParser(void);
public:
	~JsonParser(void);
};
