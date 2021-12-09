#ifndef __CGI_HPP__
# define __CGI_HPP__

# include <string>
# include <map>
# include <string.h>
# include "Request.hpp"

class Cgi {

    public:
        Cgi(Request &request);
        ~Cgi(void);
        Cgi(const Cgi &);
        Cgi &   operator=(const Cgi &);

        std::string execute(void);

    private:
        char    **_map_to_table_char(std::map<std::string, std::string> map);
        void    _init_envs(Request &request);

        std::map<std::string, std::string>  _envs;
        std::string                         _body;
};

#endif