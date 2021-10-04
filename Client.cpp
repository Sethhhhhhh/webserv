#include "Client.hpp"

Client::Client()
{
}

Client::Client(const Client &c) : _buffer(c._buffer), _fd(c._fd), _server(c._server)
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