#include <iostream>
#include <string>
#include <sstream>
#include "JSONObject.h"
#include "Utilities.h"
using hash_map = std::unordered_map<std::string, std::any>&;

Utilities* utilities = new Utilities();

hash_map JSONObject::GetData()
{
	return this->jsonData;
}

JSONObject::JSONObject()
{
	std::unordered_map<std::string, std::any> map;
	this->jsonData = map;
}

JSONObject::JSONObject(const JSONObject& other)
{
	this->jsonData = other.jsonData;
}


// JSON PARSER FUNCTIONS
std::string JSONObject::AssignPropName(std::string propName) 
{
	return propName.substr(1, propName.length() - 2);
}

std::any JSONObject::AssignPropValue(std::string propValue) 
{
	std::any result = 0;

	// STRING CASE
	if (propValue.substr(0, 1) == "\"" && propValue.substr(propValue.length() - 1, 1) == "\"") 
	{
		result = propValue.substr(1, propValue.length() - 2);
	} 
	// NUMBER CASE
	else if (utilities->CheckIfNumber(propValue)) 
	{
		result = std::stod(propValue);
	}
	// NULL CASE
	else if (propValue == "null") 
	{
		result = nullptr;
	}
	// BOOLEAN CASE
	else if (propValue == "true" || propValue == "false")
	{
		result = (propValue == "true") ? true : false;
	}

	return result;
}

// JSON PARSER
void JSONObject::JSONParse(std::ifstream& jsonFile) 
{
	std::string token;
	while (std::getline(jsonFile, token))
	{
		if (token == "{") 
		{
			continue;
		}

		// Split the line
		std::stringstream sstream(token);

		std::vector<std::string> lineData;
		std::string data;

		while (std::getline(sstream, data, ':'))
		{
			lineData.push_back(data);
		}

		// Asign
		std::string firstArg = utilities->TrimWhitespaceAndComma(lineData[0]);
		std::string secondArg = utilities->TrimWhitespaceAndComma(lineData[1]);

		if (secondArg == "{") 
		{
			std::string innerToken;
			while (std::getline(jsonFile, innerToken)) 
			{

			}
		}

		std::string propName = AssignPropName(firstArg);
		std::any propValue = AssignPropValue(secondArg);
	}
}
