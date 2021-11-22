#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

#include "../Server/Server.hpp"
#include "../../includes/webserv.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <arpa/inet.h>

class Config {

	public:
		/* Constructor & Destructor */
		Config(void);
		~Config(void);
		Config(Config &);
		Config &	operator=(Config &);

		/* Main func */
		char		parse(Server &server, std::ifstream & file, int &line_count);

		class Error : public std::exception {
			private:
				std::string	_msg;

			public:
				Error(std::string error_msg, int line) {
					_msg = "Line " + to_string(line) + ": " + error_msg;
				};
				~Error() throw() {};

 				virtual const char *what() const throw() {
					return (_msg.c_str());
        		};
        };

	private:
		Server	*server;

		/* Set (server config) */
		char	set_listen(std::string &content);
		char	set_error_pages(std::string &content);
		char	set_server_names(std::string &content);
		char	set_client_max_body_size(std::string &content);
		char	set_root(std::string &content);
		char	set_location(std::ifstream & file, std::string &content, int & line_count);

		/* Set (location config) */
		char	set_location_root(std::string &content, std::string & root);
		char	set_location_client_max_body_size(std::string &content, size_t & client_max_body_size);
		char	set_autoindex(std::string &content, bool & autoindex);
		char	set_cgi_extension(std::string &content, std::vector<std::string> &cgi_extension);
		char	set_method(std::string &content, std::vector<std::string> &method);
		char	set_cgi_path(std::string &content, std::string &cgi_path);
		char	set_auth_basic(std::string &content, std::string &auth_basic);
		char	set_auth_basic_user_file(std::string &content, std::string &auth_basic_user_file);
		char	set_index(std::string &content, std::vector<std::string> &index);

		/* Usuful func / Delete this! */
		char	check_ip(void) const;
};

#endif
