#include "Config.hpp"

char	Config::set_location_root(std::string &content, std::string &root, int line_count) {
	content.erase(0, 4);
	remove_extra_space(content, 0);
	if (content.substr(0, content.size() - 1).empty())
		throw Error("You must specified a path for the root.", line_count);

	if (!root.empty())
		throw Error("Root already specified.", line_count);

	if (content[0] != '/')
		throw Error("Only absolute path accepted.", line_count);
	root = content.substr(0, content.length() - 1);
	content.erase();

	return (0);
}

char	Config::set_location_client_max_body_size(std::string &content, size_t & client_max_body_size, int line_count) {
	content.erase(0, 20);
	remove_extra_space(content, 0);
	if (content.substr(0, content.size() - 1).empty())
		throw Error("You must specify a client max body size.", line_count);

	if (client_max_body_size)
		throw Error("Client max body size already specified.", line_count);
	if (content.find_first_not_of("0123456789MmGgKk;") != std::string::npos)
		throw Error("Bad format.", line_count);
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

char	Config::set_autoindex(std::string &content, bool & autoindex, int line_count) {
	std::string	value;

	content.erase(0, 9);
	remove_extra_space(content, 0);
	if (content.substr(0, content.size() - 1).empty())
		throw Error("No content.", line_count);

	value = content.substr(0, content.length() - 1);
	if (value == std::string("off"))
		autoindex = false;
	else if (value == std::string("on"))
		autoindex = true;
	else
		throw Error("Bad format.", line_count);
	content.erase();

	return (0);
}

char	Config::set_cgi_extension(std::string &content, std::vector<std::string> &cgi_extension, int line_count) {
	std::string	extension_expected;
	size_t	pos;

	content.erase(0, 13);
	remove_extra_space(content, 0);
	if (content.substr(0, content.size() - 1).empty())
		throw Error("You must specify a cgi extension.", line_count);

	pos = content.find_first_of(" ");
	while (pos != std::string::npos) {
		extension_expected = content.substr(0, pos);
		if (extension_expected[0] != '.')
			return (1);
		cgi_extension.push_back(extension_expected);
		content.erase(0, pos + 1);

		pos = content.find_first_of(" ");
	}
	if (content[0] != '.')
		throw Error("No dot in your extension.", line_count);

	cgi_extension.push_back(content.substr(0, content.length() - 1));
	content.erase();

	return (0);
}

char	Config::set_method(std::string &content, std::vector<std::string> &method, int line_count) {
	std::string	method_expected;
	size_t		pos;
	
	content.erase(0, 9);
	remove_extra_space(content, 0);
	if (content.substr(0, content.size() - 1).empty())
		throw Error("You must specify one or more methods between GET POST and DELETE.", line_count);
	
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
		throw Error("Unknown method.", line_count);
	method.push_back(method_expected);
	content.erase();

	return (0);
}

char	Config::set_index(std::string &content, std::vector<std::string> &index, int line_count) {
	std::string	file;
	size_t		pos;
	
	content.erase(0, 5);
	remove_extra_space(content, 0);
	if (content.substr(0, content.size() - 1).empty())
		throw Error("You must specify an index.", line_count);

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
		throw Error("Only .html or .php extension accepted.", line_count);
	index.push_back(content.substr(0, content.length() - 1));
	content.erase();

	return (0);
}

char	Config::set_cgi_path(std::string &content, std::string &cgi_path, int line_count) {
	content.erase(0, 9);
	remove_extra_space(content, 0);
	if (content.substr(0, content.size() - 1).empty())
		throw Error("You must specify a cgi path.", line_count);
	
	if (content[0] != '/' && content.find_first_of(" ") == std::string::npos)
		throw Error("The path must be absolute.", line_count);
	
	cgi_path = content.substr(0, content.length() - 1);
	content.erase();

	return (0);
}

char	Config::set_auth_basic(std::string &content, std::string &auth_basic, int line_count) {
	size_t	pos;

	content.erase(0, 10);
	remove_extra_space(content, 0);
	if (content.substr(0, content.size() - 1).empty())
		throw Error("You must specify an auth basic.", line_count);

	pos = content.find_first_of("\"");
	while (pos != std::string::npos) {
		content.erase(content.begin() + pos);
		pos = content.find_first_of("\"");
	}
	auth_basic = content.substr(0, content.length() - 1);
	content.erase();

	return (0);
}

char	Config::set_auth_basic_user_file(std::string &content, std::string &auth_basic_user_file, int line_count) {
	content.erase(0, 20);
	remove_extra_space(content, 0);
	if (content.substr(0, content.size() - 1).empty())
		throw Error("You must specify an auth basic user file.", line_count);
	
	if (content[0] != '/' && content.find_first_of(" ") == std::string::npos)
		throw Error("Only absolute path accepted.", line_count);
	
	auth_basic_user_file = content.substr(0, content.length() - 1);
	content.erase();

	return (0);
}