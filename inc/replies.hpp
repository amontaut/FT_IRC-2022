/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 17:32:17 by ali               #+#    #+#             */
/*   Updated: 2022/11/09 16:53:08 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

# include "std.hpp"

std::string getReplyEnd(unsigned short code, std::vector<std::string> args);
std::string	getReply(unsigned short code, std::vector<std::string> args,
		std::string target_nickname);

//		CONNECTION REGISTRATION (rfc 2812 3.1)

// PASS & USER
# define RPL_WELCOME(args, target_nickname) getReply(1, args, target_nickname)
# define ERR_ALREADYREGISTRED(args, target_nickname) getReply(462, args, target_nickname)
# define ERR_NEEDMOREPARAMS(args, target_nickname) getReply(461, args, target_nickname)

// NICK
# define ERR_NONICKNAMEGIVEN(args, target_nickname) getReply(431, args, target_nickname)
# define ERR_NICKNAMEINUSE(args, target_nickname) getReply(433, args, target_nickname)
# define ERR_UNAVAILRESOURCE(args, target_nickname) getReply(437, args, target_nickname)
# define ERR_ERRONEUSNICKNAME(args, target_nickname) getReply(432, args, target_nickname)
# define ERR_RESTRICTED(args, target_nickname) getReply(484, args, target_nickname)

// USER MODE
# define ERR_UMODEUNKNOWNFLAG(args, target_nickname) getReply(501, args, target_nickname)
# define ERR_USERSDONTMATCH(args, target_nickname) getReply(502, args, target_nickname)
# define RPL_UMODEIS(args, target_nickname) getReply(221, args, target_nickname)

//		CHANNEL OPERATIONS (rfc 2812 3.2)

// JOIN
# define ERR_INVITEONLYCHAN(args, target_nickname) getReply(473, args, target_nickname)
# define ERR_CHANNELISFULL(args, target_nickname) getReply(471, args, target_nickname)
# define ERR_NOSUCHCHANNEL(args, target_nickname) getReply(403, args, target_nickname)
# define ERR_TOOMANYTARGETS(args, target_nickname) getReply(407, args, target_nickname)
# define RPL_TOPIC(args, target_nickname) getReply(332, args, target_nickname)
# define ERR_BANNEDFROMCHAN(args, target_nickname) getReply(474, args, target_nickname)
# define ERR_BADCHANNELKEY(args, target_nickname) getReply(475, args, target_nickname)
# define ERR_BADCHANMASK(args, target_nickname) getReply(476, args, target_nickname)
# define ERR_TOOMANYCHANNELS(args, target_nickname) getReply(405, args, target_nickname)

// PART
# define ERR_NOTONCHANNEL(args, target_nickname) getReply(442, args, target_nickname)

// CHANNEL MODE & KICK
# define ERR_NOCHANMODES(args, target_nickname) getReply(477, args, target_nickname)
# define ERR_USERNOTINCHANNEL(args, target_nickname) getReply(441, args, target_nickname)
# define RPL_CHANNELMODEIS(args, target_nickname) getReply(324, args, target_nickname)
# define RPL_BANLIST(args, target_nickname) getReply(367, args, target_nickname)
# define RPL_EXCEPTLIST(args, target_nickname) getReply(348, args, target_nickname)
# define RPL_INVITELIST(args, target_nickname) getReply(346, args, target_nickname)
# define RPL_UNIQOPIS(args, target_nickname) getReply(325, args, target_nickname)
# define ERR_KEYSET(args, target_nickname) getReply(467, args, target_nickname)
# define ERR_CHANOPRIVSNEEDED(args, target_nickname) getReply(482, args, target_nickname)
# define ERR_UNKNOWNMODE(args, target_nickname) getReply(472, args, target_nickname)
# define RPL_ENDOFBANLIST(args, target_nickname) getReply(368, args, target_nickname)
# define RPL_ENDOFEXCEPTLIST(args, target_nickname) getReply(349, args, target_nickname)
# define RPL_ENDOFINVITELIST(args, target_nickname) getReply(347, args, target_nickname)

// TOPIC
# define RPL_NOTOPIC(args, target_nickname) getReply(331, args, target_nickname)

// NAMES
# define RPL_NAMEREPLY(args, target_nickname) getReply(353, args, target_nickname)
# define RPL_ENDOFNAMES(args, target_nickname) getReply(366, args, target_nickname)
# define ERR_NOSUCHSERVER(args, target_nickname) getReply(402, args, target_nickname)

// LIST
# define RPL_LIST(args, target_nickname) getReply(322, args, target_nickname)
# define RPL_LISTEND(args, target_nickname) getReply(323, args, target_nickname)

// INVITE
# define RPL_INVITING(args, target_nickname) getReply(341, args, target_nickname)
# define ERR_NOSUCHNICK(args, target_nickname) getReply(401, args, target_nickname)
# define ERR_USERONCHANNEL(args, target_nickname) getReply(443, args, target_nickname)
# define RPL_AWAY(args, target_nickname) getReply(301, args, target_nickname)

//		SENDING MESSAGES (rfc 2812 3.3)

// PRIVMSG & NOTICE
# define ERR_NORECIPIENT(args, target_nickname) getReply(411, args, target_nickname)
# define ERR_CANNOTSENDTOCHAN(args, target_nickname) getReply(404, args, target_nickname)
# define ERR_WILDTOPLEVEL(args, target_nickname) getReply(414, args, target_nickname)
# define ERR_NOTEXTTOSEND(args, target_nickname) getReply(412, args, target_nickname)
# define ERR_NOTOPLEVEL(args, target_nickname) getReply(413, args, target_nickname)

//		USER BASED QUERY (rfc 2812 3.6)

// WHO
# define RPL_WHOREPLY(args, target_nickname) getReply(352, args, target_nickname)
# define RPL_ENDOFWHO(args, target_nickname) getReply(315, args, target_nickname)

// WHOIS
# define RPL_WHOISUSER(args, target_nickname) getReply(311, args, target_nickname)
# define RPL_WHOISCHANNELS(args, target_nickname) getReply(319, args, target_nickname)
# define RPL_WHOISIDLE(args, target_nickname) getReply(317, args, target_nickname)
# define RPL_ENDOFWHOIS(args, target_nickname) getReply(318, args, target_nickname)
# define RPL_WHOISERVER(args, target_nickname) getReply(312, args, target_nickname)
# define RPL_WHOISOPERATOR(args, target_nickname) getReply(313, args, target_nickname)


//		MISCELLANEOUS (rfc 2812 3.7)

//	PING & PONG

# define ERR_NOORIGIN(args, target_nickname) getReply(409, args, target_nickname)


//		ADDITIONAL REPLIES

# define ERR_UNKNOWNCOMMAND(args, target_nickname) getReply(421, args, target_nickname)
# define RPL_MYINFO(args, target_nickname) getReply(4, args, target_nickname)

//		 Optional features (rfc 2812 4)
# define RPL_UNAWAY(args, target_nickname) getReply(305, args, target_nickname)
# define RPL_NOWAWAY(args, target_nickname) getReply(306, args, target_nickname)



#endif
