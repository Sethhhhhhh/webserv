#include "Config.hpp"

char	Config::set_location_root(std::string &content, std::string &root) {
	content.erase(0, 4);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);

	if (!root.empty())
		return (1);

	if (content[0] != '/' || content[content.length() - 2] != '/')
		return (1);
	root = content.substr(0, content.length() - 1);
	content.erase();

	return (0);
}

char	Config::set_location_client_max_body_size(std::string &content, size_t & client_max_body_size) {
	content.erase(0, 20);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);

	if (client_max_body_size)
		return (1);
	if (content.find_first_not_of("0123456789MmGgKk;") != std::string::npos)
		return (1);
	client_max_body_size = static_cast<size_t>(std::atol(content.c_str()));
	if (content[content.length() - 2] == 'G' || content[content.length() - 2] == 'g')
		client_max_body_size *= 1000000000;
	else if (content[content.length() - 2] == 'M' || content[content.length() - 2] == 'm')
		client_max_body_size *= 1000000;
	else if (content[content.length() - 2] == 'K' || content[content.length() - 2] == 'k')
		client_max_body_size *= 1000;
	content.erase();

	// std::cout << client_max_body_size << std::endl;
	return (0);
}

char	Config::set_autoindex(std::string &content, bool & autoindex) {
	std::string	value;

	content.erase(0, 9);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);


	value = content.substr(0, content.length() - 1);
	if (value == std::string("off"))
		autoindex = false;
	else if (value == std::string("on"))
		autoindex = true;
	else
		return (1);
	content.erase();

	// std::cout << std::boolalpha << autoindex << std::endl;
	return (0);
}

char	Config::set_cgi_extension(std::string &content, std::vector<std::string> &cgi_extension) {
	std::string	extension_expected;
	size_t	pos;


	content.erase(0, 13);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);

	pos = content.find_first_of(" ");
	while (pos != std::string::npos) {
		extension_expected = content.substr(0, pos);
		if (extension_expected[0] != '.')
			return (1);
		cgi_extension.push_back(extension_expected);
		content.erase(0, pos + 1);

		pos = content.find_first_of(" ");
	}
	if (content[0] != '.') {
		std::cout << "lol" << std::endl;
		return (1);
	}

	cgi_extension.push_back(content.substr(0, content.length() - 1));
	content.erase();

	// for (std::vector<std::string>::iterator it = cgi_extension.begin(); it < cgi_extension.end(); it++) {
	// 	std::cout << *it << std::endl;
	// }
	return (0);
}

char	Config::set_method(std::string &content, std::vector<std::string> &method) {
	std::string	method_expected;
	size_t		pos;
	
	content.erase(0, 9);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);
	
	pos = content.find_first_of(" ");
	while (pos != std::string::npos) {
		method_expected = content.substr(0, pos);
		if (method_expected != "GET" && method_expected != "POST" && method_expected != "DELETE")
			return (1);
		method.push_back(method_expected);
		content.erase(0, pos + 1);
		pos = content.find_first_of(" ");
	}
	method_expected = content.substr(0, content.length() - 1);
	if (method_expected != "GET" && method_expected != "POST" && method_expected != "DELETE")
		return (1);
	method.push_back(method_expected);
	content.erase();

	// for (std::vector<std::string>::iterator it = method.begin(); it < method.end(); it++) {
	// 	std::cout << *it << std::endl;
	// }
	return (0);
}

char	Config::set_index(std::string &content, std::vector<std::string> &index) {
	std::string	file;
	size_t		pos;
	
	content.erase(0, 5);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);

	pos = content.find_first_of(" ");
	while (pos != std::string::npos) {
		file = content.substr(0, pos);

		if (file.substr(file.find_last_of(".")) != ".html" && file.substr(file.find_last_of(".")) != ".php")
			return (1);
		index.push_back(file);
		content.erase(0, pos + 1);
		pos = content.find_first_of(" ");
	}

	pos = content.find_last_of(".");
	if (content.substr(pos, 5) != ".html" && content.substr(pos, 4) != ".php")
		return (1);
	index.push_back(content.substr(0, content.length() - 1));
	content.erase();

	// for (std::vector<std::string>::iterator it = index.begin(); it < index.end(); it++) {
	// 	std::cout << *it << std::endl;
	// }
	return (0);
}

char	Config::set_cgi_path(std::string &content, std::string &cgi_path) {
	content.erase(0, 9);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);
	
	if (content[0] != '/' && content.find_first_of(" ") == std::string::npos)
		return (1);
	
	cgi_path = content.substr(0, content.length() - 1);
	content.erase();

	// std::cout << cgi_path << std::endl;
	return (0);
}

char	Config::set_auth_basic(std::string &content, std::string &auth_basic) {
	size_t	pos;

	content.erase(0, 10);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);

	pos = content.find_first_of("\"");
	while (pos != std::string::npos) {
		content.erase(content.begin() + pos);
		pos = content.find_first_of("\"");
	}
	
	auth_basic = content.substr(0, content.length() - 1);
	content.erase();

	// std::cout << auth_basic << std::endl;
	return (0);
}

char	Config::set_auth_basic_user_file(std::string &content, std::string &auth_basic_user_file) {
	content.erase(0, 20);
	if (content.empty())
		return (1);
	remove_extra_space(content, 0);
	
	if (content[0] != '/' && content.find_first_of(" ") == std::string::npos)
		return (1);
	
	auth_basic_user_file = content.substr(0, content.length() - 1);
	content.erase();

	// std::cout << auth_basic_user_file << std::endl;
	return (0);
}