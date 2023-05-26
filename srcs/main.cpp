#include "Server.hpp"

#define PORT "6667"

int g_end;

void	sigHandler(int sig)
{
	(void) sig;
	std::cout << "Signal received! The server cleans itself" << std::endl;
	g_end = 1;
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Not enough argument: <port> <password>\n";
		return (1);
	}
	for (unsigned int i = 0; i < strlen(av[1]); i++)
	{
		if (isdigit(av[1][i]) == 0)
		{
			std::cerr << "Wrong port argument\n";
			return (1);
		}
	}
	g_end = 0;
	signal(SIGTERM, sigHandler);
	signal(SIGINT, sigHandler);
	
    Server  irc_serv(av[1], av[2]);

    irc_serv.setListenerSock();
    irc_serv.pollLoop();
	return (0);
}
