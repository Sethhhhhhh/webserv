#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <unistd.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <fcntl.h>
# include <time.h>

# include <cstring>
# include <iostream>
# include <string>
# include <algorithm>
# include <vector>
# include <list>
# include <map>
# include <sstream>
# include <fstream>

#ifndef __APPLE__

#define	TIME	st_mtim

#else

#define	TIME	st_mtimespec

#endif

/* Location struct */
struct	s_location {
    std::map<int, std::string>  error_pages;
    std::vector<std::string>	methods;
    std::vector<std::string>	cgi_extension;
    std::vector<std::string>	index;

    std::string					path;
    std::string					root;
    std::string					cgi_path;
    std::string					upload_path;
    std::string					auth_basic_user_file;
    std::string					auth_basic;

    bool						upload_eanable;
    bool						autoindex;

    unsigned int				client_max_body_size;
};

struct  s_config {
    std::map<int, std::string>					error_pages;
    std::vector<std::string>					names;
    std::vector<s_location>						locations;
    std::string									host;
    std::string									root;
    unsigned int								port;
    unsigned int								client_max_body_size;
    int											fd;
};

typedef struct  s_chunk
{
    int         length;
	int			status;

}               t_chunk;

size_t	        count_char_in_string(std::string & s, char c);
void	        remove_extra_space(std::string & str, size_t pos);

std::string		cut_line(std::string &str, bool cond, int back);
std::string		MIME_types(std::string	str);
std::string		Last_modified(std::string &path);
std::string	    status_code(int code);
std::string     read_html(std::string path, std::string &body);
std::string     to_string(int n);
std::string		get_current_time(void);

#endif
