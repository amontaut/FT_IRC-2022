/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkParam.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amontaut <amontaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 10:45:52 by ali               #+#    #+#             */
/*   Updated: 2022/11/21 10:33:21 by amontaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool	isChanName(const std::string& param)
{
	// const char *str = param.c_str();
	if (param[0] == '&' || param[0] == '#' || param[0] == '+' || param[0] == '!')
		return true;
	return false;
}