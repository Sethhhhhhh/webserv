#include "../class/Connections/Connections.hpp"
#include "../includes/webserv.hpp"

int	main(int ac, char **av)
{
	Connections connections;

	if (ac != 2)
		return (1);
	try {
		parse(connections.servers, av[1]);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}

	if (connections.init() == -1)
		return (1);
	
	connections.loop();
	return 0;
}
