#include "SimpleSocket.hpp"

// DEFAULT CONTRUCTOR

SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port,
		u_long interface, int backlog) : backlog(backlog)
{	
	int		optval = 1;

	define_address(domain, port, interface);
	// Creat socket
	this->server_sock = socket(domain, service, protocol);
	test_connection(this->server_sock);
	
	//mark a socket as non-blocking
	setsockopt(this->server_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    if (fcntl(this->server_sock, F_SETFL,  O_NONBLOCK) < 0)
		test_connection(-1);
	this->binding = connect_to_network();
	test_connection(this->binding);


	this->listening = start_listening();
	test_connection(this->listening);
}

void	SimpleSocket::define_address(int domain, int port, u_long interface)
{
	this->address.sin_family = domain;
	this->address.sin_port = htons(port);
	this->address.sin_addr.s_addr = htonl(interface);
}

void	SimpleSocket::test_connection(int item_to_test)
{
	if (item_to_test < 0)
	{
		perror("Fialed to connect...");
		exit(EXIT_FAILURE);
	}
}

int		SimpleSocket::connect_to_network(void)
{
	return (bind(this->server_sock, (struct sockaddr *)&this->address,
				sizeof(this->address)));
}

int		SimpleSocket::start_listening(void)
{
	return(listen(this->server_sock, this->backlog));
}

// Getter functions

struct sockaddr_in SimpleSocket::get_address(void)
{
	return (this->address);
}

int		SimpleSocket::get_server_sock(void)
{
	return (this->server_sock);
}

int		SimpleSocket::get_binding(void)
{
	return(this->binding);
}

// Setter fucntions
void	SimpleSocket::set_address(struct sockaddr_in address)
{
	this->address = address;
	return ;
}

void	SimpleSocket::set_server_sock(int server_sock)
{
	this->server_sock = server_sock;
	return ;
}

void	SimpleSocket::set_binding(int binding)
{
	this->binding = binding;
	return ;
}

