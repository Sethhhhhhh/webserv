#ifndef SERVER_H
# define SERVER_H

# include "../Connections/Connections.hpp"
# include "../Request/Request.hpp"

class Server {

	public:
		/* Location struct */
		struct	s_location {
			std::vector<std::string>	methods;
			std::vector<std::string>	cgi_extension;
			std::vector<std::string>	index;

			std::string					path;
			std::string					root;
			std::string					cgi_path;
			std::string					upload_path;
			std::string					auth_basic_user_file;
			std::string					auth_basic;

			bool						upload_eanable;
			bool						autoindex;

			size_t						client_max_body_size;
		};

		/* Constructor & Destructor */
		Server();
		Server(const Server &c);
		Server& operator=(const Server &c);
		~Server();

		/* I don't know */
		void	respond(int fd, Request &request);

		/* Get */
		std::map<int, std::string>					get_error_pages() const;
		std::vector<std::string>					get_names() const;
		std::vector<Server::s_location>				&get_locations();
		std::string									get_host() const;
		std::string									get_root() const;
		unsigned int								get_port() const;
		unsigned int								get_client_max_body_size() const;
		Server::s_location							&get_location();

		/* Set */
		char	set_error_pages(int, std::string);
		char	set_names(std::string);
		char	set_locations(Server::s_location &);
		char	set_host(std::string);
		char	set_root(std::string);
		char	set_port(unsigned int);
		char	set_client_max_body_size(unsigned int);

		/* Usuful func */
		void	print(void);

		/* I don't know */
		int fd;

	private:
		/* Server config */
		std::map<int, std::string>					_error_pages;
		std::vector<std::string>					_names;
		std::vector<Server::s_location>				_locations;
		std::string									_host;
		std::string									_root;
		unsigned int								_port;
		unsigned int								_client_max_body_size;
		Server::s_location							_location;
};

char	        parse(std::vector<Server*> &server, char *path);

#endif
