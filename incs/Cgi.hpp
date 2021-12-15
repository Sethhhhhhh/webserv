#ifndef __CGI_HPP__
# define __CGI_HPP__

# include <string>
# include <map>
# include <string>
# include <string.h>
# include "Response.hpp"
# include <sys/types.h>
# include <sys/wait.h>
# include <fstream>

class Response;

class Cgi {

	public:
		Cgi(Response &response);
		~Cgi(void);
		Cgi(const Cgi &);
		Cgi &   operator=(const Cgi &);

		std::string execute(void);

	private:
		char    	**_map_to_table_char(std::map<std::string, std::string> map);
		void    	_init_envs(Response &response);
		std::string	_read_file(std::string path);
		std::string	_get_query_string(std::string uri);

		char								**_envs;
		// char								**_args;
		std::string                         _exe;
		std::string                         _body;
};

#endif