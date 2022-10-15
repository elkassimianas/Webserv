#include "server/Server.hpp"

int main()
{
	Server test(AF_INET, SOCK_STREAM, 0, 18000, INADDR_ANY, 10);
	return (0);
}
