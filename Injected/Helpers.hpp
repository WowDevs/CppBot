#pragma once
#include <sstream>

std::string tohex(int i)
{
	std::stringstream stream;
	stream << std::hex << i;
	std::string result(stream.str());
	return result;
}

