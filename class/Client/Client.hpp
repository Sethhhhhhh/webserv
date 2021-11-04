#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include "../../includes/webserv.hpp"
# include "../Server/Server.hpp"
# include "../Connections/Connections.hpp"

class Server;

class Client
{
	private:
		int 				_fd;
		Server* 			_server;
		Request				_request;
		bool				_ready_request;
		std::string			_received_request;
		unsigned int		_bytes_request;

	public:

		Client();
		Client(int fd, Server *server);
		Client(const Client &c);
		Client& operator=(const Client &c);
		~Client();

		void	receive_request(void);
		void	wait_response(void);
		int		get_fd(void) const;
		bool	request_is_ready(void);

};

#endif
