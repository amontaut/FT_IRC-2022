#include "command.hpp"
#include "Server.hpp"
#include "replies.hpp"
#include "utils.hpp"

void AWAY(User *user)
{
    if (user->param_list[0].size() > 0  && user->param_list.size() > 0)
    {
        user->setMode("away", true);
         user->setStatus("away");

        if (user->param_list[0][0] == ':')
            user->param_list[0] = user->param_list[0].substr(1, user->param_list[0].size());
        user->setAway(getStr(user->param_list));
        user->getServer()->toSend(RPL_NOWAWAY(getArgs(""),
                                                user->getNick()),
                                    user->getFd());
		user->getServer()->toSend(RPL_UMODEIS(getArgs("+a"), user->getNick()),
					user->getFd());
    }
    else
    {
        user->setMode("away", false);
        user->setStatus("");
        user->setAway("");
        user->getServer()->toSend(RPL_UNAWAY(getArgs(""),
                                               user->getNick()),
                                    user->getFd());
		user->getServer()->toSend(RPL_UMODEIS(getArgs("-a"), user->getNick()),
					user->getFd());
    }
}
