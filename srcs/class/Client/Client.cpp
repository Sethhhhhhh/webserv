#include "Client.hpp"

Client::Client()
{
}

Client::Client(int fd, Server *server) : _fd(fd), _server(server), _ready_request(false)
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
	char buffer[2024];
	int ret;

	ret = recv(_fd, buffer, 2024, 0);
	buffer[ret] = 0;
	_received_request += buffer;
	_bytes_request += ret;
	std::cout << "Raw Request:\n" << _received_request << std::endl;
	if (buffer[ret-1] == '\n')
	{
		_ready_request = true;
		_request.parse(_received_request, _server->get_config());
	}
}

bool		Client::request_is_ready(void)
{
	return _ready_request;
}