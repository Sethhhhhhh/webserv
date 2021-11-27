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

	if (std::find(_conf.methods.begin(), _conf.methods.end(), _method) == _conf.methods.end())
		_ret_code = 405;
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

void		Request::parse_config(s_config conf)
{
	std::string		match = "";
	s_location		tmp;

	for (unsigned long i = 0; i < conf.locations.size(); i++)
	{
		if (conf.locations[i].path == _uri)
		{
			match = conf.locations[i].path;
			tmp = conf.locations[i];
			break ;
		}
		if (_uri.find(conf.locations[i].path) != std::string::npos && _uri.find(conf.locations[i].path) == 0)
		{
			if (conf.locations[i].path.length() > match.length())
			{
				match = conf.locations[i].path;
				tmp = conf.locations[i];
			}
		}
	}
	_conf.error_pages = tmp.error_pages;
	_conf.error_pages.insert(conf.error_pages.begin(), conf.error_pages.end());
	_conf.names = conf.names;
	_conf.host = conf.host;
	_conf.root = (tmp.root != "") ? tmp.root : conf.root;
	_conf.methods = tmp.methods;
	_conf.cgi_extension = tmp.cgi_extension;
	_conf.index = tmp.index ;
	_conf.cgi_path = tmp.cgi_path;
	_conf.upload_path = tmp.upload_path;
	_conf.auth_basic_user_file = tmp.auth_basic_user_file;
	_conf.auth_basic = tmp.auth_basic;
	_conf.upload_eanable = tmp.upload_eanable;
	_conf.autoindex = tmp.autoindex;
	_conf.client_max_body_size = tmp.client_max_body_size;
}

void		Request::parse(std::string str, s_config conf)
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
		parse_config(conf);
		print_config();
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

void	Request::print_config(void)
{
	std::cout << "REQUEST CONFIG\n#######" << std::endl;
	std::cout << "error pages : ";
	for (std::map<int, std::string>::iterator it = _conf.error_pages.begin(); it != _conf.error_pages.end(); it++)
		std::cout << it->first << " " << it->second << std::endl;
	std::cout << "names : ";
	for (std::vector<std::string>::iterator it = _conf.names.begin(); it != _conf.names.end(); it++)
		std::cout << *it << " ";
	std::cout << "\nhost : " << _conf.host << std::endl;
	std::cout << "root : " << _conf.root << std::endl;
	std::cout << "methods : ";
	
	for (std::vector<std::string>::iterator it = _conf.methods.begin(); it != _conf.methods.end(); it++)
		std::cout << *it << " ";
	std::cout << "\ncgi extension  ";
	for (std::vector<std::string>::iterator it = _conf.cgi_extension.begin(); it != _conf.cgi_extension.end(); it++)
		std::cout << *it << " ";
	std::cout << "\ncgi path " << _conf.cgi_path  << std::endl;
	std::cout << "index : ";
	for (std::vector<std::string>::iterator it = _conf.index.begin(); it != _conf.index.end(); it++)
		std::cout << *it << " ";
	std::cout << "upload_path " << _conf.upload_path << std::endl;
	std::cout << "auth_basic_user_file " << _conf.auth_basic_user_file << std::endl;
	std::cout << "auth_basic " << _conf.auth_basic << std::endl;
	std::cout << "upload_eanable " << _conf.upload_eanable << std::endl;
	std::cout << "auto index " <<std::boolalpha << 	_conf.autoindex << std::endl;
	std::cout << "client body size " << _conf.client_max_body_size << std::endl;
	std::cout << "##########" << std::endl;
} 