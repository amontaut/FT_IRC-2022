#include "command.hpp"

void	QUIT(User* user)
{
	Server*	serv = user->getServer();
	std::vector<std::string> chanList = user->getChannelList();

	for (std::vector<std::string>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		serv->toSend(getMsg(user, "QUIT", user->getNick()), serv->getChannel(*it)->getOtherFds(user->getNick()));
		serv->getChannel(*it)->kickUser(user->getNick());
		if (serv->getChannel(*it)->getUserNum() == 0)
			serv->deleteChannel(*it);
	}
	user->setStatus("out");
}
