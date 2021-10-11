#include "Response.hpp"

Response::Response(Request &req) : _ret_code(req._ret_code), _path(req._uri), _status(Request::START)
{
    _headers["Content-Language: "] = req._lan;
    _headers["Server: "] = "Luigi's Mansion";
    _raw_response = req._version + " ";
}

Response::Response(const Response &res)
{
    *this = res;
}

Response& Response::operator=(const Response &res)
{
    if (this != &res)
    {
        _raw_response = res._raw_response;
        _ret_code = res._ret_code;
        _status = res._status;
        _path = res._path;
        _method = res._method;
        _headers = res._headers;
        _body = res._body;
    }
    return *this;
}

Response::~Response()
{
}

void Response::print_Response(void)
{
    std::cout << _raw_response << std::endl;
}

void    Response::get_method(void)
{
    
}