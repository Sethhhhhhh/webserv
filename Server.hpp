#ifndef SERVER_H
# define SERVER_H

# include "Connections.hpp"
# include "Config.hpp"

class Server
{
private:
	std::string                     _host;
	unsigned int                    _port;
	std::vector<std::string>        _names;
	std::vector<t_location>			_locations;
	std::string						_root;
	std::map<int, std::string>		_error_pages;
	unsigned int                    _client_max_body_size;

public:
	Server();
	Server(	std::string host, 
			unsigned short port,
			std::vector<std::string> names,
			std::vector<t_location> locations,
			std::string root,
			std::map<int, std::string> error_pages,
			unsigned int client_max_body_size);
	Server(const Server &c);
	Server& operator=(const Server &c);
	~Server();

	void respond(int fd, Request &request);
	
	int fd;
	int host;
	int port;

	Config	config;	
};

#endif
