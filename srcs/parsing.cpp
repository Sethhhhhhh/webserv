#include "../class/Config/Config.hpp"

size_t	count_char_in_string(std::string & s, char c) {
	size_t	count;

	count = 0;
	for (size_t	i = 0; (i = s.find(c, i)) != std::string::npos; i++)
		count++;
	return (count);
}

void	remove_extra_space(std::string & str, size_t pos) {
	while (std::isspace(str[pos]))
		str.erase(str.begin() + pos);
}

char	parse(std::vector<Server*> servers, char *path) {
	std::ifstream	file(path);
	std::string		content;
	Config			config;

	size_t			line_count;

	if (!file.is_open()) {
		return (1);
	}

	line_count = 0;
	while (std::getline(file, content)) {
		
		remove_extra_space(content, 0);
			
		if (content.empty()) {
			line_count++;
			continue;
		}
		
		if (!content.compare(0, 5, "server")) {
			return (1);
		}
		remove_extra_space(content, 6);
		
		if (!content.compare(0, 6, "server{")) {
			return (1);
		}
		
		Server	serv;
		if (config.parse(serv, file, line_count)) {
			return (1);
		}
		servers.push_back(&serv);
	}
	return (0);
}
