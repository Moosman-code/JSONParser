#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <any>
#include <string>

static class Utilities 
{
public:
	std::string TrimWhitespaceAndComma(const std::string& str);
	bool CheckIfNumber(std::string input);
	std::vector<std::string> SplitAndPassArguments(std::string& token);
	Utilities() = default;
};
