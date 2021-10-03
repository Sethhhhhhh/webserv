#include "Sockets.hpp"


Sockets::Sockets() 
{
}

Sockets::Sockets(const Sockets &c)
{
	(void)c;

}


Sockets& Sockets::operator=(const Sockets &c)
{
	(void)c;

	return *this;
}

Sockets::~Sockets()
{
}

int Sockets::init(std::vector<Server*> parsed)
{
	struct sockaddr_in addr;
	int		optval;

	FD_ZERO(&active_set);
	optval = 1;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	for (std::vector<Server*>::iterator i = parsed.begin(); i != parsed.end(); i++)
	{
		(*i)->fd = socket(AF_INET, SOCK_STREAM, 0);
		if ((*i)->fd == -1)
			 continue ;
		if (setsockopt((*i)->fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		{
			close((*i)->fd);
			continue ;
		}
		addr.sin_port = htons((*i)->port);
		if (bind((*i)->fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		{
			close ((*i)->fd);
			continue ;
		}
		if (listen((*i)->fd, SOMAXCONN) < 0)
		{
			close ((*i)->fd);
			continue ;
		}
		servers.insert(servers.end(), *i);
		FD_SET((*i)->fd, &active_set);
		fd_list.insert(fd_list.end(), (*i)->fd);
		max_fd = (*i)->fd;
	}
	if (servers.size() == 0)
		return -1;
	else
	{
		max_fd = *std::max_element(fd_list.begin(), fd_list.end());
		return 0;
	}
}

int Sockets::add_client(Client& client)
{
	ready_fd--;
	if (servers.size() + clients.size() < FD_SETSIZE)
	{
		FD_SET(client.fd, &active_set);
		fd_list.insert(fd_list.end(), client.fd);
		max_fd = *std::max_element(fd_list.begin(), fd_list.end());
		clients.insert(clients.end(), &client);
	}
	else
	{
		std::cerr << "Error. Max connections reached." << std::endl;
		return -1;
	}
	return 0;
}

int Sockets::check_clients()
{
	char buf[1024];
	int ret;

	for (std::vector<Client*>::iterator i = clients.begin(); i != clients.end();
		i++)
	{
		if (FD_ISSET((*i)->fd, &ready_set))
		{
			std::cout << "fd: " << (*i)->fd << std::endl;
			ret = recv((*i)->fd, buf, 1024, 0);
			std::cout << "buf: " << buf << std::endl;
		}
	}
	return 0;
}