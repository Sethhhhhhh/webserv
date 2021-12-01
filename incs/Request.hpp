#ifndef REQUEST_H
# define REQUEST_H

# include "webserv.hpp"

struct	s_request_config
{
	std::map<int, std::string>					error_pages;
	std::vector<std::string>					names;
	std::string									host;
	std::string									path;
	std::string									root;
	std::vector<std::string>					methods;
	std::vector<std::string>					cgi_extension;
    std::vector<std::string>					index;
	std::string									cgi_path;
    std::string									upload_path;
    std::string									auth_basic_user_file;
    std::string									auth_basic;
    bool										upload_eanable;
    bool										autoindex;
    size_t										client_max_body_size;
};

class Request {
	private:
		
		std::string							_raw_request;
		int									_status;
		std::string							_method;
		std::string							_version;
		std::string 						_uri;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_lan;
		int									_ret_code;
		t_chunk								_chunk;
		s_request_config					_conf;


	public:
		Request();
		Request(const Request &c);
		Request& operator=(const Request &c);
		~Request();

		void	parse(std::string str, s_config conf);
		void	parse_config(s_config conf);
		void	print_request(void);
		void	check_parsing(void);
		void	parse_body(void);
		void	parse_chunked_body(void);
		void	parse_language(void);
		void	print_config(void);
		

		enum         status
		{
			UNCHUNKED,
			BEGIN,
			FOUND,
			FINISHED
		};

		enum         stage
		{
			START,
			HEADERS,
			BODY,
			DONE
		};
		friend class Response;
};

#endif