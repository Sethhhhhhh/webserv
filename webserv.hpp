#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <unistd.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>

# include <cstring>
# include <iostream>
# include <string>
# include <algorithm>
# include <vector>
# include <map>
# include <sstream>
# include <fstream>


# include "Tools.hpp"

# define	START 	0
# define	HEADERS	1
# define	BODY	2
# define	DONE	3

typedef struct  s_chunk
{
    int         length;
	int			status;

}               t_chunk;

#endif
