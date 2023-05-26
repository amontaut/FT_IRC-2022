/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 13:32:51 by ali               #+#    #+#             */
/*   Updated: 2022/11/16 18:38:55 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "std.hpp"
# include "User.hpp"

class User;

class Channel
{
	private:
		std::string	_name;
		std::string	_topic;
		std::map<std::string, bool>	_flags;
		std::vector<std::string> _invite_list;
		std::vector<std::string> _ban_list;
		std::map<std::string, User*> _users;
		std::map<std::string, std::map<std::string, bool> > _user_modes;
		// map to check for each user if voiced or if chanOp
		std::string	_key;
		int	_user_limit;

	public:
		Channel(const std::string& aName);
		~Channel();

		const std::string&	getName() const;
		const std::string&	getTopic() const;
		const std::string&	getKey() const;
		int	getUserLimit() const;
		std::vector<User*>	getUserList() const;
		const std::vector<std::string> getNickList() const;
		int	getUserNum() const;
		const std::vector<int>	getFds() const;
		const std::vector<int>	getOtherFds(const std::string& nick) const;

		bool	isInvited(const std::string& nick) const;
		bool	isBanned(const std::string& nick) const;
		bool	isChanOp(const std::string&	nick);
		bool	isVoiced(const std::string& nick);
		bool	isHere(const std::string& nick) const;

		bool	isSecret();
		bool	isModerated();
		bool	isInviteOnly();
		bool	isAnonymous();
		bool	isPrivate();
		bool	isTopicOperatorOnly();
		bool	hasOneOp();
		bool	canSpeak(const std::string& nick);

		void	banUser(const std::string& nick);
		void	inviteUser(const std::string& nick);
		void	unbanUser(const std::string& nick);
		void	uninviteUser(const std::string& nick);

		void	kickUser(const std::string &nick);
		void	addUser(User* user);

		void	setName(const std::string& aName);
		void	setTopic(const std::string& aTopic);
		void	setKey(const std::string& aKey);
		void	setUserLimit(int aUserLimit);
		void	setChanFlag(const std::string& flag, bool b);
		void	setUserOp(const std::string& nick, bool b);
		void	setUserVoiced(const std::string& nick, bool b);

		void	updateUser(const std::string& oldNick, const std::string& newNick);
};

#endif
