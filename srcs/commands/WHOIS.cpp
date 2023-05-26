#include "command.hpp"

void send_infos(User *user, std::string nick, std::string username, std::string realname, std::string host,
time_t idle, User *user_info)
{
    user->getServer()->toSend(RPL_WHOISUSER(getArgs(nick, username, host, realname), nick), user->getFd());
    if (user_info->getStatus() == "inactive")
        user->getServer()->toSend(RPL_WHOISIDLE(getArgs(nick, convert(idle)), nick), user->getFd());
    else if (user_info->getStatus() == "away")
        user->getServer()->toSend(RPL_AWAY(getArgs(nick, user_info->getAway()), nick), user->getFd());
}

void    WHOIS(User *user)
{
    std::vector<std::string> user_list = splitStr(user->param_list[0], ",");
    User *user_info = NULL;

    if (user->param_list.size() == 0)
        return (user->getServer()->toSend(ERR_NONICKNAMEGIVEN(getArgs(),user->getNick()),
        user->getFd()));
    else
    {
        for(std::vector<std::string>::iterator it = user_list.begin(); it != user_list.end(); it++)
        {
            user_info = user->getServer()->getUser(*it);

            if (user->getServer()->isHere(*it) == false)
                user->getServer()->toSend(ERR_NOSUCHNICK(getArgs(*it), user->getNick()), user->getFd());
            else
                send_infos(user, *it, user_info->getUsername(), user_info->getRealName(), user_info->getHostname(), 
                user_info->getActivity(), user_info);
        }
    }
    return (user->getServer()->toSend(RPL_ENDOFWHOIS(getArgs(user->getNick()), user->getNick()), user->getFd()));
}
