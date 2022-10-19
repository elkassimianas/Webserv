#include "Server.hpp"

Server::Server(int domain, int service, int protocol, int port,
		u_long interface, int backlog)
{
	creatSocket(domain, service, protocol, port, interface, backlog);
	initialize_current_sockets();
	launch();
}

void	Server::creatSocket(int domain, int service, int protocol,
		int port, u_long interface, int backlog)
{
	this->socket = new SimpleSocket(domain, service, protocol, port,
			interface, backlog);
	this->server_socket = this->socket->get_server_sock();
}

void	Server::initialize_current_sockets(void)
{
	FD_ZERO(&this->current_sockets);
	FD_ZERO(&this->second_sockets);
	FD_SET(this->server_socket, &this->current_sockets);
}

bool	Server::check_header(std::string &request)
{
	size_t pos;

	if((pos = request.find("\r\n\r\n")) == std::string::npos)
		return (false);
    request.replace(pos, pos,1, '$');
	return (true);
}

void	Server::readSocket(int index)
{
	char			buf[1025];
	char			*body = NULL;
	ParsingRequest	objRequest = this->map_Clients[vClient_socket[index]];
	std::string		request = objRequest.get_request();
	bzero(&buf, 1025);
	if (recv(vClient_socket[index], &buf, 1024, 0) < 0)
	{
		std::cout << "recv error" << std::endl;
		exit(EXIT_FAILURE);
	}
	request += buf;
	if (check_header(request))
	{
		objRequest.set_request(strtok(&request[0], "$"));
		body = strtok(NULL, "$");
		objRequest.start_parsing();
		if (body != NULL)
		{
			std::cout << "inside body" << std::endl;
		}
	}
	bzero(&buf, 1025);
}

void	Server::selecter(void)
{
	// because select is destructive
	this->read_sockets = this->current_sockets;
	this->write_sockets = this->second_sockets;
	int maxFd;

	maxFd = (this->vClient_socket.empty()) ? this->server_socket: this->vClient_socket.back();
	maxFd++;

	if (select(maxFd, &this->read_sockets, &this->write_sockets, NULL, NULL) < 0)
	{
		perror("select error");
		exit(EXIT_FAILURE);
	}
}

void	Server::accepter(void)
{
	struct sockaddr_in	address = this->socket->get_address();
	int			addrlen = sizeof(this->socket->get_address());
	
	if (FD_ISSET(this->server_socket, &this->read_sockets))
	{
		ParsingRequest		objClient;
		this->vClient_socket.push_back(accept(this->server_socket,
		(struct sockaddr *)&address, (socklen_t *)&addrlen));
		map_Clients.insert(std::pair<int, ParsingRequest>(this->vClient_socket.back(), objClient));
		FD_SET(this->vClient_socket.back(), &this->current_sockets);
	}
}

void	Server::responder(int index)
{
	std::string rep = "HTTP/1.1  200 OK\r\nContent-Type: text/html\r\nContent-Length: 37\r\nConnection: close\r\n\r\n<html><body><h2>ok</h2></body></html>";
	if (send(vClient_socket[index], rep.c_str(), rep.length(), 0) < 0)
	{
		std::cout << "send error" << std::endl;
		exit(EXIT_FAILURE);
	}
	close(vClient_socket[index]);
	FD_CLR(vClient_socket[index], &this->second_sockets);
	FD_CLR(vClient_socket[index], &this->current_sockets);
	this->map_Clients.erase(this->vClient_socket[index]);
	vClient_socket.erase(vClient_socket.begin() + index);
}

void	Server::launch(void)
{
	while(true)
	{
		//std::cout << "============ WAITING 	=============" << std::endl;
		selecter();
		accepter();
		for(size_t i = 0; i < vClient_socket.size(); i++)
		{
			if (FD_ISSET(vClient_socket[i], &this->read_sockets))
			{
				readSocket(i);
				FD_CLR(vClient_socket[i], &this->current_sockets);
				FD_SET(vClient_socket[i], &this->second_sockets);
			}
			else if (FD_ISSET(vClient_socket[i], &this->write_sockets))
			{
				responder(i);
			}
		}
	//	std::cout << "============  DONE 	=============" << std::endl;
	}
}