#include "command.hpp"
#include "Server.hpp"

void	chanNotMsg(Channel* chan, User* user, std::vector<std::string>& params)
{
	if (chan->canSpeak(user->getNick()) == false)
		return user->getServer()->toSend(ERR_CANNOTSENDTOCHAN(getArgs(params[0]), user->getNick()), 
				user->getFd());

	user->getServer()->toSend(getMsg(user, "NOTICE", params), chan->getOtherFds(user->getNick()));
}

void	userNotMsg(User* recipient, User* user, std::vector<std::string>& params)
{ 
	user->getServer()->toSend(getMsg(user, "NOTICE", params), recipient->getFd());
}

void	NOTICE(User* user)
{
	std::vector<std::string> params = user->param_list;

	if (params.size() == 0)
			return user->getServer()->toSend(ERR_NORECIPIENT(getArgs("NOTICE"),
						user->getNick()), user->getFd());
	else if (params.size() < 2)
		return user->getServer()->toSend(ERR_NOTEXTTOSEND(getArgs(), user->getNick()),
				user->getFd());
	if (isChanName(params[0]))
	{
		Channel* channel = user->getServer()->getChannel(params[0]);
		if (channel == NULL)
			return user->getServer()->toSend(ERR_NORECIPIENT(getArgs("NOTICE"),
						user->getNick()), user->getFd());
		else
			chanNotMsg(channel, user, params);
	}
	else
	{
		User* recipient = user->getServer()->getUser(params[0]);
		if (recipient == NULL)
			return user->getServer()->toSend(ERR_NOSUCHNICK(getArgs(params[0]),
						user->getNick()), user->getFd());
		else
			userNotMsg(recipient, user, params);
	}
}

