#ifndef SERVER_HPP
#define SERVER_HPP

#include "std.hpp"
#include "User.hpp"
#include "Channel.hpp"

#define PORT "6667"
#define DEBUG

class User;
class Channel;

extern int g_end;

class Server
{

public:
	Server(const std::string& arg_port, const std::string& pword);
	~Server(void);

	void setListenerSock(void); // set le socket listener
	void addSocketToList(int filed, short ev, short rev);
	void addChannel(const std::string& new_channel);
	void addNick(const std::string& new_nick);
	void removeNick(const std::string& old_nick);
	void deleteUser(const std::string& nick);
	void deleteChannel(const std::string& name);
	void addChannel(const std::string& new_channel, Channel *chan);
	void handlePfds(void);
	void handleNewConnection(void);
	void closeConnection(int sender_fd, int nbytes);
	void handleRaw(int sender_fd);

	void arrToList(void);
	void listToArr(void);
	void pollLoop(void);
	void polling(void);

	void toSend(const std::string msg, std::vector<int> sockfds);
	void toSend(const std::string msg, int sockfd);

	void checkActivity(void);
	bool isHere(const std::string &nick); // checks if a User is present in the server

	/* accessors */
	std::string getAddress(void);
	std::string getPassword(void);
	std::string getServerName(void);
	std::map<int, User *> getUserList(void);
	Channel *getChannel(const std::string &name);
	User *getUser(const std::string &nick);
	std::map<std::string, Channel *> getChannelList(void);

	void	cleanServer();

private:
	std::string _message;
	std::string _port;
	std::string _password;
	std::string _address;
	std::string _serverName;

	std::list<pollfd> _pfds;
	struct pollfd *_arr_pfds;

	std::map<int, User *> _user_list;
	std::map<std::string, Channel *> _channels;

	char _buf[512];
	int _listener;
};

std::ostream &operator<<(std::ostream &o, Server const &i);

#endif
