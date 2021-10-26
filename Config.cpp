#include "Config.hpp"

/* REMOVE THIS ! */

static void	remove_extra_space(std::string & str) {
	while (std::isspace(str[0]))
		str.erase(str.begin());
}

/* CONSTRUCTOR AND DESTRUCTOR */

Config::Config(void) {
	_port = 0;
	_client_max_body_size = 0;
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

/* SET */

char	Config::check_ip(void) const {
	struct sockaddr_in sa;
	return (!inet_pton(AF_INET, _host.c_str(), &(sa.sin_addr)));
}

char	Config::set_listen(std::string &content) {
	size_t			pos;
	unsigned int	expected_port;
	
	content.erase(0, 6);
	if (content.empty())
		return (1);
	if (_port && !_host.empty())
		return (1);
	remove_extra_space(content);

	pos = content.find(":");
	if (pos != std::string::npos) {
		expected_port = std::atoi(content.substr(pos + 1, content.length()).c_str());
		if (expected_port <= 0 || expected_port > 65535)
			return (1);
		_port = expected_port;
		_host = content.substr(0, pos);
	}
	else
		_host = content;

	if (_host == std::string("*"))
		_host = std::string("0.0.0.0");
	else if (_host == std::string("localhost"))
		_host = std::string("127.0.0.1");
	
	if (check_ip())
		return (1);
	content.erase();
	return (0);
}

char	Config::set_error_pages(std::string &content) {
	std::string	path;
	std::string	error_expected;
	size_t		error;
	size_t		pos;

	error = 0;
	content.erase(0, 10);
	if (content.empty())
		return (1);
	remove_extra_space(content);

	pos = content.find_first_of(" ");
	if (pos == std::string::npos)
		return (1);
	
	error_expected = content.substr(0, pos);
	if (error_expected.find_first_not_of("0123456789") != std::string::npos)
		return (1);
	error = std::atoi(error_expected.c_str());

	content.erase(0, pos);
	remove_extra_space(content);
	if (content.empty())
		return (1);

	if (content[0] != '/' || content.substr(content.length() - 6, 5) != std::string(".html"))
		return (1);
	_error_pages.push_back(std::pair<int, std::string>(error, content.substr(0, content.length() - 1)));

	// for (std::vector<std::pair<int, std::string> >::iterator it = _error_pages.begin(); it < _error_pages.end(); it++) {
	// 	std::cout << it->first << std::endl;
	// 	std::cout << it->second << std::endl;
	// }
	return (0);
}

char	Config::set_server_names(std::string &content) {
	std::string name;
	size_t 		pos;

	content.erase(0, 11);
	if (!_names.empty())
		return (1);
	remove_extra_space(content);
	if (content.empty())
		return (1);

	pos = content.find_first_of(" ");
	while (pos != std::string::npos) {
		_names.push_back(content.substr(0, pos));
		content.erase(0, pos);
		remove_extra_space(content);
		pos = content.find_first_of(" ");
	}
	_names.push_back(content.substr(0, content.length() - 1));
	content.erase();
	return (0);
}

char	Config::set_client_max_body_size(std::string &content) {
	content.erase(0, 20);
	if (content.empty())
		return (1);
	remove_extra_space(content);

	if (_client_max_body_size)
		return (1);
	if (content.find_first_not_of("0123456789MmGgKk;") != std::string::npos)
		return (1);
	_client_max_body_size = static_cast<size_t>(std::atol(content.c_str()));
	if (content[content.length() - 2] == 'G' || content[content.length() - 2] == 'g')
		_client_max_body_size *= 1000000000;
	else if (content[content.length() - 2] == 'M' || content[content.length() - 2] == 'm')
		_client_max_body_size *= 1000000;
	else if (content[content.length() - 2] == 'K' || content[content.length() - 2] == 'k')
		_client_max_body_size *= 1000;
	content.erase();
	return (0);
}

char	Config::set_root(std::string &content) {
	content.erase(0, 4);
	if (content.empty())
		return (1);
	remove_extra_space(content);

	if (!_root.empty())
		return (1);
	
	if (content[0] != '/' || content[content.length() - 2] != '/')
		return (1);
	_root = content.substr(0, content.length() - 1);
	content.erase();
	return (0);
}

/* PARSE LOCATION */

char	Config::set_location(std::ifstream & file, std::string &content, size_t &line_count) {
	t_location		location;
	size_t			pos;

	content.erase(0, 8);
	if (content.empty())
		return (1);
	remove_extra_space(content);
	if (content[0] != '/')
		return (1);
	pos = content.find_first_of(" ");
	if (pos == std::string::npos)
		return (1);
	location.path = content.substr(0, pos);
	content.erase(0, pos);
	remove_extra_space(content);
	if (content.empty() || content[0] != '{')
		return (1);

	location.client_max_body_size = 0;
	location.autoindex = false;
	
	while (std::getline(file, content)) {
		remove_extra_space(content);
		if (content.empty()) {
			line_count++;
			continue;
		}

		if (content[content.length() - 1] != ';' && content[0] != '}')
			return (1);
		if (!content.compare(0, 4, "root")) {
			set_location_root(content, location.root);
		}
		else if (!content.compare(0, 6, "method")) {
			set_method(content, location.methods);
		}
		else if (!content.compare(0, 9, "autoindex")) {
			set_autoindex(content, location.autoindex);
		}
		else if (!content.compare(0, 5, "index")) {
			set_index(content, location.index);
		}
		else if (!content.compare(0, 13, "cgi_extension")) {
			set_cgi_extension(content, location.cgi_extension);
		}
		else if (!content.compare(0, 8, "cgi_path")) {
			set_cgi_path(content, location.cgi_path);
		}
		else if (!content.compare(0, 20, "client_max_body_size")) {
			set_location_client_max_body_size(content, location.client_max_body_size);
		}
		else if (!content.compare(0, 20, "auth_basic_user_file")) {
			set_auth_basic_user_file(content, location.auth_basic_user_file);
		}
		else if (!content.compare(0, 10, "auth_basic")) {
			set_auth_basic(content, location.auth_basic);
		}
		else if (content[0] == '}') {
			_locations.push_back(location);
			return (0);
		}

		line_count++;
	}
	return (0);
}

/* PARSE */

char	Config::parse(std::ifstream & file, size_t & line_count) {
	std::string	content;

	while (std::getline(file, content)) {
		
		remove_extra_space(content);
		if (content.empty()) {
			line_count++;
			continue;
		}

		if (content[content.length() - 1] != ';' && content[0] != '}'
			&& content.compare(0, 8, "location"))
			return (1);
		if (!content.compare(0, 6, "listen")) {
			set_listen(content);
		}
		else if (!content.compare(0, 10, "error_page")) {
			set_error_pages(content);
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
			set_location(file, content, line_count);
		}
		else if (content[0] == '}') {
			break;
		}
		line_count++;
	}
	print();

	return (0);
}

void	Config::print(void) {
	std::cout << "----------------------------------" << std::endl << "- SERVER INFORMATIONS -" << std::endl;


	/* NAMES */
	std::cout << "server_names: ";
	for (std::vector<std::string>::iterator it = _names.begin(); it < _names.end(); it++) {
		std::cout << *it;
		if (it != _names.end() - 1)
			std::cout << ", ";
		else
			std::cout << std::endl;
	}

	/* ERROR PAGES */
	std::cout << "error_pages: ";
	for (std::vector<std::pair<int, std::string> >::iterator it = _error_pages.begin(); it < _error_pages.end(); it++) {
		std::cout << "[" << it->first << "]" << it->second;
		if (it != _error_pages.end() - 1)
			std::cout << ", ";
		else
			std::cout << std::endl;
	}

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

	for (std::vector<t_location>::iterator it = _locations.begin(); it < _locations.end(); it++) {
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
		for (std::vector<std::string>::iterator m = it->methods.begin(); m < it->methods.end(); m++) {
			std::cout << *m;
			if (m != it->methods.end() - 1)
				std::cout << ", ";
			else
				std::cout << std::endl;
		}

		std::cout << "cgi_extension: ";
		for (std::vector<std::string>::iterator m = it->cgi_extension.begin(); m < it->cgi_extension.end(); m++) {
			std::cout << *m;
			if (m != it->cgi_extension.end() - 1)
				std::cout << ", ";
			else
				std::cout << std::endl;
		}

		std::cout << "index: ";
		for (std::vector<std::string>::iterator m = it->index.begin(); m < it->index.end(); m++) {
			std::cout << *m;
			if (m != it->index.end() - 1)
				std::cout << ", ";
			else
				std::cout << std::endl;
		}
	}


	std::cout << std::endl << std::endl;
}