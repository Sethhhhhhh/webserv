#include "../class/Connections/Connections.hpp"
#include "../includes/webserv.hpp"

int	main(int ac, char **av)
{
	Connections connections;
	std::vector<Server*> servers;
	Server test;


	test.set_port(1234);
	servers.push_back(&test);

	if (ac != 2 || parse(servers, av[1])) {
		return (1);
	}
	if (connections.init(servers) == -1)
		return 1;
	
	connections.loop();
	return 0;
}
