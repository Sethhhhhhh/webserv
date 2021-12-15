#include "Cgi.hpp"

Cgi::Cgi(Response &response) {
	_init_envs(response);
	execute();
	return ;
}

char    **Cgi::_map_to_table_char(std::map<std::string, std::string> map) {
	char        **envs_char;
	std::string s;
	size_t      j;

	j = 0;
	std::cout << "--------------" << std::endl;
	envs_char = new char*[map.size()];
	for (std::map<std::string, std::string>::iterator i = map.begin(); i != map.end(); i++) {
		s = i->first + "=" + i->second;
		envs_char[j] = new char[s.size()];
		envs_char[j] = strdup(s.c_str());
		std::cout << envs_char[j] << std::endl;
		j++;
	}
	std::cout << "--------------" << std::endl;
	return (envs_char);
}

std::string	Cgi::_get_query_string(std::string uri) {
	std::string	query;
	size_t		pos;
	
	pos = uri.find_first_of("?");
	if (pos != std::string::npos)
		query = uri.substr(uri.find_first_of("?") + 1, uri.size());
	return (query);
}

void	Cgi::_init_envs(Response &response) {
	std::map<std::string, std::string>	envs;

	envs["REDIRECT_STATUS"] = "200";
	envs["SERVER_PROTOCOL"] = "HTTP/1.1";
	envs["PATH_INFO"] = response.get_path();
	envs["CONTENT_LENGTH"] = response.get_headers()["Content-length: "];
	envs["CONTENT_TYPE"] = response.get_headers()["Content-type: "];
	envs["GATEWAY_INTERFACE"] = "CGI/1.1";
	envs["PATH_TRANSLATED"] = response.get_path();
	envs["QUERY_STRING"] = _get_query_string(response.get_request().get_uri());
	envs["REMOTE_ADDR"] = "";
	envs["REMOTE_HOST"] = "";
	envs["REMOTE_IDENT"] = "";
	envs["REMOTE_USER"] = "";
	envs["REQUEST_METHOD"] = response.get_methodd();
	envs["SERVER_PORT"] = to_string(response.get_request().get_conf().port);
	envs["SERVER_SOFTWARE"] = "Webserv";
	envs["SERVER_NAME"] = response.get_request().get_conf().host;
	

	_envs = _map_to_table_char(envs);
}

std::string	Cgi::_read_file(std::string path) {
	std::string		s;
	std::string		buff;
	std::ifstream	file(path.c_str());

	if (!file.is_open())
		return (s);
	while(std::	getline(file, buff)) {
		s += buff;
	}
	file.close();

	return (s);
}

std::string	Cgi::execute(void) {
	char * const 	*args = NULL;
	pid_t   		pid;
	int				status;
	int				ret_fd;
	int				fds[2];

	if (pipe(fds) == -1)
		exit(0);
	pid = fork();
	if (pid == -1) {
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid) {
		close(fds[1]);
		dup2(fds[0], 0);
	
		ret_fd = open("/tmp/webserv_cgi", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		std::cout << "ok" << std::endl;

		dup2(ret_fd, 1);
		execve("ubuntu_cgi_tester", args, _envs);
	
		close(ret_fd);
		close(fds[0]);

		exit(0);
	}
	close(fds[0]);
	// write(fds[1], )
	close(fds[1]);
	wait(&status); 

	return (_read_file("/tmp/webserv_cgi"));
}

Cgi::~Cgi(void) {
	return ;
}

Cgi::Cgi(const Cgi & src) {
	(void)src;

	return ;
}

Cgi &   Cgi::operator=(const Cgi & src) {
	(void)src;

	return (*this);
}