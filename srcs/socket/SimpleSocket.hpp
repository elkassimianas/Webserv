#ifndef SIMPLESOCKET_HPP
#define SIMPLESOCKET_HPP

#include <cstdio>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class SimpleSocket
{
	private:
		struct sockaddr_in	address;
		int 			server_sock;
		int 			binding;
		int			listening;
		int			backlog;

	public:
		SimpleSocket(int domain, int service, int protocol, int port,
				u_long interface, int backlog);

		void 		test_connection(int);
		int		connect_to_network(void);
		int		start_listening(void);
		void		define_address(int domain, int port,
					u_long interface);

		// getter functions
		struct sockaddr_in	get_address();
		int 			get_server_sock();
		int 			get_binding();

		//Setter fucntions
		void			set_address(struct sockaddr_in address);
		void			set_server_sock(int server_sock);
		void			set_binding(int binding);
};

#endif
