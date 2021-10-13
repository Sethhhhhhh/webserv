#include "Connections.hpp"

int	main()
{
	Connections connections;
	std::vector<Server*> servers;
	Server test;

	test.port = 1234;
	servers.push_back(&test);

	// if (parse(servers, "pathtoconfig") == -1)
	// 	return 1;
	if (connections.init(servers) == -1)
		return 1;
	
	connections.loop();
	return 0;
}