#include "utils.hpp"

const std::vector<std::string> splitStr(std::string& str, const std::string& delimiter)
{
	std::vector<std::string> strs;
	size_t	pos = 0;
	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		strs.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	if (str.find(delimiter) == std::string::npos)
		strs.push_back(str);
	return strs;
}
