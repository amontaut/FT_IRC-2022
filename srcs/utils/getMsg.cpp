/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMsg.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:17:03 by ali               #+#    #+#             */
/*   Updated: 2022/11/21 10:33:21 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

const std::string    getMsg(User* user, const std::string& command,
        std::vector<std::string>& params)
{
    std::string msg = ":" + user->getNick() + "!" + user->getUsername() + "@"
        + user->getHostname() + " " + command + " ";
    for (std::vector<std::string>::iterator it = params.begin(); it != params.end();
            it++)
	{
        msg += *it;
        if (it != params.end() - 1)
            msg += " ";
	}
	msg += "\r\n";

    return msg;
}

const std::string	getMsg(User* user, const std::string& command, const std::string& param)
{
    return ":" + user->getNick() + "!" + user->getUsername() + "@"
        + user->getHostname() + " " + command + " " + param + "\r\n";
}
