#include "command.hpp"
#include "Server.hpp"

void	chanMsg(Channel* chan, User* user, std::vector<std::string>& params)
{
	if (chan->canSpeak(user->getNick()) == false)
		return user->getServer()->toSend(ERR_CANNOTSENDTOCHAN(getArgs(params[0]), user->getNick()), 
				user->getFd());

	user->getServer()->toSend(getMsg(user, "PRIVMSG", params), chan->getOtherFds(user->getNick()));
}

void	userMsg(User* recipient, User* user, std::vector<std::string>& params)
{ 
	if (recipient->getStatus() == "away")
	{
		user->getServer()->toSend(RPL_AWAY(getArgs(recipient->getNick(), recipient->getAway()),
					user->getNick()), user->getFd());
	}
	else
		user->getServer()->toSend(getMsg(user, "PRIVMSG", params), recipient->getFd());
}

void	PRIVMSG(User* user)
{
	std::vector<std::string> params = user->param_list;

	if (params.size() == 0)
			return user->getServer()->toSend(ERR_NORECIPIENT(getArgs("PRIVMSG"),
						user->getNick()), user->getFd());
	else if (params.size() < 2)
		return user->getServer()->toSend(ERR_NOTEXTTOSEND(getArgs(), user->getNick()),
				user->getFd());
	if (isChanName(params[0]))
	{
		Channel* channel = user->getServer()->getChannel(params[0]);
		if (channel == NULL)
			return user->getServer()->toSend(ERR_NORECIPIENT(getArgs("PRIVMSG"),
						user->getNick()), user->getFd());
		else
			chanMsg(channel, user, params);
	}
	else
	{
		User* recipient = user->getServer()->getUser(params[0]);
		if (recipient == NULL)
			return user->getServer()->toSend(ERR_NOSUCHNICK(getArgs(params[0]),
						user->getNick()), user->getFd());
		else
			userMsg(recipient, user, params);
	}
}

