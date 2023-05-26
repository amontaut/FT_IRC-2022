#include "command.hpp"
#include "Server.hpp"
#include "colors.hpp"
#include "Channel.hpp"

int specialchar_join(char c)
{
    if (c == ' ' ||
        c == 7 ||
        c == ',' ||
        c == ':')
        return (1);
    return (0);
}

int check_forbiden_char_join(std::string channel)
{
    if (channel != "")
    {
        unsigned int i = 1;
        while (i < channel.length())
        {
            if (specialchar_join(channel[i]) == 1)
                return (0);
            i++;
        }
    }
    return (1);
}

void nameReply_join(User *user, Channel *chan)
{
    std::string reply_channel;
    std::string reply_nick = "";

    if (chan->isSecret() == true)
        reply_channel = "@ " + chan->getName();
    else if (chan->isPrivate() == true)
        reply_channel = "* " + chan->getName();
    else
        reply_channel = "= " + chan->getName();

    std::vector<std::string> nick_list = chan->getNickList();
    for (std::vector<std::string>::iterator it = nick_list.begin(); it != nick_list.end(); it++)
    {
        if (reply_nick != "")
            reply_nick += " ";
        if (chan->isChanOp(*it))
            reply_nick += "@" + *it;
        else if (chan->isModerated() && chan->canSpeak(*it))
            reply_nick += "+" + *it;
        else
            reply_nick += " " + *it;
    }
    user->getServer()->toSend(RPL_NAMEREPLY(getArgs(reply_channel, reply_nick),
                                            user->getNick()),
                              user->getFd());
    user->getServer()->toSend(RPL_ENDOFNAMES(getArgs(chan->getName()), user->getNick()), user->getFd());
    user->getServer()->toSend(getMsg(user, "JOIN", chan->getName()),
                              chan->getFds());
}

void create_channel(User *user, std::string channel, std::string pwdchan)
{
    if (channel.size() >= 50)
        return user->getServer()->toSend(ERR_NOSUCHCHANNEL(getArgs(channel), user->getNick()),
                                         user->getFd());
    if (check_forbiden_char_join(channel) == 0)
        return user->getServer()->toSend(ERR_BADCHANMASK(getArgs(channel), user->getNick()),
                                         user->getFd());
   if (user->getChannelList().size() >= static_cast<unsigned int>(user->getChannelLimit()))
        return user->getServer()->toSend(ERR_TOOMANYCHANNELS(getArgs(channel), user->getNick()),
                                         user->getFd());
    Channel *chan = new Channel(channel);
    chan->setName(channel);
    chan->addUser(user);
    user->getServer()->addChannel(channel, chan);
    user->addChannel(channel);
    chan->setUserOp(user->getNick(), true);
    YELLOW;

    if (pwdchan != "")
        chan->setKey(pwdchan);

    nameReply_join(user, chan);
}

void join_channel(Channel *chan, User *user)
{
    std::string channel = chan->getName();
    if (chan->isBanned(user->getNick()) == 1)
        return user->getServer()->toSend(ERR_BANNEDFROMCHAN(getArgs(channel), user->getNick()),
                                         user->getFd());
    if (chan->getUserNum() >= chan->getUserLimit())
        return user->getServer()->toSend(ERR_CHANNELISFULL(getArgs(channel), user->getNick()),
                                         user->getFd());
    if (user->getChannelList().size() >= static_cast<unsigned int>(user->getChannelLimit()))
        return user->getServer()->toSend(ERR_TOOMANYCHANNELS(getArgs(channel), user->getNick()),
                                         user->getFd());
    if (chan->isInvited(user->getNick()) == false && chan->isInviteOnly() == 1)
        return user->getServer()->toSend(ERR_INVITEONLYCHAN(getArgs(channel), user->getNick()),
                                         user->getFd());

    chan->addUser(user);
    user->addChannel(chan->getName());

    if (chan->getTopic() != "")
        user->getServer()->toSend(RPL_TOPIC(getArgs(channel, chan->getTopic()),
                                            user->getNick()),
                                  user->getFd());
    else
        user->getServer()->toSend(RPL_NOTOPIC(getArgs(channel),
                                              user->getNick()),
                                  user->getFd());

    if (chan->hasOneOp() == false)
        chan->setUserOp(user->getNick(), true);

    nameReply_join(user, chan);
}

void JOIN(User *user)
{
    if (user->param_list.size() == 0)
        return user->getServer()->toSend(ERR_NEEDMOREPARAMS(getArgs("JOIN"), user->getNick()),
                                         user->getFd());
    std::map<std::string, Channel *> channelList = user->getServer()->getChannelList();
    if (user->param_list[0] == "0" || user->param_list[0] == "#0")
    {
        for (std::map<std::string, Channel *>::iterator it = channelList.begin(); it != channelList.end(); ++it)
        {
            user->toCommand("PART " + it->first);
        }
        user->toCommand("PART 0");
        return;
    }

    std::string key;
    std::vector<std::string> listNewChans;
    if (user->param_list[0][0] == '#' || user->param_list[0][0] == '&' || user->param_list[0][0] == '+' || user->param_list[0][0] == '!')
        listNewChans = splitStr(user->param_list[0], ",");
    else
        return;
    std::vector<std::string> listNewPwd;
    if (user->param_list.size() != 1 && user->param_list[1][0] != '#' && user->param_list[1][0] != '&' && user->param_list[1][0] != '+' && user->param_list[1][0] != '!' && isalnum(user->param_list[1][0]) != 0)
    {
        if (user->param_list[1][0] != '#' || user->param_list[1][0] != '&' || user->param_list[1][0] != '+' || user->param_list[1][0] != '!')
            listNewPwd = splitStr(user->param_list[1], ",");
    }

    #ifdef DEBUG
        std::cout << YELLOW "chans are:" E << std::endl;
        for (std::vector<std::string>::iterator it = listNewChans.begin(); it != listNewChans.end(); ++it)
            std::cout << ' ' << *it;
        std::cout << std::endl;

        std::cout << YELLOW "pwd are:" E << std::endl;
        for (std::vector<std::string>::iterator it = listNewPwd.begin(); it != listNewPwd.end(); ++it)
            std::cout << ' ' << *it;
        std::cout << std::endl;
    #endif

    unsigned int i = 0;
    while (i < listNewChans.size())
    {
        Channel *chan = user->getServer()->getChannel(listNewChans[i]);
        if (listNewPwd.size() > 0)
        {
            if (listNewPwd[i].size() > 0)
                key = listNewPwd[i];
        }
        else
        {
            key = "";
        }
        if (chan == NULL)
        {
            create_channel(user, listNewChans[i], key);
        }
        else
        {
            if (chan->isHere(user->getNick()) == 1)
            {
                std::cout << "Already in the chan" << std::endl;
                return;
            }
            if (key == chan->getKey())
                join_channel(chan, user);
            else
                user->getServer()->toSend(ERR_BADCHANNELKEY(getArgs(listNewChans[i]), user->getNick()),
                                          user->getFd());
        }
        i++;
    }
    #ifdef DEBUG
        std::cout << YELLOW "all chan + pwd + nick in the chan" E << std::endl;
        std::map<std::string, Channel *> channelsList = user->getServer()->getChannelList();
        for (std::map<std::string, Channel *>::iterator it = channelsList.begin(); it != channelsList.end(); ++it)
        {
            std::cout << it->first << "   ";
            std::cout << it->second->getKey() << std::endl;
            std::vector<std::string> listNick = it->second->getNickList();
            for (std::vector<std::string>::iterator it = listNick.begin(); it != listNick.end(); ++it)
                std::cout << ' ' << *it << std::endl;
        }
    #endif
}
