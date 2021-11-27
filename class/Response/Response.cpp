#include "Response.hpp"

Response::Response(Request &req) : _req(req), _path(req._uri), _ret_code(req._ret_code)
{
    
    if (req._body.length() > req._conf.client_max_body_size)
        _ret_code = 413;
    _headers["Content-Language: "] = req._lan;
    _headers["Server: "] = "Luigi's Mansion";
    _raw_response = req._version + " ";
    _methods = methods_map();
}

Response::Response(const Response &res) : _req(res._req), _path(res._path), _ret_code(res._ret_code)
{
    *this = res;
}

Response& Response::operator=(const Response &res)
{
    if (this != &res)
    {
        _raw_response = res._raw_response;
        _ret_code = res._ret_code;
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

std::map<std::string, void (Response::*)()>    Response::methods_map(void)
{
    std::map<std::string, void (Response::*)()>    map;

    map["GET"] = &Response::get_method;
    map["POST"] = &Response::post_method;
    map["DELETE"] = &Response::delete_method;

    return (map);
}

void Response::print_Response(void)
{
    std::cout << _raw_response << std::endl;
}

void    Response::init_response(void)
{
    if (_ret_code >= 400)
        generate_error_page(_ret_code);
    else
        _methods[_method];
}

void    Response::get_method(void)
{
    
}

void    Response::post_method(void)
{

}

void    Response::delete_method(void)
{
    
}

void    Response::generate_error_page(int error_code)
{
    _raw_response += status_code(error_code);
    if (error_code == 405)
    {
        std::string     methods;
        for (std::vector<std::string>::iterator it = _req._conf.methods.begin(); it != _req._conf.methods.end(); it++)
        {
            methods += *it;
            if ((it + 1) != _req._conf.methods.end())
                methods += ", ";
        }
        _headers["Allow: "] = methods;
        _headers["Content-Length: "] = read_html(_req._conf.root + _req._conf.error_pages[405], _body);
    }
    if (error_code == 413)
    {
        _headers["Content-Length: "] = read_html(_req._conf.root + _req._conf.error_pages[413], _body);
    }
}