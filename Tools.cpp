#include "Tools.hpp"

std::string	cut_line(std::string &str, bool cond, int back)
{
	std::string ret;

	ret = str.substr(0, str.find('\n') + back);
	if (cond == true)
		str.erase(0, str.find('\n') + 1);
	return (ret);
}
