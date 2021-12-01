#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"
# include "Request.hpp"

class Response
{
private:
	
	Request	&							_req;
	std::string							_method;
	std::string 						_path;
	std::map<std::string, std::string>	_headers;
	std::string							_body;
	int									_ret_code;
	std::map<std::string, void (Response::*)(void)> _methods;


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
	void    									generate_raw_response(void);
	std::map<std::string, void (Response::*)(void)>	methods_map(void);

	std::string							_raw_response;

};

#endif