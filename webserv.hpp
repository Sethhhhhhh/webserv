#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <unistd.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <sys/stat.h>
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


# include "Tools.hpp"

#ifndef __APPLE__

#define	TIME	st_mtim

#else

#define	TIME	st_mtimespec

#endif

typedef struct  s_chunk
{
    int         length;
	int			status;

}               t_chunk;

size_t	count_char_in_string(std::string & s, char c);
void	remove_extra_space(std::string & str, size_t pos);

#endif
