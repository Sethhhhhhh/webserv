#include "Client.hpp"

Client::Client() : _status(START)
{
	_chunk.status = Client::UNCHUNKED;
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
	_ret_code = 200;
	// the minimum required information is METHOD, RESOURCE, PROTOCOL VERSION and at least the Host header
	if (_method.length() == 0 || (_method != "GET" && _method != "POST"
		&& _method != "HEAD" && _method != "PUT"
		&& _method != "CONNECT" && _method != "TRACE"
		&& _method != "OPTIONS" && _method != "DELETE"))
		_ret_code = 400;

	else if (_method != "GET" && _method != "POST"
			&& _method != "DELETE")
		_ret_code = 405;

	if (_uri.length() == 0 || _uri[0] != '/')
		_ret_code = 400;

	if (_version.length() == 0 || (_version != "HTTP/1.1\r" && _version != "HTTP/1.1"))
		_ret_code = 400;

	if (_headers.find("Host") == _headers.end())
		_ret_code = 400;
}

void	Client::parse_body()
{
	if (_headers.find("Content-Length") != _headers.end())
	{
		unsigned long length = atoi(_headers.find("Content-Length")->second.c_str());

		if (_buffer.length() == length)
		{
			_body += _buffer;
			_ret_code = 200;

		}
		else
			_ret_code = 400;
		_status = DONE;
	}
	else if (_headers.find("Transfer-Encoding") != _headers.end())
	{
		parse_chunked_body();
	}
	else if (_buffer.size() != 0)
		_ret_code = 411;
}

void		Client::parse_chunked_body(void)
{
	if (_chunk.status == Client::UNCHUNKED)
			_chunk.status = Client::BEGIN;
		while (_buffer.size() != 0 && (_chunk.status != Client::FINISHED))
		{
			switch (_chunk.status)
			{
				case Client::BEGIN :
				{
					std::stringstream strm;
					unsigned int count;

					strm.str("");
					strm << std::hex << cut_line(_buffer, true, 0);
					strm >> count;

					if (count == 0)
					{
						_chunk.status = Client::FINISHED;
						_status = DONE;
					}
					else
					{
						_chunk.status = Client::FOUND;
						_chunk.length = count;
					}
					break;
				}
				case Client::FOUND :
					if (_buffer.size() >= (unsigned long)_chunk.length)
					{
						_body += _buffer.substr(0, _chunk.length);
						_buffer.erase(0, _chunk.length + 2);
						_chunk.status = Client::BEGIN;
					}
					else
					{
						_body += _buffer;
						_chunk.length -= _buffer.size();
						_buffer.erase(_buffer.begin(), _buffer.end());
						_chunk.status = Client::FOUND;
					}
					break;
			}
		}
}

void		Client::parse_language(void)
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
	parse_language();
	check_parsing();
	if (_method == "POST" && _status == BODY)
		parse_body();
	else
		_status = DONE;
	print_request();
}


void	Client::print_request(void)
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
