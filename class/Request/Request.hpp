#ifndef REQUEST_H
# define REQUEST_H

# include "../../includes/webserv.hpp"

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


	public:
		Request();
		Request(const Request &c);
		Request& operator=(const Request &c);
		~Request();

		void	parse(std::string str, std::vector<s_location> &locs);
		void	parse_config(std::vector<s_location> &locs);
		void	print_request(void);
		void	check_parsing(void);
		void	parse_body(void);
		void	parse_chunked_body(void);
		void	parse_language(void);

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