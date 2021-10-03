#include "Client.hpp"

Client::Client() : server(defaultServer)
{
}

Client::Client(const Client &c) : server(defaultServer)
{
}

Client& Client::operator=(const Client &c)
{
	return *this;
}

Client::~Client()
{

}