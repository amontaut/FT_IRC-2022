/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getArgs.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 15:06:39 by ali               #+#    #+#             */
/*   Updated: 2022/11/21 10:33:21 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

const std::vector<std::string>	getArgs(const std::string& arg0,
		const std::string& arg1,
		const std::string& arg2,
		const std::string& arg3,
		const std::string& arg4,
		const std::string& arg5,
		const std::string& arg6)
{
	std::vector<std::string> args;

	if (arg0 != "")
		args.push_back(arg0);
	if (arg1 != "")
		args.push_back(arg1);
	if (arg2 != "")
		args.push_back(arg2);
	if (arg3 != "")
		args.push_back(arg3);
	if (arg4 != "")
		args.push_back(arg4);
	if (arg5 != "")
		args.push_back(arg5);
	if (arg6 != "")
		args.push_back(arg6);

	return args;
}
