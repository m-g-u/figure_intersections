#include <cstdarg>
#include <ctime>
#include <fstream>
#include <iostream>
#include "utils.h"

using namespace std;

clock_t startTime = clock();
clock_t lastTime;

void trace(string message, int fullTime)
{
	if (!TRACE)
		return;

	if (lastTime)
		cout << '\t' << clock() - lastTime << "ms" << endl;

	if (fullTime)
	{
		cout << "Elapsed time: " << clock() - startTime << "ms" << endl;
		return;
	}

	if (message.empty())
	{
		lastTime = NULL;
		return;
	}

	cout << message;
	lastTime = clock();
}

string readFile(const char *fileName)
{
	ifstream fbuffer(fileName);
	if (!fbuffer.is_open())
		throw runtime_error(string("Could not open file '" + string(fileName) + "'.\n"));

	stringstream ss;
	ss << fbuffer.rdbuf();

	return ss.str();
}

// For std::string parameters, pass their string::c_str().
string format(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
	int sz = vsnprintf(NULL, 0, fmt, args);
	vector<char> buf(sz + 1); // +1 for null terminator.
	vsnprintf(&buf[0], buf.size(), fmt, args);
    va_end(args);
    return string(&buf[0]);
}

string capitalize(std::string s)
{
	s[0] = toupper(s[0]);
	return s;
}

string join(const vector<string> v)
{
	string s = "";
	string sep;
	size_t size = v.size();

	for(size_t i = 0; i < size; i++)
	{
		if (i == 0)
			sep = "";
		else if (size == 2)
			sep = " and ";
		else if (i == size - 1)
			sep = ", and ";
		else
			sep = ", ";

		s += sep + v[i];
	}

	return s;
}
