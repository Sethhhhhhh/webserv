#include "Client.hpp"

Client::Client() : _status(START)
{
}

Client::Client(const Client &c) : _fd(c._fd), _buffer(c._buffer), _server(c._server)
{
	(void)c;
}

Client& Client::operator=(const Client &c)
{
	(void)c;

	return *this;
}

Client::~Client()
{

}

void		Client::check_parsing(void)
{
	// the minimum required information is METHOD, RESOURCE, PROTOCOL VERSION and at least the Host header
	if (_method.length() == 0 || (_method != "GET" && _method != "POST"
		&& _method != "HEAD" && _method != "PUT"
		&& _method != "CONNECT" && _method != "TRACE"
		&& _method != "OPTIONS" && _method != "DELETE"))
		_ret = 400;
	if (_uri.length() == 0 || _uri[0] != '/')
		_ret = 400;
	if (_version.length() == 0 || (_version != "HTTP/1.1\r" && _version != "HTTP/1.1"))
		_ret = 400;
	if (_headers.find("Host") == _headers.end())
		_ret = 400;
	_ret = 200;
}

void	Client::parse_body()
{
	//parse based on content type
}

void		Client::parse(void)
{
	std::string		line;
	std::string		key;
	std::string		value;

	if (_status == START)
	{
		line = cut_line(_buffer, true, 0);
		_method = line.substr(0, line.find(" "));
		line.erase(0, _method.length() + 1);
		_uri = line.substr(0, line.find(" "));
		line.erase(0, _uri.length() + 1);
		_version = line.substr(0, line.find(" "));
		line = cut_line(_buffer, true, 0);
		_status = HEADERS;
	}
	while (_status == HEADERS && line.find(":") != std::string::npos)
	{
		key = line.substr(0, line.find(":"));
		value = line.substr(line.find(":") + 2);
		_headers[key] = value;
		line = cut_line(_buffer, true, 0);
	}
	_status = BODY;
	print_request();
	check_parsing();
	if (_method == "POST" && _status == BODY)
		parse_body();
	else
		_status = DONE;
}


void	Client::print_request(void)
{
	std::cout << "method : " << _method << std::endl;
	std::cout << "version : " << _version << std::endl;
	std::cout << "headers :" << std::endl;
	std::map<std::string, std::string>::iterator it = _headers.begin();
	for (; it != _headers.end(); ++it)
	{
		std::cout << it->first << " : " << it->second << std::endl;
	}
}
