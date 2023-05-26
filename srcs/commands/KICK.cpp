/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:42:08 by ali               #+#    #+#             */
/*   Updated: 2022/11/21 10:33:21 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

void	kickUser(Channel* chan, User* user, std::string& nick, std::string& kickMsg)
{
	if (chan->isHere(user->getNick()) == false)
		return user->getServer()->toSend(ERR_NOTONCHANNEL(getArgs(chan->getName()),
				user->getNick()), user->getFd());

	if (chan->isChanOp(user->getNick()) == false)
		return user->getServer()->toSend(ERR_CHANOPRIVSNEEDED(getArgs(chan->getName()),
					user->getNick()), user->getFd());

	if (chan->isHere(nick) == false)
		return user->getServer()->toSend(ERR_USERNOTINCHANNEL(getArgs(nick, chan->getName()),
					user->getNick()), user->getFd());

	std::vector<std::string> msgParams;
	if (kickMsg != "")
		msgParams = getArgs(chan->getName(), nick, kickMsg);
	else
		msgParams = getArgs(chan->getName(), nick, user->getNick());
	user->getServer()->toSend(getMsg(user, "KICK", msgParams), chan->getFds());
	chan->kickUser(nick);
	if (chan->getUserNum() == 0)
		user->getServer()->deleteChannel(chan->getName());
}

void	KICK(User* user)
{
	std::vector<std::string> params = user->param_list;
	std::string	kickMsg = "";

	if (params.size() < 2)
		return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("KICK"),
					user->getNick()), user->getFd());
	if (params.size() > 2)
	{
		std::vector<std::string> msg(params.begin() + 2, params.end());
		kickMsg = getStr(msg);
	}

	std::vector<std::string> chanNames = splitStr(params[0], ",");
	std::vector<std::string> userNicks = splitStr(params[1], ",");

	if (chanNames.size() != 1 && chanNames.size() != userNicks.size())
		return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("KICK"),
					user->getNick()), user->getFd());

	Channel* chan;
	for (unsigned int i = 0; i < chanNames.size(); i++)
	{
		chan = user->getServer()->getChannel(chanNames[i]);
		if (chan == NULL)
		{
			user->getServer()->toSend(ERR_NOSUCHCHANNEL(getArgs(chanNames[i]),
					user->getNick()), user->getFd());
			continue ;
		}
		if (chanNames.size() == 1)
		{
			for (std::vector<std::string>::iterator it = userNicks.begin(); it != userNicks.end();
					it++)
				kickUser(chan, user, *it, kickMsg);
		}
		else
			kickUser(chan, user, userNicks[i], kickMsg);
	}
}

