#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"
# include "Request.hpp"

class Response
{
private:
	
	std::string							_raw_response;
	int									_status;
	std::string							_method;
	std::string 						_path;
	std::map<std::string, std::string>	_headers;
	std::string							_body;
	int									_ret_code;


public:
	Response(Request &req);
	Response(const Response &c);
	Response& operator=(const Response &c);
	~Response();

	void	print_Response(void);
	void	get_method(void);

};

#endif