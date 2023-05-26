#include "Server.hpp"
#include "colors.hpp"

/*
** ------------------------------- CONSTRUCTOR/DESTRUCTOR --------------------------------
*/

Server::Server(const std::string& given_port, const std::string& pword) : _port(given_port), _password(pword)
{
	_address = "127.0.0.1";
	_serverName = "OurSuperServer";
}

Server::~Server() {}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*vvvvvvvvvvvvvvvvvvvv Socket listener creation and adding socket to the server's list vvvvvvvvvvvvvvv*/
void Server::setListenerSock(void)
{
	int listener;
	int yes = 1;
	int status;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, _port.c_str(), &hints, &servinfo)) != 0)
	{
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		exit(EXIT_FAILURE);
	}

	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		listener = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
		if (listener < 0)
		{
			std::cerr << "socket error" << std::endl;
			continue;
		}
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		fcntl(listener, F_SETFL, O_NONBLOCK);

		if ((bind(listener, servinfo->ai_addr, servinfo->ai_addrlen)) < 0)
		{
			std::cerr << "bind error" << std::endl;
			close(listener);
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo);

	if (p == NULL)
	{
		std::cerr << "Server: failed to bind" << std::endl;
		exit(1);
	}

	if (listen(listener, 5) == -1)
	{
		std::cerr << "listen" << std::endl;
		exit(1);
	}

	_listener = listener;
	addSocketToList(listener, POLLIN, 0);
}

void Server::addSocketToList(int filed, short ev, short rev)
{
	struct pollfd tmp;

	tmp.fd = filed;
	tmp.events = ev;
	tmp.revents = rev;

	_pfds.push_back(tmp);
}

void Server::addChannel(const std::string& new_channel, Channel * chan)
{
	_channels.insert(std::pair<std::string, Channel*>(new_channel, chan));
}
/*	poll() function uses array and i wanted to work with container
	so i made two function to go to one from another.
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
void Server::pollLoop()
{
	while (1)
	{
		if (g_end == 1)
		{
			cleanServer();
			exit(0);
		}
		polling();
		handlePfds();
		checkActivity();
	}
}

void Server::listToArr()
{
	_arr_pfds = (pollfd *)malloc(sizeof(struct pollfd) * sizeof(_pfds.size()));
	std::copy(_pfds.begin(), _pfds.end(), _arr_pfds);
}

void Server::arrToList()
{
	std::copy(_arr_pfds, _arr_pfds + _pfds.size(), _pfds.begin());
}

void Server::polling()
{
	listToArr();
	poll(_arr_pfds, _pfds.size(), -1);
	arrToList();
	free(_arr_pfds);
}

void Server::checkActivity(void)
{
	std::map<int, User *>::iterator it;

	for (it = _user_list.begin(); it != _user_list.end(); it++)
	{
		if (it->second->getActivity() > 180)
			it->second->setStatus("inactive");
	}
	for (it = _user_list.begin(); it != _user_list.end(); it++)
	{
		if (it->second->getStatus() == "out")
		{
			deleteUser(it->second->getNick());
			break ;
		}
	}
}

bool	Server::isHere(const std::string& nick)
{
	for (std::map<int, User*>::iterator it = _user_list.begin(); it != _user_list.end();
			it++)
	{
		if (it->second->getNick() == nick)
			return true;
	}
	return false;
}

/*  This function will check the results of poll() by doing a bitwise AND on the returned event.
	If its the listener that has something to say, that means we have a new connection.
	Otherwise, we have data to read with recv().
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
void Server::handlePfds()
{
	std::list<pollfd>::iterator it;
	std::list<pollfd>::iterator itend;

	itend = _pfds.end();
	for (it = _pfds.begin(); it != itend; it++)
	{
		if (it->revents & POLLIN)
		{
			if (it->fd == _listener)
				handleNewConnection();
			else
			{
				int nbytes = recv(it->fd, _buf, sizeof(_buf), 0);
				int sender_fd = it->fd;
				if (nbytes <= 0)
				{
					std::cout << "nbytes = " << nbytes << std::endl;
					closeConnection(sender_fd, nbytes);
					_pfds.erase(it++);
				}
				else
				{
					_buf[nbytes] = '\0';
					std::cout << BBLUE << "< received " << nbytes << " bytes (" << _buf << ")" << E << "\n";
					handleRaw(sender_fd);
				}
				// std::cout << message << "\n";
				std::cout << "------------------------------\n";
			}
		}
	}
}

/*	This function will accept a new connection, add it to the server's fd list,
	alloc a new User and add it to the server's User map.
vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
void Server::handleNewConnection()
{
	struct sockaddr_storage remote_addr;
	socklen_t addr_size = sizeof(remote_addr);
	int new_fd = accept(_listener, (struct sockaddr *)&remote_addr, &addr_size);

	fcntl(new_fd, F_SETFL, O_NONBLOCK);
	addSocketToList(new_fd, POLLIN, 0);
	_user_list[new_fd]= new User(new_fd, this);
	_user_list[new_fd]->setStatus("entering");
	std::cout << "pollserver: new connection\n";
}

void Server::closeConnection(int sender_fd, int nbytes)
{
	#ifdef DEBUG
		char errstr[256];
		if (nbytes == 0)
			std::cout << "socket " << sender_fd << " hang up\n";
		else
		{
			std::cerr << "recv: " << errno << " " << strerror_r(errno, errstr, 256) << "\n";
		}
	#endif
	close(sender_fd);
}

void Server::handleRaw(int sender_fd)
{
	std::string tmp; 

	tmp.append(_user_list[sender_fd]->message);
	tmp.append(_buf);

	std::size_t pos = tmp.find("\r\n");
	while ((pos = tmp.find("\r\n")) != std::string::npos)
	{
		if (pos != 0)
			_user_list[sender_fd]->toCommand(tmp.substr(0, pos));
		if (_user_list[sender_fd]->getStatus() == "out")
			return ;
		tmp = tmp.substr(pos + 2);
	}
	this->_user_list[sender_fd]->message = tmp;
	memset(this->_buf, 0, 512);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
std::string				Server::getAddress(void) { return (_address); }

std::string				Server::getServerName(void) { return (_serverName); }

std::string				Server::getPassword() { return (_password); }

std::map<int, User *>	Server::getUserList() { return (_user_list); }
std::map<std::string, Channel*> Server::getChannelList() { return (_channels); }

Channel*				Server::getChannel(const std::string& name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

User*					Server::getUser(const std::string& nick)
{
	for (std::map<int, User*>::iterator it = _user_list.begin(); it != _user_list.end();
			it++)
	{
		if (it->second->getNick() == nick)
			return it->second;
	}
	return NULL;
}

void	Server::deleteChannel(const std::string& name)
{
	Channel* to_erase = _channels[name];
	_channels.erase(name);
	delete to_erase;
}

void	Server::deleteUser(const std::string& nick)
{
	int fd;

	for (std::map<int, User*>::iterator it = _user_list.begin(); it != _user_list.end();
			it++)
	{
		if (it->second->getNick() == nick)
		{
			User* to_erase = it->second;
			fd = it->second->getFd();
			close(fd);
			_user_list.erase(it);
			delete to_erase;
			for (std::list<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
			{
				if (it->fd == fd)
				{
					_pfds.erase(it);
					return ;
				}
			}
		}
	}
}

void	Server::cleanServer()
{
	std::vector<int>	userFds;
	std::vector<std::string>	chanNames;

	for (std::map<int, User*>::iterator it = _user_list.begin(); it != _user_list.end();
			it++)
		userFds.push_back(it->first);
	for (std::vector<int>::iterator it = userFds.begin(); it != userFds.end(); it++)
	{
		User* to_erase = _user_list[*it];
		_user_list.erase(*it);
		delete to_erase;
	}

	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
		chanNames.push_back(it->first);
	for(std::vector<std::string>::iterator it = chanNames.begin(); it != chanNames.end(); it++)
	{
		Channel* to_erase = _channels[*it];
		_channels.erase(*it);
		delete to_erase;
	}
	_pfds.clear();
}
/* ************************************************************************** */
