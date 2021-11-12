#include "../class/Connections/Connections.hpp"
#include "../includes/webserv.hpp"

int	main(int ac, char **av)
{
	Connections connections;

	if (ac != 2 || parse(connections.servers, av[1])) {
		std::cout << "Problem config" << std::endl;
		return (1);
	}

	if (connections.init() == -1)
		return 1;
	
	connections.loop();
	return 0;
}
