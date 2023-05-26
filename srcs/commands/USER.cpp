#include "command.hpp"
#include "replies.hpp"
#include "colors.hpp"

bool	checkRealName(const std::string& param)
{
	if (param.size() < 1)
		return (false);
	return (true);
}

void USER(User *user)
{
    if (user->param_list.size() < 4)
        return (user->getServer()->toSend(ERR_NEEDMOREPARAMS(
            getArgs("USER"), user->getNick()), user->getFd()
        ));
    if (user->getStatus() == "registered")
    {
        return (user->getServer()->toSend(ERR_ALREADYREGISTRED(
            getArgs(), user->getNick()), user->getFd()
        ));
    }
    else
    {
        user->setUsername(user->param_list[1]);
        user->setHostname(user->param_list[2]);

        std::string realname;
        for (unsigned int i = 3; i < user->param_list.size(); i++)
            realname.append(user->param_list[i] + ' ');
        realname = realname.substr(1,realname.size() - 1);
        if (checkRealName(realname) == true)
            user->setRealName(realname);

        #ifdef DEBUG
            std::cout << BBLUE "Your User is: " << user->getUsername() << E << std::endl;
            std::cout << BBLUE "Your Hostname is: " << user->getHostname() << E << std::endl;
            std::cout << BBLUE "Your realname is: " << user->getRealName() << E << std::endl;
        #endif

        if (user->getNick().size() != 0)
        {
            user->setStatus("registered");
            return (user->getServer()->toSend(RPL_WELCOME(
                getArgs(user->getNick(), user->getUsername(), user->getHostname()), user->getNick()), user->getFd()));
        }
        else
        {
            user->setStatus("out");
            return ;
        }
    }
}