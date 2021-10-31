#include "Server.hpp"

Server::Server()
{
}

Server::Server(	std::string host, unsigned short port,
			std::vector<std::string> names,
			std::vector<t_location> locations, std::string root,
			std::map<int, std::string> error_pages,
			unsigned int client_max_body_size) :
			_host(host), _port(port), _names(names), _locations(locations),
			_root(root), _error_pages(error_pages), 
			_client_max_body_size(client_max_body_size)
{

}


Server::Server(const Server &c)
{
	(void)c;
}

Server& Server::operator=(const Server &c)
{
	(void)c;
	return *this;
}

Server::~Server()
{
}

void Server::respond(int fd, Request &request)
{
	(void)request;
	std::stringstream ss;
	std::ifstream ifs;
	std::string  resp;
	std::string	 file;
	std::stringstream len;

	file = "pages/test.html";
	resp = "HTTP/1.1 200 OK\n";
	resp += "Content-Length: ";
	ifs.open(file.c_str());
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
	send(fd, resp.c_str(), resp.size(), 0);
}