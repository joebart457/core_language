#pragma once

#include <string>
#include <vector>

bool contains(const std::string& s, const std::string& match)
{
	return s.find(match) != std::string::npos;
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

std::string lower(const std::string& s)
{
	std::string data = s;
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}