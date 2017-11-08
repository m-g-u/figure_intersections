#pragma once
#include <sstream>
#include <vector>

extern int TRACE;
void trace(std::string message, int fullTime = 0);
std::string readFile(const char *fileName);
std::string format(const char *fmt, ...);
std::string capitalize(std::string s);
std::string join(const std::vector<std::string> v);

template<class Container>
std::string join(const Container &container)
{
	ostringstream os;
	for(Container::const_iterator it = container.begin(); it != container.end(); ++it)
		os << "," << *it;
	return os.str().substr(1);
}

template<class Container>
std::string joinKeys(const Container &container)
{
	ostringstream os;
	for(Container::const_iterator it = container.begin(); it != container.end(); ++it)
		os << "," << it->first;
	return os.str().substr(1);
}

template <typename L, typename R>
std::string concat(const L left, const R right)
{
  std::ostringstream os;
  os << left << right;
  return os.str();
}
