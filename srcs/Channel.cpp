#include "Channel.hpp"

Channel::Channel(const std::string &aName) : _name(aName)
{
	_topic = "";
	_flags["secret"] = false;
	_flags["moderated"] = false;
	_flags["inviteOnly"] = false;
	_flags["anonymous"] = false;
	_flags["private"] = false;
	_flags["topic"] = false;
	_key = "";
	_user_limit = 500;
}

Channel::~Channel()
{
	_flags.clear();
	_invite_list.clear();
	_ban_list.clear();
	_users.clear();
}

const std::string &Channel::getName() const
{
	return _name;
}

const std::string &Channel::getTopic() const
{
	return _topic;
}

const std::string &Channel::getKey() const
{
	return _key;
}

int Channel::getUserLimit() const
{
	return _user_limit;
}

std::vector<User *> Channel::getUserList() const
{
	std::vector<User *> userList;
	for (std::map<std::string, User *>::const_iterator it = _users.begin();
		 it != _users.end(); it++)
		userList.push_back(it->second);
	return userList;
}

const std::vector<std::string> Channel::getNickList() const
{
	std::vector<std::string> nickList;
	for (std::map<std::string, User *>::const_iterator it = _users.begin();
		 it != _users.end(); it++)
		nickList.push_back(it->first);
	return nickList;
}

int Channel::getUserNum() const
{
	return _users.size();
}

const std::vector<int> Channel::getFds() const
{
	std::vector<int> fds;

	for (std::map<std::string, User *>::const_iterator it = _users.begin();
		 it != _users.end(); it++)
		fds.push_back(it->second->getFd());
	return fds;
}

const std::vector<int> Channel::getOtherFds(const std::string &nick) const
{
	std::vector<int> fds;

	for (std::map<std::string, User *>::const_iterator it = _users.begin();
		 it != _users.end(); it++)
	{
		if (nick != it->second->getNick())
			fds.push_back(it->second->getFd());
	}
	return fds;
}

bool Channel::isInvited(const std::string &nick) const
{
	if (std::find(_invite_list.begin(), _invite_list.end(), nick) != _invite_list.end())
		return true;
	return false;
}

bool Channel::isBanned(const std::string &nick) const
{
	if (std::find(_ban_list.begin(), _ban_list.end(), nick) != _ban_list.end())
		return true;
	return false;
}

bool Channel::isChanOp(const std::string &nick)
{
	if (_user_modes[nick]["operator"] == true)
		return true;
	return false;
}

bool Channel::isVoiced(const std::string &nick)
{
	if (_user_modes[nick]["voiced"] == true)
		return true;
	return false;
}

bool Channel::isHere(const std::string &nick) const
{
	if (_users.find(nick) != _users.end())
		return true;
	return false;
}

bool Channel::isSecret()
{
	if (_flags["secret"] == true)
		return true;
	return false;
}

bool Channel::isModerated()
{
	if (_flags["moderated"] == true)
		return true;
	return false;
}

bool Channel::isInviteOnly()
{
	if (_flags["inviteOnly"] == true)
		return true;
	return false;
}

bool Channel::isAnonymous()
{
	if (_flags["anonymous"] == true)
		return true;
	return false;
}

bool Channel::isPrivate()
{
	if (_flags["private"] == true)
		return true;
	return false;
}

bool Channel::isTopicOperatorOnly()
{
	if (_flags["topic"] == true)
		return true;
	return false;
}

bool Channel::hasOneOp()
{
	int count = 0;

	for (std::map<std::string, std::map<std::string, bool> >::iterator it = _user_modes.begin();
		 it != _user_modes.end(); it++)
	{
		if (it->second["operator"] == true)
			count++;
	}
	if (count >= 1)
		return true;
	return false;
}

bool	Channel::canSpeak(const std::string& nick)
{
	if (!isHere(nick) || isBanned(nick) || (!isVoiced(nick) && isModerated()))
		return false;
	return true;
}

void	Channel::banUser(const std::string& nick)
{
	_ban_list.push_back(nick);
	_users.erase(nick);
	_user_modes.erase(nick);
}

void Channel::inviteUser(const std::string &nick)
{
	_invite_list.push_back(nick);
}

void Channel::unbanUser(const std::string &nick)
{
	std::vector<std::string>::iterator it = std::find(_ban_list.begin(), _ban_list.end(), nick);
	if (it != _ban_list.end())
		_ban_list.erase(it);
}

void Channel::uninviteUser(const std::string &nick)
{
	std::vector<std::string>::iterator it = std::find(_invite_list.begin(), _invite_list.end(), nick);
	if (it != _invite_list.end())
		_invite_list.erase(it);
}

void Channel::kickUser(const std::string &nick)
{
	_users[nick]->removeChannel(_name);
	_users.erase(nick);
	_user_modes.erase(nick);
}

void Channel::addUser(User *user)
{
	std::string nick = user->getNick();
	_users[nick] = user;
	if (_user_modes.empty())
		_user_modes[nick]["operator"] = true;
	else
		_user_modes[nick]["operator"] = false;
	_user_modes[nick]["voiced"] = false;
}

void Channel::setName(const std::string &aName)
{
	_name = aName;
}

void Channel::setTopic(const std::string &aTopic)
{
	_topic = aTopic;
}

void Channel::setKey(const std::string &aKey)
{
	_key = aKey;
}

void Channel::setUserLimit(int aUserLimit)
{
	_user_limit = aUserLimit;
}

void Channel::setChanFlag(const std::string &flag, bool b)
{
	_flags[flag] = b;
}

void Channel::setUserOp(const std::string &nick, bool b)
{
	_user_modes[nick]["operator"] = b;
}

void Channel::setUserVoiced(const std::string &nick, bool b)
{
	_user_modes[nick]["voiced"] = b;
}

void	Channel::updateUser(const std::string& oldNick, const std::string& newNick)
{
	std::map<std::string, User*>::iterator it = _users.find(oldNick);
	if (isBanned(oldNick))
	{
		unbanUser(oldNick);
		banUser(newNick);
	}
	if (isInvited(oldNick))
	{
		uninviteUser(oldNick);
		inviteUser(newNick);
	}
	if (it != _users.end())
	{
		User* user = it->second;
		_users.erase(it);
		_users[newNick] = user;
	}
}
