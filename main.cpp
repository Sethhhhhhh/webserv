#include "Sockets.hpp"

int	main()
{
	Sockets sockets;
	std::vector<Server*> servers;
	Server test;

	test.port = 1234;
	servers.insert(servers.end(), &test);

	// if (parse(servers, "pathtoconfig") == -1)
	// 	return 1;
	if (sockets.init(servers) == -1)
		return 1;
	
	std::cout << "Waiting for connection." << std::endl;
	sockets.loop();
	return 0;
}