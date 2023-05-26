#include "command.hpp"
#include "colors.hpp"

void INVITE(User *user)
{
    if (user->param_list.size() < 2)
    {
		return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("INVITE"), user->getNick()),
				user->getFd());
    }

	Channel* chan = NULL;
	User* invited = NULL;

	std::string chanName = user->param_list[1];
	std::string invitedName = user->param_list[0];
	chan = user->getServer()->getChannel(user->param_list[1]);
	invited = user->getServer()->getUser(user->param_list[0]);

	if (chan == NULL)
	{
		return user->getServer()->toSend(ERR_NOSUCHCHANNEL(getArgs(chanName), user->getNick()),
				user->getFd());
	}
	if (invited == NULL)
	{
		return user->getServer()->toSend(ERR_NOSUCHNICK(getArgs(invitedName), user->getNick()),
				user->getFd());
	}
	if (chan->isHere(user->getNick()) == false)
	{
		return user->getServer()->toSend(ERR_NOTONCHANNEL(getArgs(chan->getName()), user->getNick()),
				user->getFd());
	}
	if (chan->isHere(invited->getNick()))
	{
		return user->getServer()->toSend(ERR_USERONCHANNEL(getArgs(invited->getNick(), chan->getName()),
					user->getNick()), user->getFd());
	}
	if (chan->isChanOp(user->getNick()) == false)
	{
		return user->getServer()->toSend(ERR_CHANOPRIVSNEEDED(getArgs(chan->getName()),
					user->getNick()), user->getFd());
	}
	if (invited->getStatus() == "away")
		return user->getServer()->toSend(RPL_AWAY(getArgs(invited->getNick(), invited->getAway()), user->getNick()), user->getFd());

	std::vector<int> fds;
	fds.push_back(user->getFd());
	fds.push_back(invited->getFd());
	chan->inviteUser(invited->getNick());
	user->getServer()->toSend(RPL_INVITING(getArgs(chan->getName(), invited->getNick()), user->getNick()),
		fds);
}
