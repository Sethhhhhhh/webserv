#ifndef CLIENT_H
# define CLIENT_H

# include <iostream>
# include <string>
# include "Server.hpp"

class Client
{
private:
	
public:
	Client();
	Client(const Client &c);
	Client& operator=(const Client &c);
	~Client();

	int fd;
	std::string buffer;
	Server *server;
};

#endif