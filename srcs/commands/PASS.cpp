#include "command.hpp"
#include "Server.hpp"
#include "colors.hpp"

void    PASS(User *user)
{
    if (user->getStatus() == "registered" || user->getStatus() == "pending")
        return (user->getServer()->toSend(
            ERR_ALREADYREGISTRED(getArgs(), user->getNick()), user->getFd()));
    if (user->param_list.size() == 0)
    {
        user->setStatus("out");
        return (user->getServer()->toSend(
            ERR_NEEDMOREPARAMS(getArgs("PASS"), user->getNick()), user->getFd()));
    }
	if (user->getServer()->getPassword() != user->param_list[0])
    {
        #ifdef DEBUG
            std::cout << RED "FAILED TO CONNECT : wrong password" E << std::endl;
        #endif
		user->setStatus("out");
    }
    else
    {
        #ifdef DEBUG
            std::cout << GREEN "CONNECTED/REGISTERED !" E << std::endl;
        #endif
        user->setStatus("pending");
    }
}
