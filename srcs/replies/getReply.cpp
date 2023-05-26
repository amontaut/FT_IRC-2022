/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getReply.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 18:53:39 by ali               #+#    #+#             */
/*   Updated: 2022/11/21 10:33:21 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "replies.hpp"

std::string	getReply(unsigned short code, std::vector<std::string> args,
		std::string target_nickname)
{
	std::stringstream ss;
	ss << code;
	std::string codestr = ss.str();
 	if (code < 10)
 		codestr = "00" + codestr;
 	else if (code < 100)
 		codestr = "0" + codestr;

	std::string end = getReplyEnd(code, args);
	if (end != "")
		return ":OurSuperServer " + codestr + " " + target_nickname + " " 
		+ end + "\r\n";
	else
		return "";
}

/* Pour RPL_NAMERPLY, RPL_WHOREPLY et RPL_WHOISCHANNELS, il y a des trucs que j'ai pas encore compris.
 * Il faudra peut-être plus d'arguments que simplement les trucs indiqués <> pour ceux-là */
std::string	getReplyEnd(unsigned short code, std::vector<std::string> args)
{
	switch(code)
	{
		case 1:
			return ":Welcome to the Internet Relay Network " + args[0] + "!" + args[1] + "@" + args[2];
		case 462:
			return ":Unauthorized command (already registered)";
		case 461:
			return args[0] + " :Not enough parameters";
		case 431:
			return ":No nickname given";
		case 433:
			return args[0] + " :Nickname is already in use";
		case 437:
			return args[0] + " :Nick/channel is temporarily unavailable";
		case 432:
			return args[0] + " :Erroneous nickname";
		case 484:
			return ":Your connection is restricted!";
		case 501:
			return ":Unknown MODE flag";
		case 502:
			return ":Cannot change mode for other users";
		case 221:
			return args[0];
		case 473:
			return args[0] + " :Cannot join channel (+i)";
		case 471:
			return args[0] + " :Cannot join channel (+l)";
		case 403:
			return args[0] + " :No such channel";
		case 407:
			return args[0] + " :<error code> recipients. <abort message>";
		case 332:
			return args[0] + " :" + args[1];
		case 474:
			return args[0] + " :Cannot join channel (+b)";
		case 475:
			return args[0] + " :Cannot join channel (+k)";
		case 476:
			return args[0] + " :Bad Channel Mask";
		case 405:
			return args[0] + " :You have joined too many channels";
		case 442:
			return args[0] + " :You're not on that channel";
		case 477:
			return args[0] + " :Channel doesn't support modes";
		case 441:
			return args[0] + " " + args[1] + " :They aren't on that channel";
		case 324:
			return args[0] + " " + args[1] + " " + args[2];
		case 367:
			return args[0] + " " + args[1];
		case 348:
			return args[0] + " " + args[1];
		case 346:
			return args[0] + " " + args[1];
		case 325:
			return args[0] + " " + args[1];
		case 467:
			return args[0] + " :Channel key already set";
		case 482:
			return args[0] + " :You're not channel operator";
		case 472:
			return args[0] + " :is unknown mode char to me for " + args[1];
		case 368:
			return args[0] + " :End of channel ban list";
		case 349:
			return args[0] + " :End of channel exception list";
		case 347:
			return args[0] + " :End of channel invite list";
		case 331:
			return args[0] + " :No topic is set";
		case 353:
			return args[0] + " :" + args[1];
		case 366:
			return args[0] + " :End of NAMES list";
		case 402:
			return args[0] + " :No such server";
		case 322:
			return args[0] + " " + args[1] + " :" + args[2];
		case 323:
			return ":End of LIST";
		case 341:
			return args[0] + " " + args[1];
		case 401:
			return args[0] + " :No such nick/channel";
		case 443:
			return args[0] + " " + args[1] + " :is already on channel";
		case 301:
			return args[0] + " :" + args[1];
		case 411:
			return ":No recipient given (" + args[0] + ")";
		case 404:
			return args[0] + " :Cannot send to channel";
		case 414:
			return args[0] + " :Wildcard in toplevel domain";
		case 412:
			return ":No text to send";
		case 413:
			return args[0] + " :No toplevel domain specified";
		case 352:
			return args[0] + " " + args[1] + " " + args[2] + " " + args[3] + " " + args[4] 
				/*truc chelou*/ + " :" + args[5] + " " + args[6];
		case 315:
			return args[0] + " :End of WHO list";
		case 311:
			return args[0] + " " + args[1] + " " + args[2] + " * " + ":" + args[3];
		case 319:
			return args[0] + " :" /*truc chelou*/ + args[1];
		case 317:
			return args[0] + " " + args[1] + " :seconds idle";
		case 318:
			return args[0] + " :End of WHOIS list";
		case 312:
			return args[0] + " " + args[1] + " :" + args[2];
		case 313:
			return args[0] + " :is an IRC operator";
		case 409:
			return ":No origin specified";
		case 421:
			return args[0] + " :Unknown command";
		case 4:
			return args[0] + " " + args[1] + " " + args[2] + " " + args[3];
		case 305:
			return ":You are no longer marked as being away";
		case 306:
			return ":You have been marked as being away";
	}
	return "";
}
/*
int main()
{
	std::vector<std::string> args;
	args.push_back("<arg0>");
	args.push_back("<arg1>");
	args.push_back("<arg2>");
	args.push_back("<arg3>");
	args.push_back("<arg4>");
	args.push_back("<arg5>");
	args.push_back("<arg6>");
	args.push_back("<arg7>");
	for (unsigned short i = 0; i < 500; i++)
	{
		std::string reply = getReply(i, args, "<target_nick>");
		if (reply != "")
			std::cout << reply << std::endl;
	}
}
*/
