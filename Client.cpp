#include "Client.hpp"

Client::Client()
{
}

Client::Client(int fd, Server *server) : _fd(fd), _server(server)
{
}

Client::Client(const Client &c) : _fd(c._fd), _server(c._server)
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


int		Client::get_fd(void) const
{
	return _fd;
}

void	Client::wait_response(void)
{
	_server->respond(_fd, _request);
}


void 	Client::receive_request(void)
{
	char buffer[100001];
	int ret;
	std::string request;
	unsigned long bytes;

	bytes = 0;
	while ((ret = recv(_fd, buffer, 100000, 0)) == 100000)
	{
		buffer[ret] = 0;
		request += buffer;
		bytes += ret;
	}
	_request.parse(request);
}