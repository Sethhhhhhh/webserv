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

int Sockets::add_client(Client	&client)
{
	ready_fd--;
	if (servers.size() + clients.size() < FD_SETSIZE)
	{
		FD_SET(client._fd, &active_set);
		fd_list.push_back(client._fd);
		max_fd = *std::max_element(fd_list.begin(), fd_list.end());
		clients.push_back(&client);
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
	char buf[100001];
	int ret;

	for (std::vector<Client*>::iterator i = clients.begin(); i != clients.end();
		i++)
	{
		if (FD_ISSET((*i)->_fd, &ready_set))
		{
			std::stringstream ss;
			std::cout << "fd: " << (*i)->_fd << std::endl;
			ret = recv((*i)->_fd, buf, 100000, 0);
			buf[ret] = 0;
			ss << buf;
			(*i)->_buffer = ss.str();
			(*i)->parse();

			std::ifstream ifs;
			std::string  resp;
			std::string	 file;
			std::stringstream len;

			file = "pages/test.html";
			resp = "HTTP/1.1 200 OK\n";
			resp += "Content-Length: ";
			ifs.open(file.c_str());
			ss.str("");
			ss << ifs.rdbuf();
			std::cout << ss.str() << std::endl;
			len << ss.str().length();
			resp += len.str();
			resp += "\n";
			resp += "Content-Type: ";
			resp += MIME_types(file);
			resp += "\n\n";
			resp += ss.str();
			std::cout << resp << std::endl;
			send((*i)->_fd, resp.c_str(), resp.size(), 0);
			close((*i)->_fd);
		}
	}
	return 0;
}

void	Sockets::loop(void)
{
	while (1)
	{
		ready_set = active_set;
		ready_fd = select(max_fd + 1, &ready_set, 0, 0, 0);
		for (std::vector<Server*>::iterator i = servers.begin();
			i != servers.end(); i++)
		{
			if (FD_ISSET((*i)->fd, &ready_set))
			{
				Client *new_client;
				new_client = new Client;
				(*new_client)._fd = accept((*i)->fd, 0, 0);
				if ((*new_client)._fd != -1)
				{
					std::cout << "Connection accepted." << std::endl;
					(*new_client)._server = *i;
					add_client((*new_client));
				}
			}
		}
		check_clients();
	}
}
