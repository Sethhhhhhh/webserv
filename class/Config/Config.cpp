#include "Config.hpp"

/* Constructor & Destructor */
Config::Config(void) {
	return ;
}

Config::Config(Config & src) {
	*this = src;
	return ;
}

Config &	Config::operator=(Config & src) {
	(void)src;
	return (*this);
}

Config::~Config(void) {
	return ;
}	


char	Config::check_ip(void) const {
	struct sockaddr_in sa;
	return (!inet_pton(AF_INET, server->get_host().c_str(), &(sa.sin_addr)));
}

/* Set */
char	Config::set_listen(std::string &content, int line_count) {
	size_t			pos;
	unsigned int	expected_port;
	
	content.erase(0, 6);
	if (content.empty())
		throw Error("no content.", line_count);
	if (server->get_port() && !server->get_host().empty())
		throw Error("duplicate host.", line_count);
	remove_extra_space(content, 0);

	pos = content.find(":");
	if (pos != std::string::npos) {
		expected_port = std::atoi(content.substr(pos + 1, content.length()).c_str());
		if (expected_port <= 0 || expected_port > 65535)
			throw Error("The port used is not valid.", line_count);
		server->set_port(expected_port);
		server->set_host(content.substr(0, pos));
	}
	else
		server->set_host(content);

	if (server->get_host() == std::string("*"))
		server->set_host(std::string("0.0.0.0"));
	else if (server->get_host() == std::string("localhost"))
		server->set_host(std::string("127.0.0.1"));
	
	if (check_ip())
		throw Error("The ip address is invalid.", line_count);
	content.erase();
	return (0);
}

char	Config::set_error_pages(std::string &content, int line_count) {
	std::string	path;
	std::string	error_expected;
	size_t		error;
	size_t		pos;

	error = 0;
	content.erase(0, 10);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);

	pos = content.find_first_of(" ");
	if (pos == std::string::npos)
		return (1);

	error_expected = content.substr(0, pos);
	if (error_expected.find_first_not_of("0123456789") != std::string::npos)
		throw Error("The error is invalid.", line_count);
	error = std::atoi(error_expected.c_str());

	content.erase(0, pos);
	remove_extra_space(content, 0);
	if (content.empty())
		throw Error("No error related path is used.", line_count);

	if (content[0] != '/' || content.substr(content.length() - 6, 5) != std::string(".html"))
		throw Error("The path must be absolute and must end in .html.", line_count);
	server->set_error_pages(error, content.substr(0, content.length() - 1));

	return (0);
}

char	Config::set_server_names(std::string &content) {
	std::string name;
	size_t 		pos;

	content.erase(0, 11);
	if (!server->get_names().empty())
		return (1);
	remove_extra_space(content, 0);
	if (content.empty())
		return (1);

	pos = content.find_first_of(" ");
	while (pos != std::string::npos) {
		server->set_names(content.substr(0, pos));
		content.erase(0, pos);
		remove_extra_space(content, 0);
		pos = content.find_first_of(" ");
	}
	server->set_names(content.substr(0, content.length() - 1));
	content.erase();
	return (0);
}

char	Config::set_client_max_body_size(std::string &content) {
	content.erase(0, 20);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);

	if (server->get_client_max_body_size())
		return (1);
	if (content.find_first_not_of("0123456789MmGgKk;") != std::string::npos)
		return (1);
	server->set_client_max_body_size(static_cast<size_t>(std::atol(content.c_str())));
	if (content[content.length() - 2] == 'G' || content[content.length() - 2] == 'g')
		server->set_client_max_body_size(server->get_client_max_body_size() * 1000000000);
	else if (content[content.length() - 2] == 'M' || content[content.length() - 2] == 'm')
		server->set_client_max_body_size(server->get_client_max_body_size() * 1000000);
	else if (content[content.length() - 2] == 'K' || content[content.length() - 2] == 'k')
		server->set_client_max_body_size(server->get_client_max_body_size() * 1000);
	content.erase();
	return (0);
}

char	Config::set_root(std::string &content) {
	content.erase(0, 4);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);

	if (!server->get_root().empty())
		return (1);
	
	if (content[0] != '/')
		return (1);
	server->set_root(content.substr(0, content.length() - 1));
	content.erase();
	return (0);
}

/* PARSE LOCATION */

char	Config::set_location(std::ifstream & file, std::string &content, int &line_count) {
	s_location		location;
	std::string		path;
	size_t			pos;

	content.erase(0, 8);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);
	if (content[0] != '/')
		return (1);
	pos = content.find_first_of(" ");
	if (pos == std::string::npos)
		return (1);
	path = content.substr(0, pos);
	for (std::vector<s_location>::iterator it = server->get_locations().begin(); it < server->get_locations().end(); it++) {
		if ((*it).path == path) {
			return (1);		
		}
	}
	location.path = path;
	content.erase(0, pos);
	remove_extra_space(content, 0);
	if (content.empty() || content[0] != '{')
		return (1);

	location.client_max_body_size = 0;
	location.autoindex = false;
	while (std::getline(file, content)) {
		remove_extra_space(content, 0);
		if (content.empty() || content[0] == '#') {
			std::cout << line_count << std::endl;
			line_count++;
			continue;
		}
		line_count++;

		if (content[content.length() - 1] != ';' && content[0] != '}') {
			throw Error("semicolon required at the end of the line.", line_count);
		}
		if (count_char_in_string(content, ';') > 1)
			throw Error("There is more than one semicolon at the end of the line.", line_count);
		if (!content.compare(0, 4, "root")) {
			if (set_location_root(content, location.root)) {
				
				return (1);
			}
		}
		else if (!content.compare(0, 6, "method")) {
			if (set_method(content, location.methods))
				return (1);
		}
		else if (!content.compare(0, 9, "autoindex")) {
			if (set_autoindex(content, location.autoindex))
				return (1);
		}
		else if (!content.compare(0, 5, "index")) {
			if (set_index(content, location.index))
				return (1);
		}
		else if (!content.compare(0, 13, "cgi_extension")) {
			if (set_cgi_extension(content, location.cgi_extension))
				return (1);
		}
		else if (!content.compare(0, 8, "cgi_path")) {
			if (set_cgi_path(content, location.cgi_path))
				return (1);
		}
		else if (!content.compare(0, 20, "client_max_body_size")) {
			if (set_location_client_max_body_size(content, location.client_max_body_size))
				return (1);
		}
		else if (!content.compare(0, 20, "auth_basic_user_file")) {
			if (set_auth_basic_user_file(content, location.auth_basic_user_file))
				return (1);
		}
		else if (!content.compare(0, 10, "auth_basic")) {
			if (set_auth_basic(content, location.auth_basic))
				return (1);
		}
		else if (content[0] == '}') {
			server->set_locations(location);

			return (0);
		}
	}
	return (0);
}

/* PARSE */

char	Config::parse(Server &server, std::ifstream &file, int &line_count) {
	std::string	content;
	char		bracket;

	bracket = 0;
	this->server = &server;
	while (std::getline(file, content)) {
		
		remove_extra_space(content, 0);
		if (content.empty() || content[0] == '#') {
			line_count++;
			continue;
		}

		line_count++;
		if (content[content.length() - 1] != ';' && content[0] != '}' && content.compare(0, 8, "location")) {
			throw Error("semicolon required at the end of the line.", line_count);
		}
		if (count_char_in_string(content, ';') > 1) {
			throw Error("There is more than one semicolon at the end of the line.", line_count);
		}
		if (!content.compare(0, 6, "listen")) {
			set_listen(content, line_count);
		}
		else if (!content.compare(0, 10, "error_page")) {
			set_error_pages(content, line_count);
		}
		else if (!content.compare(0, 11, "server_name")) {
			set_server_names(content);
		}
		else if (!content.compare(0, 20, "client_max_body_size")) {
			set_client_max_body_size(content);
		}
		else if (!content.compare(0, 4, "root")) {
			set_root(content);
		}
		else if (!content.compare(0, 8, "location")) {
			if (set_location(file, content, line_count)) {
				return (1);
			}
		}
		else if (content[0] == '}') {
			bracket = 1;
			break;
		}
		else {
			return (1);
		}
	}
	if (!bracket)
		return (1);
	return (0);
}
