#include "command.hpp"

void	WHO(User* user)
{
	if (user->param_list.size() == 0)
		return ;
	Channel* chan = user->getServer()->getChannel(user->param_list[0]);
	if (chan == NULL)
		return ;
	
	std::string status;
	std::vector<User*> userList = chan->getUserList();
	for (std::vector<User*>::iterator it = userList.begin();
			it != userList.end(); it++)
	{
		if ((*it)->getMode("away") == true)
			status = "G";
		else
			status = "H";
		if (chan->isChanOp((*it)->getNick()))
			status += "@";
		user->getServer()->toSend(RPL_WHOREPLY(getArgs(chan->getName(), (*it)->getUsername(),
						(*it)->getHostname(), (*it)->getNick(), "0", status, (*it)->getRealName()),
					user->getNick()), user->getFd());
	}
	user->getServer()->toSend(RPL_ENDOFWHO(getArgs(chan->getName()), user->getNick()), user->getFd());
}
