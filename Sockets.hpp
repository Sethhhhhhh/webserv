#ifndef SOCKETS_H
# define SOCKETS_H

# include <iostream>
# include <string>
# include <algorithm>
# include <vector>
# include "Client.hpp"
# include "Server.hpp"

class Sockets
{
private:
	
public:
	Sockets();
	Sockets(const Sockets &c);
	Sockets& operator=(const Sockets &c);
	~Sockets();

	fd_set	active_set;
	fd_set	ready_set;
	int		ready_fd;
	std::vector<Client&> clients;
	std::vector<Server&> servers;

};

#endif