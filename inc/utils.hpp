/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 10:44:05 by ali               #+#    #+#             */
/*   Updated: 2022/11/10 17:48:46 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "std.hpp"
#include "User.hpp"

class User;

const std::vector<std::string>	getArgs(const std::string& arg0 = "",
		const std::string& arg1 = "",
		const std::string& arg2 = "",
		const std::string& arg3 = "",
		const std::string& arg4 = "",
		const std::string& arg5 = "",
		const std::string& arg6 = "");
bool isChanName(const std::string &param);
const std::string getMsg(User *user, const std::string &command,
		std::vector<std::string> &params);
const std::string getMsg(User *user, const std::string &command,
		const std::string& params);
const std::vector<std::string>	splitStr(std::string& str, const std::string& delimiter);
const std::string	getStr(std::vector<std::string>& v);

template<class T>
const std::string	convert(const T& elem)
{
	std::ostringstream ss;
	ss << elem;
	std::cout << "convert=" << ss.str() << std::endl;
	return ss.str();
}

#endif
