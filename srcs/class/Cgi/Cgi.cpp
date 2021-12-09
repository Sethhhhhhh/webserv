#include "Cgi.hpp"

Cgi::Cgi(Request &request) {
    _init_envs(request);
    return ;
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

void    Cgi::_init_envs(Request &request) {

    (void)request;

    _envs["REDIRECT_STATUS"] = "200";
    _envs["SERVER_PROTOCOL"] = "HTTP/1.1";
    _envs["PATH_INFO"] = request.get_conf().path;
}

char    **Cgi::_map_to_table_char(std::map<std::string, std::string> map) {
    char        **envs_char;
    std::string s;
    size_t      j;

    j = 0;
    envs_char = new char*[map.size()];
    for (std::map<std::string, std::string>::iterator i = map.begin(); i != map.end(); i++) {
        s = i->first + "=" + i->second;
        envs_char[j] = new char[s.size()];
        envs_char[j] = strdup(s.c_str());
        j++;
    }
    return (envs_char);
}

std::string Cgi::execute(void) {
    char    **envs_char;
    pid_t   pid;

    envs_char = _map_to_table_char(_envs);

    pid = fork();
    if (pid == -1) {
        return ("Status: 500\r\n\r\n");
    }
    else if (!pid) {
        std::cout << "l" << std::endl;
    }
    else {
        std::cout << "l" << std::endl;
    }
    _body = "lol";
    return (_body);
}