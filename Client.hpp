#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include "Server.hpp"
# include "Sockets.hpp"

class Client
{
	private:
		int 	_fd;
		char*	_buffer;
		Server* _server;

	public:

		Client();
		Client(const Client &c);
		Client& operator=(const Client &c);
		~Client();

	friend class Sockets;
};

#endif