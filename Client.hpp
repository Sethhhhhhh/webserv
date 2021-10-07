#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include "webserv.hpp"
# include "Server.hpp"
# include "Connections.hpp"

class Server;

class Client
{
	private:
		int 				_fd;
		Server* 			_server;
		Request				_request;

	public:

		Client();
		Client(int fd, Server *server);
		Client(const Client &c);
		Client& operator=(const Client &c);
		~Client();

		void	receive_request(void);
		void	wait_response(void);
		int		get_fd(void) const;


};

#endif
