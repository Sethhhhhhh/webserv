#include "Config.hpp"

Config::Config(void) {
	return ;
}

Config::Config(const Config & src) {
	return ;
}

Config &	Config::operator(const Config & src) {
	return (*this);
}

Config::~Config(void) {
	return ;
}

std::vector<std::string>	Config::getServerName(void) const {
	return (_serverName);
}

std::vector<std::string>	Config::getIndex(void) const {
	return (_index);
}

std::vector<int>	Config::getListen(void) const {
	return (_listen);
}

std::string	Config::getRoot(void) const {
	return (_root);
}

std::string Config::getCgiPass(void) const {
	return (_cgiPass);
}

std::string Config::getAlias(void) const {
	return (_alias);
}

int	Config::getClientBodyBufferSize(void) const {
	return (_clientBodyBufferSize);
}

bool	Config::getAliasSet(void) const {
	return (_aliasSet);
}

bool	Config::getAutoIndex(void) const {
	return (_autoIndex);
}

int	Config::parseFile(std::ifstream & file) {
	return (0);
}
