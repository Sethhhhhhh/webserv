#include "Cgi.hpp"

Cgi::Cgi(Request &request) {
	_init_envs(request);
	return ;
}

char    **Cgi::_map_to_table_char(std::map<std::string, std::string> map) {
	char        **envs_char;
	std::string s;
	size_t      j;

	j = 0;
	envs_char = new char*[map.size() + 1];
	for (std::map<std::string, std::string>::iterator i = map.begin(); i != map.end(); i++) {
		s = i->first + "=" + i->second;
		envs_char[j] = new char[s.size()];
		envs_char[j] = strdup(s.c_str());
		j++;
	}
	envs_char[j] = 0;
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

void	Cgi::_init_envs(Request &request) {
	std::map<std::string, std::string>	envs;

	envs["REDIRECT_STATUS"] = "200";
	envs["SERVER_PROTOCOL"] = "HTTP/1.1";
	envs["PATH_INFO"] = request.get_conf().path;
	envs["CONTENT_LENGTH"] = request._headers["Content-Length: "];
	envs["CONTENT_TYPE"] = request._headers["Content-Type: "];
	envs["GATEWAY_INTERFACE"] = "CGI/1.1";
	envs["PATH_TRANSLATED"] = request._conf.root + request._uri;
	envs["QUERY_STRING"] = request._uri.substr(request._uri.find("?") + 1);
	envs["REMOTE_ADDR"] = "";
	envs["REMOTE_HOST"] = "";
	envs["REMOTE_IDENT"] = "";
	envs["REMOTE_USER"] = "";
	envs["REQUEST_METHOD"] = request._method;
	envs["SERVER_PORT"] = to_string(request._conf.port);
	envs["SERVER_SOFTWARE"] = request._conf.names[0];
	envs["SERVER_NAME"] = request._headers["Host: "];
    envs["SCRIPT_NAME"] = request._conf.root + request._conf.cgi_path;
    for (std::map<std::string, std::string>::iterator it = request._headers.begin(); it != request._headers.end(); it++)
    {
        std::string tmp = "HTTP_";
        tmp += it->first;
        std::replace(tmp.begin(), tmp.end(), '-', '_');
        tmp = upper_case(tmp);
        envs[tmp] = upper_case(it->second);
    }

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

std::string	Cgi::execute(Request &request) {
	pid_t   		pid;
	int				status;
	int				ret_fd;
	int				fds[2];
    char            **args;

    args = (char**)malloc(sizeof(**args) * 3);
    args[0] = (char*)(request._conf.root + request._conf.cgi_path).c_str();
    args[1] = (char*)(request._conf.root + request._uri).c_str();
    args[2] = NULL;
	if (pipe(fds) == -1)
		exit(0);
	pid = fork();
	if (pid == -1) {
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid) {
		close(fds[1]);
		dup2(fds[0], 0);
	
		ret_fd = open("webserv_cgi", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

		dup2(ret_fd, 1);
		dup2(ret_fd, 2);

		std::cout << execve(args[0], args, _envs) << std::endl;
		close(ret_fd);
		close(fds[0]);

		exit(0);
	}
	close(fds[0]);
	write(fds[1], request.get_body().c_str(), request.get_body().length());
	close(fds[1]);
	wait(&status); 

	return (_read_file("webserv_cgi"));
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