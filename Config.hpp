#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

#include <string>

class Config {

	private:
		std::vector<int>			_listen;
		std::vector<std::string>	_serverName;
		std::vector<std::string>	_index;
		std::string					_root;
		std::string					_cgiPass;
		std::string					_alias;
		int							_clientBodyBufferSize;
		bool						_aliasSet;
		bool						_autoIndex;
		
	public:
		Config(void);
		~Config(void);
		Config(const Config &);
		
		Config &	operator=(const Config &);

		std::vector<std::string>	getServerName(void) const;
		std::vector<std::string>	getIndex(void) const;
		std::vector<int>			getListen(void) const;
		std::string					getRoot(void) const;
		std::string					getCgiPass(void) const;
		std::string					getAlias(void) const;
		int							getClientBodyBufferSize(void) const;
		int							parseFile(std::ifstream & file);
		bool						getAliasSet(void) const;
		bool						getAutoIndex(void) const;
		
};

#endif
