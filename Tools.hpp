#ifndef TOOLS_HPP
# define TOOLS_HPP

#include "webserv.hpp"

std::string		cut_line(std::string &str, bool cond, int back);
std::string		MIME_types(std::string	&str);
std::string		Last_modified(std::string &path);

#endif
