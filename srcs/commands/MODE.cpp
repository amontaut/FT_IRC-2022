/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:06:42 by ali               #+#    #+#             */
/*   Updated: 2022/11/21 16:51:55 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"
#include "Server.hpp"
#include "colors.hpp"

void    broadcast(User *user, Channel* channel, const std::vector<std::string>& params)
{
    std::vector<std::string> msgParams = params;
    user->getServer()->toSend(getMsg(user, "MODE", msgParams), channel->getFds());
}

void    kickBroadcast(User *user, Channel* channel, const std::vector<std::string>& params)
{
    std::vector<std::string> msgParams = params;
    user->getServer()->toSend(getMsg(user, "KICK", msgParams), channel->getFds());
}

void    setChanMode(User* user, Channel* channel, const std::vector<std::string>& params)
{
    std::string sign = "+";
    bool b = true;
        
    for (unsigned int i = 0; i < params[1].length(); i++)
    {
        if (params[1][i] == 'o' || params[1][i] == 'v' || params[1][i] == 'I' || params[1][i] == 'b')
        {
            if (params.size() < 3)
                return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("MODE"),
                            user->getNick()), user->getFd());
            if (params[1][i] == 'I')
            {
                if (sign == "+")
                {
                    channel->inviteUser(params[2]);
                    user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "i", params[2]),
                        user->getNick()), user->getFd());
                    broadcast(user, channel, getArgs(channel->getName(), sign + "i", params[2]));
                }
                else
                {
                    channel->uninviteUser(params[2]);
                    user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "i", params[2]),
                        user->getNick()), user->getFd());
                    broadcast(user, channel, getArgs(channel->getName(), sign + "i", params[2]));
                }
            }
            if (params[1][i] == 'b')
            {
                if (sign == "+")
                {
                    if (channel->isHere(params[2]))
                    {
                        kickBroadcast(user, channel, getArgs(channel->getName(), params[2]));
                        channel->kickUser(params[2]);
                    }
                    channel->banUser(params[2]);
                    user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "b", params[2]),
                        user->getNick()), user->getFd());
                    broadcast(user, channel, getArgs(channel->getName(), sign + "b", params[2]));
                }
                else
                {
                    channel->unbanUser(params[2]);
                    user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "b", params[2]),
                        user->getNick()), user->getFd());
                    broadcast(user, channel, getArgs(channel->getName(), sign + "b", params[2]));
                }
            }
            else if (channel->isHere(params[2]) == false)
            {
                return user->getServer()->toSend(ERR_USERNOTINCHANNEL(getArgs(params[2],
                                params[0]), user->getNick()), user->getFd());
            }
            if (params[1][i] == 'o')
            {
                channel->setUserOp(params[2], b);
                user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "o", params[2]),
                        user->getNick()), user->getFd());
                broadcast(user, channel, getArgs(channel->getName(), sign + "o", params[2]));
            }
            else if (params[1][i] == 'v')
            {
                channel->setUserVoiced(params[2], b);
                user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "v", params[2]),
                        user->getNick()), user->getFd());
                broadcast(user, channel, getArgs(channel->getName(), sign + "v", params[2]));
            }
        }
        else if (params[1][i] == 'i')
        {
            channel->setChanFlag("inviteOnly", b);
            user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "i", " "),
                    user->getNick()), user->getFd());
            broadcast(user, channel, getArgs(channel->getName(), sign + "i"));
        }
        else if (params[1][i] == 's')
        {
            channel->setChanFlag("secret", b);
            user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "s",  " "),
                    user->getNick()), user->getFd());
            broadcast(user, channel, getArgs(channel->getName(), sign + "s"));
        }
        else if (params[1][i] == 'a')
        {
            channel->setChanFlag("anonymous", b);
            user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "a", " "),
                    user->getNick()), user->getFd());
            broadcast(user, channel, getArgs(channel->getName(), sign + "a"));
        }
        else if (params[1][i] == 'm')
        {
            channel->setChanFlag("moderated", b);
            user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "m", " "),
                    user->getNick()), user->getFd());
            broadcast(user, channel, getArgs(channel->getName(), sign + "m"));
        }
        else if (params[1][i] == 'p')
        {
            channel->setChanFlag("private", b);
            user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "p", " "),
                    user->getNick()), user->getFd());
            broadcast(user, channel, getArgs(channel->getName(), sign + "p"));
        }
        else if (params[1][i] == 't')
        {
            channel->setChanFlag("topic", b);
            user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], sign + "t", " "),
                    user->getNick()), user->getFd());
            broadcast(user, channel, getArgs(channel->getName(), sign + "t"));
        }
        else if (params[1][i] == 'l')
        {
            if (sign == "+")
            {
                if (params.size() < 3)
                    return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("MODE"),
                                user->getNick()), user->getFd());
                std::stringstream ss;
                int limit;
                ss << params[2];
                ss >> limit;
                if (limit != 0)
                {
                    channel->setUserLimit(limit);
                    user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], "+l", params[2]),
                         user->getNick()), user->getFd());
                    broadcast(user, channel, getArgs(channel->getName(), "+l", params[2]));
                }
            }
            else
            {
                channel->setUserLimit(500);
                user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], "-l", " "),
                    user->getNick()), user->getFd());
                broadcast(user, channel, getArgs(channel->getName(), "-l", params[2]));
            }
        }
        else if (params[1][i] == 'k')
        {
            if (sign == "+")
            {
                if (params.size() < 3)
                    return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("MODE"),
                                user->getNick()), user->getFd());
                if (channel->getKey() != "")
                    return user->getServer()->toSend(ERR_KEYSET(getArgs(params[0]),
                                user->getNick()), user->getFd());
                channel->setKey(params[2]);
                user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], "+k", params[2]),
                     user->getNick()), user->getFd());
                broadcast(user, channel, getArgs(channel->getName(), "+k", params[2]));
            }
            else
            {
                channel->setKey("");
                user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], "-k", " "),
                    user->getNick()), user->getFd());
                broadcast(user, channel, getArgs(channel->getName(), "-k", params[2]));
            }
        }
        else if (params[1][i] == '+' || params[1][i] == '-')
        {
            char charsign = params[1][i];
            if (charsign == '+')
            {
                sign = "+";
                b = true;
            }
            else
            {
                sign = "-";
                b = false;
            }
        }
        else
            user->getServer()->toSend(ERR_UNKNOWNMODE(getArgs(convert(params[1][i]), params[0]),
                    user->getNick()), user->getFd());
    }
}

void    chanMode(User* user, std::vector<std::string>& params)
{
    Channel* channel = user->getServer()->getChannel(params[0]);

    if (channel == NULL)
        return user->getServer()->toSend(ERR_NOSUCHCHANNEL(getArgs(params[0]), user->getNick()),
                user->getFd());

    if (channel->isChanOp(user->getNick()) == false)
        return user->getServer()->toSend(ERR_CHANOPRIVSNEEDED(getArgs(params[0]), user->getNick()),
                user->getFd());

    setChanMode(user, channel, params);
}

void    userMode(User* user, std::vector<std::string>& params)
{
    std::string sign = "+";
    bool b = true;

    if (user->getServer()->isHere(params[0]) == false)
        return ;
    for (unsigned int i = 0; i < params[1].length(); i++)
    {
        if (params[1][i] == 'a')
        {
            user->setMode("away", b);
            user->getServer()->toSend(RPL_UMODEIS(getArgs(sign + "a"), user->getNick()),
                    user->getFd());
        }
        else if (params[1][i] == 'i')
        {
            user->setMode("invisible", b);
            user->getServer()->toSend(RPL_UMODEIS(getArgs(sign + "i"), user->getNick()),
                    user->getFd());
        }
        else if (params[1][i] == 'w')
        {
            user->setMode("wallops", b);
            user->getServer()->toSend(RPL_UMODEIS(getArgs(sign + "w"), user->getNick()),
                    user->getFd());
        }
        else if (params[1][i] == 'r')
        {
            user->setMode("restricted", b);
            user->getServer()->toSend(RPL_UMODEIS(getArgs(sign + "r"), user->getNick()),
                    user->getFd());
        }
        else if (params[1][i] == 'o')
        {
            user->setMode("operator", b);
            user->getServer()->toSend(RPL_UMODEIS(getArgs(sign + "o"), user->getNick()),
                    user->getFd());
        }
        else if (params[1][i] == 'O')
        {
            user->setMode("local_op", b);
            user->getServer()->toSend(RPL_UMODEIS(getArgs(sign + "O"), user->getNick()),
                    user->getFd());
        }
        else if (params[1][i] == 's')
        {
            user->setMode("server_notice", b);
            user->getServer()->toSend(RPL_UMODEIS(getArgs(sign + "s"), user->getNick()),
                    user->getFd());
        }
        else if (params[1][i] == '+' || params[1][i] == '-')
        {
            char charsign = params[1][i];
            if (charsign == '+')
            {
                sign = "+";
                b = true;
            }
            else
            {
                sign = "-";
                b = false;
            }
        }
        else
            user->getServer()->toSend(ERR_UMODEUNKNOWNFLAG(getArgs(" "),
                        user->getNick()), user->getFd());
    }
}

void    MODE(User *user)
{
    std::vector<std::string> params = user->param_list;
    if (params.size() == 1 && isChanName(params[0]))
    {
        Channel* chan = user->getServer()->getChannel(params[0]);
        if (chan == NULL)
        {
            return user->getServer()->toSend(ERR_NOSUCHCHANNEL(getArgs(params[0]), user->getNick()),
                user->getFd());
        }
        if (chan->isChanOp(user->getNick()))
        {
            user->getServer()->toSend(RPL_CHANNELMODEIS(getArgs(params[0], "+o", user->getNick()),
                    user->getNick()), user->getFd());
            broadcast(user, chan, getArgs(chan->getName(), "+o", user->getNick()));
        }
    }
    else if (params.size() < 2)
        return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("MODE"), user->getNick()),
                user->getFd());
    else if (isChanName(params[0]))
        chanMode(user, params);
    else
        userMode(user, params);
}
