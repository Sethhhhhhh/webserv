#include "Connections.hpp"

int	main(int ac, char **av)
{
	Connections connections;
	std::vector<Server*> servers;
	Server test;


	test.port = 1234;
	servers.push_back(&test);

	// if (parse(servers, "pathtoconfig") == -1)
	// 	return 1;
	(void)ac;
	(void)av;
	//if (ac != 1 || parse(servers, av[1]) != -1)
	//	return (1);
	if (connections.init(servers) == -1)
		return 1;
	
	connections.loop();
	return 0;
}
