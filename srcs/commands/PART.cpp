/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:22:03 by ali               #+#    #+#             */
/*   Updated: 2022/11/21 16:23:55 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

void	partChan(std::string& partMsg, User* user, Channel* chan)
{
	if (chan->isHere(user->getNick()) == false)
		return user->getServer()->toSend(ERR_NOTONCHANNEL(getArgs(chan->getName()),
				user->getNick()), user->getFd());
	
	std::vector<std::string> msgParams;
	if (partMsg != "")
		msgParams = getArgs(chan->getName(), partMsg);
	else
		msgParams = getArgs(chan->getName(), user->getNick());
	user->getServer()->toSend(getMsg(user, "PART", msgParams), chan->getFds());
	chan->kickUser(user->getNick());
	if (chan->getUserNum() == 0)
		user->getServer()->deleteChannel(chan->getName());
}

void	PART(User* user)
{
	std::vector<std::string> params = user->param_list;
	std::string partMsg = "";

	if (params.size() < 1)
		return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("PART"),
					user->getNick()), user->getFd());
	if (params.size() > 1)
	{
		std::vector<std::string> msg(params.begin() + 1, params.end());
		partMsg = getStr(msg);
	}

	std::vector<std::string> chanNames = splitStr(params[0], ",");
	Channel* chan;
	for (std::vector<std::string>::iterator it = chanNames.begin(); it != chanNames.end(); it++)
	{
		chan = user->getServer()->getChannel(*it);
		if (chan == NULL)
			user->getServer()->toSend(ERR_NOSUCHCHANNEL(getArgs(*it),
					user->getNick()), user->getFd());
		else
			partChan(partMsg, user, chan);
	}
}
