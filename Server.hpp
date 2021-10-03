#ifndef SERVER_H
# define SERVER_H

# include <iostream>
# include <string>

class Server
{
private:
	
public:
	Server();
	Server(const Server &c);
	Server& operator=(const Server &c);
	~Server();

	int fd;
	int host;
	int port;
	
};

#endif