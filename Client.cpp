#include "Client.hpp"

Client::Client()
{
}

Client::Client(const Client &c)
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