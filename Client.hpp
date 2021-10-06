#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include "webserv.hpp"
# include "Server.hpp"
# include "Sockets.hpp"

class Client
{
	private:
		int 								_fd;
		std::string							_buffer;
		Server* 							_server;
		int									_status;
		std::string							_method;
		std::string							_version;
		std::string 						_uri;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		int									_ret_code;
		t_chunk								_chunk;

	public:

		Client();
		Client(const Client &c);
		Client& operator=(const Client &c);
		~Client();

		void		parse(void);
		void	print_request(void);
		void	check_parsing(void);
		void	parse_body(void);
		void	parse_chunked_body(void);

		enum         status
		{
			UNCHUNKED,
			BEGIN,
			FOUND,
			FINISHED
		};

	friend class Sockets;
};

#endif
