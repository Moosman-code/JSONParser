#include <iostream>
#include <string>
#include <sstream>
#include "Utilities.h"

std::string Utilities::TrimWhitespaceAndComma(const std::string& str)
{
    std::size_t start = 0;
    std::size_t end = str.length() - 1;

    while (start < str.length() && std::isspace(str[start])) 
    {
        start++;
    }

    while (end >= start && std::isspace(str[end])) 
    {
        end--;
    }

    std::string trimmed;
    if (str[str.length() - 1] == ',') 
    {
        trimmed = str.substr(start, end - start);
    }
    else {
        trimmed = str.substr(start, end - start + 1);
    }
    return trimmed;
}

bool Utilities::CheckIfNumber(std::string input) 
{
    for (int i = 0; i < input.length(); i++)
    {
        if (!isdigit(input[i]) && input[i] != '.' && input[i] != '-')
        {
            return false;
        }
    }
    return true;
}
