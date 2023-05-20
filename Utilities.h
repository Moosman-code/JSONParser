#pragma once
#include <iostream>

static class Utilities 
{
public:
	std::string TrimWhitespaceAndComma(const std::string& str);
	bool CheckIfNumber(std::string input);
	Utilities() = default;
};
