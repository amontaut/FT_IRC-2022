/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:42:08 by ali               #+#    #+#             */
/*   Updated: 2022/11/21 15:18:07 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "colors.hpp"

void TOPIC(User *user)
{
	if (user->param_list.size() < 2)
		return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("TOPIC"), user->getNick()),
				user->getFd());
	std::string channel = user->param_list[0];
	std::string newtopic = user->param_list[1];
		if (user->param_list[1][0] == ':')
           newtopic = user->param_list[1].substr(1, user->param_list[1].size());
	if (newtopic.length() > 80)
	{
		return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("TOPIC"), user->getNick()),
				user->getFd());
	}

	Channel* chan = user->getServer()->getChannel(channel);
	if (chan == NULL)
	{
		return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("TOPIC"), user->getNick()),
				user->getFd());
	}

	if (chan->isHere(user->getNick()) == false)
	{
		return user->getServer()->toSend(ERR_NOTONCHANNEL(getArgs(channel), user->getNick()),
				user->getFd());
	}

	if ((chan->isTopicOperatorOnly() && chan->isChanOp(user->getNick()) == 1)
			|| chan->isTopicOperatorOnly() == false)
	{
		chan->setTopic(newtopic);
		user->getServer()->toSend(RPL_TOPIC(getArgs(channel, newtopic), user->getNick()), chan->getFds());
	}
	else
	{
		user->getServer()->toSend(ERR_CHANOPRIVSNEEDED(getArgs(channel), user->getNick()), user->getFd());
	}
}
