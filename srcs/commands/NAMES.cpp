/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NAMES.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 11:02:35 by amontaut          #+#    #+#             */
/*   Updated: 2022/11/21 11:30:53 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "colors.hpp"

void nameReply_names(User *user, Channel *chan)
{
	std::string reply_channel = " ";
	std::string reply_nick = " ";

	if (chan->isPrivate() == true && chan->isHere(user->getNick()) == true)
		reply_channel = "* " + chan->getName();
	else if (chan->isSecret() == true && chan->isHere(user->getNick()) == true)
		reply_channel = "@ " + chan->getName();
	else if (chan->isAnonymous() == true && chan->isHere(user->getNick()) == true)
		reply_channel = chan->getName();
	else if (chan->isAnonymous() == false && chan->isSecret() == false && chan->isPrivate() == false)
		reply_channel = "= " + chan->getName();
	else
		return;

	std::vector<std::string> nick_list = chan->getNickList();
	for (std::vector<std::string>::iterator it = nick_list.begin(); it != nick_list.end(); it++)
	{
		if (chan->isChanOp(*it))
			reply_nick += "@" + *it + " ";
		else if (chan->isModerated() && chan->canSpeak(*it))
			reply_nick += "+" + *it  + " ";
		else
			reply_nick += " " + *it  + " ";
	}
	user->getServer()->toSend(RPL_NAMEREPLY(getArgs(reply_channel, reply_nick),
											user->getNick()),
							  user->getFd());
	user->getServer()->toSend(RPL_ENDOFNAMES(getArgs(reply_channel), user->getNick()), user->getFd());
}

void NAMES(User *user)
{
	std::map<std::string, Channel *> channelList = user->getServer()->getChannelList();

	if (user->param_list.size() == 0)
	{
		std::map<std::string, Channel *> channelsList = user->getServer()->getChannelList();
		for (std::map<std::string, Channel *>::iterator it = channelsList.begin(); it != channelsList.end(); ++it)
			nameReply_names(user, it->second);
	}
	else
	{
        std::vector<std::string> channels = splitStr(user->param_list[0], ",");
        for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			Channel *chan = user->getServer()->getChannel(*it);
			if (chan == NULL)
				return (user->getServer()->toSend(RPL_ENDOFNAMES(getArgs(*it), user->getNick()), user->getFd()));
			if ((chan->isHere(user->getNick()) == true && chan->isPrivate() == true) || (chan->isHere(user->getNick()) == true && chan->isSecret() == true))
				nameReply_names(user, chan);
			if (chan->isSecret() == false && chan->isPrivate() == false && chan->isAnonymous() == false)
				nameReply_names(user, chan);
		}
	}
}
