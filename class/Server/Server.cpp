#include "Server.hpp"

Server::Server(void) {
	_port = 0;
	_client_max_body_size = 0;
}


Server::Server(const Server &c) {
	(void)c;
}

Server& Server::operator=(const Server &c) {
	(void)c;
	return *this;
}

Server::~Server(void) {
	return ;
}

void Server::respond(int fd, Request &request) {
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

/* Get */
Server::s_location							&Server::get_location() {return _location;}
std::vector<Server::s_location>				&Server::get_locations() {return _locations;}
std::map<int, std::string>					Server::get_error_pages() const {return _error_pages;}
std::vector<std::string>					Server::get_names() const {return _names;}
std::string									Server::get_host() const {return _host;}
std::string									Server::get_root() const {return _root;}
unsigned int								Server::get_port() const {return _port;}
int											Server::get_fd() const {return _fd;}
unsigned int								Server::get_client_max_body_size() const {return _client_max_body_size;}

/* Set */
char	Server::set_error_pages(int id, std::string path) {
	_error_pages[id] = path;
	return (0);	
}

char	Server::set_names(std::string name) {
	_names.push_back(name);
	return (0);	
}

char	Server::set_locations(Server::s_location &location) {
	_locations.push_back(location);
	return (0);	
}

char	Server::set_host(std::string host) {
	_host = host;
	return (0);	
}

char	Server::set_root(std::string root) {
	_root = root;
	return (0);	
}

char	Server::set_port(unsigned int port) {
	_port = port;
	return (0);
}

char	Server::set_fd(int fd) {
	_fd = fd;
	return (0);
}

char	Server::set_client_max_body_size(unsigned int client_max_body_size) {
	_client_max_body_size = client_max_body_size;
	return (0);	
}

/* Usuful func */
void	Server::print(void) {
	std::cout << "----------------------------------" << std::endl << "- SERVER INFORMATIONS -" << std::endl;


	/* NAMES */
	std::cout << "server_names: ";
	for (std::vector<std::string>::iterator it = _names.begin(); it !=_names.end(); it++) {
		std::cout << *it;
		if (it != _names.end() - 1)
			std::cout << ", ";
		else
			std::cout << std::endl;
	}

	/* ERROR PAGES */
	std::cout << "error_pages: ";
	for (std::map<int, std::string>::iterator it = _error_pages.begin(); it != _error_pages.end(); it++) {
		std::cout << "[" << it->first << "]" << it->second << ", ";
	}
	std::cout << std::endl;

	/* HOST */
	std::cout << "host: " << _host << std::endl;

	/* PORT */	
	std::cout << "port: " << _port << std::endl;

	/* ROOT */
	std::cout << "root: " << (_root.empty() ? "null" : _root) << std::endl;

	/* CLIENT MAX BODY SIZE */
	std::cout << "client_max_body_size: " << _client_max_body_size << std::endl;

	/* LOCATION(S) */
	std::cout << std::endl << "- LOCATIONS -" << std::endl;

	for (std::vector<struct s_location>::iterator it = _locations.begin(); it != _locations.end(); it++) {
		std::cout << std::endl;
		std::cout << "path: " << it->path << std::endl;
		std::cout << "root: " << (it->root.empty() ? "null" : it->root) << std::endl;
		std::cout << "cgi_path: " << (it->cgi_path.empty() ? "null" : it->cgi_path) << std::endl;
		std::cout << "upload_path: " << (it->upload_path.empty() ? "null" : it->upload_path) << std::endl;
		std::cout << "auth_basic_user_file: " << (it->auth_basic_user_file.empty() ? "null" : it->auth_basic_user_file) << std::endl;
		std::cout << "auth_basic: " << (it->auth_basic.empty() ? "null" : it->auth_basic) << std::endl;
		std::cout << "client_max_body_size: " << it->client_max_body_size << std::endl;
		std::cout << std::boolalpha << "upload_eanable: " << it->upload_eanable << std::endl;
		std::cout << std::boolalpha << "autoindex: " << it->autoindex << std::endl;

		std::cout << "methods: ";
		for (std::vector<std::string>::iterator m = it->methods.begin(); m != it->methods.end(); m++) {
			std::cout << *m;
			if (m != it->methods.end() - 1)
				std::cout << ", ";
		}
		std::cout << std::endl;

		std::cout << "cgi_extension: ";
		for (std::vector<std::string>::iterator m = it->cgi_extension.begin(); m != it->cgi_extension.end(); m++) {
			std::cout << *m;
			if (m != it->cgi_extension.end() - 1)
				std::cout << ", ";
		}
		std::cout << std::endl;

		std::cout << "index: ";
		for (std::vector<std::string>::iterator m = it->index.begin(); m != it->index.end(); m++) {
			std::cout << *m;
			if (m != it->index.end() - 1)
				std::cout << ", ";
		}
		std::cout << std::endl;
	}


	std::cout << std::endl << std::endl;
}