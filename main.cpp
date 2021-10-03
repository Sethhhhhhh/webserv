#include "Sockets.hpp"

int	main()
{
	Sockets sockets;
	std::vector<Server*> servers;
	Server test;
	Client *new_client;

	test.port = 1234;
	servers.insert(servers.end(), &test);

	// if (parse(servers, "pathtoconfig") == -1)
	// 	return 1;
	if (sockets.init(servers) == -1)
		return 1;
	
	std::cout << "Waiting for connection." << std::endl;

	while (1)
	{
		sockets.ready_set = sockets.active_set;
		sockets.ready_fd = select(sockets.max_fd + 1, &sockets.ready_set, 0, 0, 0);
		for (std::vector<Server*>::iterator i = sockets.servers.begin(); 
			i != sockets.servers.end(); i++)
		{
			if (FD_ISSET((*i)->fd, &sockets.ready_set))
			{
				new_client = new Client;
				(*new_client).fd = accept((*i)->fd, 0, 0);
				if ((*new_client).fd != -1)
				{
					std::cout << "Connection accepted." << std::endl;
					(*new_client).server = *i;
					sockets.add_client((*new_client));
				}
			}
		}
		sockets.check_clients();
	}
	return 0;
}