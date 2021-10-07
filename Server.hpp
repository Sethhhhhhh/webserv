#ifndef SERVER_H
# define SERVER_H

# include "Connections.hpp"

class Server
{
private:
	
public:
	Server();
	Server(const Server &c);
	Server& operator=(const Server &c);
	~Server();

	void respond(int fd, Request &request);
	
	int fd;
	int host;
	int port;
	
};

#endif