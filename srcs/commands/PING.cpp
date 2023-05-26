#include "command.hpp"
#include "User.hpp"
#include "Server.hpp"

void    PING(User *user)
{
    std::vector<std::string> params = user->param_list;

    if (params.size() == 0)
        return(user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs(), user->getNick()), user->getFd()));
    
    if (params[0] != user->getServer()->getServerName())
        return(user->getServer()->toSend(ERR_NOORIGIN(getArgs(user->getServer()->getAddress()), user->getNick()), user->getFd()));

    return(user->getServer()->toSend("PONG " + params[0] + "\r\n", user->getFd()));
}
