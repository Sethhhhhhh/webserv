#include "Request.hpp"

Request::Request()  : _status(START)
{
	_chunk.status = Request::UNCHUNKED;
}

Request::Request(const Request &req)
{
	*this = req;
}

Request& Request::operator=(const Request &c)
{
	if (this != &c)
	{
		_raw_request = c._raw_request;
		_status = c._status;
		_method = c._method;
		_version = c._version;
		_uri = c._uri;
		_headers = c._headers;
		_body = c._body;
		_lan = c._lan;
		_ret_code = c._ret_code;
		_chunk = c._chunk;
	}
	return *this;
}

Request::~Request()
{
}

void		Request::check_parsing(void)
{
	_ret_code = 200;
	// the minimum required information is METHOD, RESOURCE, PROTOCOL VERSION and at least the Host header
	if (_method.length() == 0 || (_method != "GET" && _method != "POST"
		&& _method != "HEAD" && _method != "PUT"
		&& _method != "CONNECT" && _method != "TRACE"
		&& _method != "OPTIONS" && _method != "DELETE"))
		_ret_code = 400;

	else if (_method != "GET" && _method != "POST"
			&& _method != "DELETE")
		_ret_code = 501;

	if (_uri.length() == 0 || _uri[0] != '/')
		_ret_code = 400;

	if (_version.length() == 0 || (_version != "HTTP/1.1\r" && _version != "HTTP/1.1"))
		_ret_code = 400;

	if (_headers.find("Host") == _headers.end())
		_ret_code = 400;
}

void	Request::parse_body()
{
	if (_headers.find("Content-Length") != _headers.end())
	{
		unsigned long length = atoi(_headers.find("Content-Length")->second.c_str());

		if (_raw_request.length() == length)
		{
			_body += _raw_request;
			_ret_code = 200;

		}
		else
			_ret_code = 400;
		_status = Request::DONE;
	}
	else if (_headers.find("Transfer-Encoding") != _headers.end())
	{
		parse_chunked_body();
	}
	else if (_raw_request.size() != 0)
		_ret_code = 411;
}

void		Request::parse_chunked_body(void)
{
	if (_chunk.status == Request::UNCHUNKED)
			_chunk.status = Request::BEGIN;
		while (_raw_request.size() != 0 && (_chunk.status != Request::FINISHED))
		{
			switch (_chunk.status)
			{
				case Request::BEGIN :
				{
					std::stringstream strm;
					unsigned int count;

					strm.str("");
					strm << std::hex << cut_line(_raw_request, true, 0);
					strm >> count;

					if (count == 0)
					{
						_chunk.status = Request::FINISHED;
						_status = Request::DONE;
					}
					else
					{
						_chunk.status = Request::FOUND;
						_chunk.length = count;
					}
					break;
				}
				case Request::FOUND :
					if (_raw_request.size() >= (unsigned long)_chunk.length)
					{
						_body += _raw_request.substr(0, _chunk.length);
						_raw_request.erase(0, _chunk.length + 2);
						_chunk.status = Request::BEGIN;
					}
					else
					{
						_body += _raw_request;
						_chunk.length -= _raw_request.size();
						_raw_request.erase(_raw_request.begin(), _raw_request.end());
						_chunk.status = Request::FOUND;
					}
					break;
			}
		}
}

void		Request::parse_language(void)
{
	if (_headers.find("Accept-Language") != _headers.end())
	{
		std::string		current;

		current = _headers["Accept-Language"];

		if (current.find(',') == std::string::npos)
			_lan = "en-US";
		else
		{
			while (current.find(',') != std::string::npos)
			{
				_lan +=	current.substr(0, current.find(',') + 1);
				if (_lan.find(';') != std::string::npos)
					_lan.erase(_lan.find(';'), _lan.length());
				current.erase(0, current.find(',') + 1);
			}
			_lan += current;
			if (_lan.find(';') != std::string::npos)
					_lan.erase(_lan.find(';'), _lan.length());
		}
	}
}

void		Request::parse(std::string str)
{
	std::string		line;
	std::string		key;
	std::string		value;

	_raw_request = str;
	if (_status == Request::START)
	{
		line = cut_line(_raw_request, true, 0);
		_method = line.substr(0, line.find(" "));
		line.erase(0, _method.length() + 1);
		_uri = line.substr(0, line.find(" "));
		line.erase(0, _uri.length() + 1);
		_version = line.substr(0, line.find(" "));
		line = cut_line(_raw_request, true, 0);
		_status = Request::HEADERS;
	}
	while (_status == Request::HEADERS && line.find(":") != std::string::npos)
	{
		key = line.substr(0, line.find(":"));
		value = line.substr(line.find(":") + 2);
		_headers[key] = value;
		line = cut_line(_raw_request, true, 0);
	}
	_status = Request::BODY;
	parse_language();
	check_parsing();
	if (_method == "POST" && _status == Request::BODY)
		parse_body();
	else
		_status = Request::DONE;
	print_request();
}


void	Request::print_request(void)
{
	std::cout << "method : " << _method << std::endl;
	std::cout << "URI : " << _uri << std::endl;
	std::cout << "version : " << _version << std::endl;
	std::cout << "languages : " << _lan << std::endl;
	std::cout << "headers :" << std::endl;
	std::map<std::string, std::string>::iterator it = _headers.begin();
	for (; it != _headers.end(); ++it)
	{
		std::cout << it->first << " : " << it->second << std::endl;
	}
	std::cout << "body :\n" << std::endl;
	std::cout << _body << std::endl;
}