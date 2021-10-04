#ifndef SOCKETS_HPP
# define SOCKETS_HPP

# include <unistd.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>

# include <cstring>
# include <iostream>
# include <string>
# include <algorithm>
# include <vector>

# include "Client.hpp"
# include "Server.hpp"

class Client;

class Sockets
{
	private:
		fd_set	active_set;
		fd_set	ready_set;
		int		ready_fd;
		int		max_fd;	
	public:
		Sockets();
		Sockets(const Sockets &c);
		Sockets& operator=(const Sockets &c);
		~Sockets();

		int init(std::vector<Server*> servers);
		int add_client(Client& client);
		int check_clients();
		void	loop();

		std::vector<int> fd_list;
		std::vector<Client*> clients;
		std::vector<Server*> servers;

};

#endif