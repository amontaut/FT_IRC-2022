#include "utils.hpp"

const std::string	getStr(std::vector<std::string>& v)
{
	std::string str = "";
	for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); it++)
	{
		str += *it;
		if (it != v.end() - 1)
			str += " ";
	}
	return str;	
}
