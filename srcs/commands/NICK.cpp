#include "command.hpp"
#include "colors.hpp"
#include "User.hpp"
#include "Server.hpp"
#include "replies.hpp"

int specialcharNick(char c)
{
    if (c == '-' ||
        c == '[' ||
        c == ']' ||
        c == '\\' ||
        c == '`' ||
        c == '_' ||
        c == '^' ||
        c == '{' ||
        c == '|' ||
        c == '}')
        return (1);
    return (0);
}

int checkForbidenCharNick(std::string nick)
{
    if (nick != "")
    {
        unsigned int i = 0;
        if (nick.length() != 1 && nick[i] == ':') // tbc
        {
            i++;
        }
        if (nick.length() == 1 && isalpha(nick[i]) == 0 && specialcharNick(nick[i]) == 0)
            return (-1);
        while (i < nick.length() - 1)
        {
            if ((isalpha(nick[i]) == 0 && specialcharNick(nick[i]) == 0) || (nick.length() == 1 && nick[i] == ':'))
                return (-1);
            i++;
        }
        if (isalnum(nick[i]) == 0 && specialcharNick(nick[i]) == 0)
            return (-1);
        return (0);
    }
    return (0);
}

void NICK(User *user)
{
    if (user->param_list[0][0] == ':') // tbc
        user->param_list[0].erase(0, 1);
    std::string nickname = user->param_list[0].substr(0, 9);
    if (user->param_list.size() == 0)                                    // quand on passe NICK dans irssi sans param ca fait rien a part redire le nick
        return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("JOIN"), user->getNick()),
				user->getFd());
    if (checkForbidenCharNick(nickname) == -1)
            return (user->getServer()->toSend(ERR_ERRONEUSNICKNAME(user->param_list, nickname), user->getFd()));
    std::map<int, User *> users = user->getServer()->getUserList();
    for (std::map<int, User *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if (nickname == it->second->getNick())
            return (user->getServer()->toSend(ERR_NICKNAMEINUSE(user->param_list, nickname), user->getFd()));
    }
	
	std::map<std::string, Channel*> chanList = user->getServer()->getChannelList();
	for (std::map<std::string, Channel*>::iterator it = chanList.begin(); it != chanList.end(); it++)
	{
		it->second->updateUser(user->getNick(), nickname);
		user->getServer()->toSend(getMsg(user, "NICK", nickname), it->second->getFds());
	}
    user->setNick(nickname);
}
