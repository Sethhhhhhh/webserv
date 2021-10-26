#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <arpa/inet.h>

typedef	struct	s_location {
	std::vector<std::string>	methods;
	std::vector<std::string>	cgi_extension;
	std::vector<std::string>	index;

	std::string					path;
	std::string					root;
	std::string					cgi_path;
	std::string					upload_path;
	std::string					auth_basic_user_file;
	std::string					auth_basic;

	bool						upload_eanable;
	bool						autoindex;

	size_t						client_max_body_size;
}				t_location;

class Config {

	public:
		Config(void);
		~Config(void);
		Config(Config &);
		
		Config &	operator=(Config &);

		char		parse(std::ifstream & file, size_t & line_count);

	private:
		std::vector<std::pair<int, std::string> >	_error_pages;
		std::vector<std::string>					_names;
		std::vector<t_location>						_locations;
		std::string									_host;
		std::string									_root;
		unsigned int								_port;
		unsigned int								_client_max_body_size;

		char	set_listen(std::string &content);
		char	set_error_pages(std::string &content);
		char	set_server_names(std::string &content);
		char	set_client_max_body_size(std::string &content);
		char	set_root(std::string &content);
		char	set_location(std::ifstream & file, std::string &content, size_t & line_count);

		char	set_location_root(std::string &content, std::string & root);
		char	set_location_client_max_body_size(std::string &content, size_t & client_max_body_size);
		char	set_autoindex(std::string &content, bool & autoindex);
		char	set_cgi_extension(std::string &content, std::vector<std::string> &cgi_extension);
		char	set_method(std::string &content, std::vector<std::string> &method);
		char	set_cgi_path(std::string &content, std::string &cgi_path);
		char	set_auth_basic(std::string &content, std::string &auth_basic);
		char	set_auth_basic_user_file(std::string &content, std::string &auth_basic_user_file);
		char	set_index(std::string &content, std::vector<std::string> &index);

		char	check_ip(void) const;

		void	print(void);
};

#endif
