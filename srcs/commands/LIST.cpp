#include "command.hpp"
#include "Server.hpp"
#include "replies.hpp"
#include "utils.hpp"
#include "colors.hpp"

std::string itoa(int i)
{
    std::string str = "";
    if (i == 0)
        return ("0");
    while (i)
    {
        int x = i % 10;
        i /= 10;
        char c = '0';
        c = c + x;
        str = c + str;
    }
    return str;
}

std::string count_users(std::vector<std::string> listNick, User *user)
{
    int count = 0;
    for (std::vector<std::string>::iterator it = listNick.begin(); it != listNick.end(); ++it)
    {
        if (user->getServer()->getUser(*it)->getMode("invisible") == false)
            count++;
    }
    return (itoa(count));
}

void LIST(User *user)
{
    std::map<std::string, Channel *> channelList = user->getServer()->getChannelList();
    if (user->param_list.size() == 0)
    {
        for (std::map<std::string, Channel *>::iterator it = channelList.begin(); it != channelList.end(); ++it)
        {
            if (it->second->isSecret() == true &&  it->second->isHere(user->getNick()) == true)
            {
            std::vector<std::string> listNick = it->second->getNickList();
            if (it->second->getTopic().size() > 0)
                user->getServer()->toSend(RPL_LIST(getArgs(it->first, count_users(listNick, user), it->second->getTopic()), user->getNick()),
                                        user->getFd());
            else
                user->getServer()->toSend(RPL_LIST(getArgs(it->first, count_users(listNick, user), " "), user->getNick()),
                                        user->getFd());
            }
            else if (it->second->isPrivate() == true && it->second->isHere(user->getNick()) == false)
            {
                user->getServer()->toSend(RPL_LIST(getArgs("Prv", "Prv", " "), user->getNick()),
                                        user->getFd());
            }
            else if (it->second->isPrivate() == true && it->second->isHere(user->getNick()) == true)
            {
            std::vector<std::string> listNick = it->second->getNickList();
            if (it->second->getTopic().size() > 0)
                user->getServer()->toSend(RPL_LIST(getArgs(it->first, count_users(listNick, user), it->second->getTopic()), user->getNick()),
                                        user->getFd());
            else
                user->getServer()->toSend(RPL_LIST(getArgs(it->first, count_users(listNick, user), " "), user->getNick()),
                                        user->getFd());
            }
            else if (it->second->isPrivate() == false && it->second->isSecret() == false)
            {
            std::vector<std::string> listNick = it->second->getNickList();
            if (it->second->getTopic().size() > 0)
                user->getServer()->toSend(RPL_LIST(getArgs(it->first, count_users(listNick, user), it->second->getTopic()), user->getNick()),
                                        user->getFd());
            else
                user->getServer()->toSend(RPL_LIST(getArgs(it->first, count_users(listNick, user), " "), user->getNick()),
                                        user->getFd());
            }
        }
    }
    else
    {
        std::vector<std::string> channels = splitStr(user->param_list[0], ",");
        for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            Channel *chan = user->getServer()->getChannel(*it);
            if (chan == NULL)
            {
                user->getServer()->toSend(ERR_NOSUCHCHANNEL(getArgs(*it), user->getNick()),
                                            user->getFd());
                continue;
            }
            if (chan->isSecret() == false)
            {
            std::vector<std::string> listNick = user->getServer()->getChannel(*it)->getNickList();
            if (user->getServer()->getChannel(*it)->getTopic().size() > 0)
                user->getServer()->toSend(RPL_LIST(getArgs(user->getServer()->getChannel(*it)->getName(), count_users(listNick, user), user->getServer()->getChannel(*it)->getTopic()), user->getNick()),
                                        user->getFd());
            else
                user->getServer()->toSend(RPL_LIST(getArgs(user->getServer()->getChannel(*it)->getName(), count_users(listNick, user), " "), user->getNick()),
                                        user->getFd());
            }
        }
    }
    return user->getServer()->toSend(RPL_LISTEND(getArgs(), user->getNick()),
                                       user->getFd());
}
