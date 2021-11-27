#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../../includes/webserv.hpp"
# include "../Request/Request.hpp"

class Response
{
private:
	
	Request	&							_req;
	std::string							_raw_response;
	std::string							_method;
	std::string 						_path;
	std::map<std::string, std::string>	_headers;
	std::string							_body;
	int									_ret_code;
	std::map<std::string, void (Response::*)()> _methods;


public:
	Response(Request &req);
	Response(const Response &c);
	Response& operator=(const Response &c);
	~Response();

	void    									init_response(void);
	void										print_Response(void);
	void										get_method(void);
	void										post_method(void);
	void										delete_method(void);
	void										generate_error_page(int);
	std::map<std::string, void (Response::*)()>	methods_map(void);

};

#endif