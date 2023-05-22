#include <iostream>
#include <string>
#include <sstream>
#include "JSONObject.h"
#include "Utilities.h"
#include "JSONData.cpp"
// The tokens identifiers are meaningless
using hash_map = std::unordered_map<std::string, Data*>;

Utilities* utilities = new Utilities();

hash_map& JSONObject::GetData()
{
	return this->jsonData;
}
void JSONObject::SetData(hash_map& other)
{
	this->jsonData = other;
}

JSONObject::JSONObject()
{
	std::unordered_map<std::string, Data*> map;
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

Data* JSONObject::AssignPropValue(std::string propValue) 
{
	Data* result = 0;

	// STRING CASE
	if (propValue.substr(0, 1) == "\"" && propValue.substr(propValue.length() - 1, 1) == "\"") 
	{
		result = new StringData(propValue.substr(1, propValue.length() - 2));
	} 
	// NUMBER CASE
	else if (utilities->CheckIfNumber(propValue)) 
	{
		result = new NumberData(std::stod(propValue));
	}
	// NULL CASE
	else if (propValue == "null") 
	{
		result = new NullData();
	}
	// BOOLEAN CASE
	else if (propValue == "true" || propValue == "false")
	{
		result = new BoolData((propValue == "true") ? true : false);
	}

	return result;
}


Data* JSONObject::CreateVector(std::ifstream& jsonFile) 
{
	VectorData* propValueVector = new VectorData();

	std::string innerToken;
	std::getline(jsonFile, innerToken);
	innerToken = utilities->TrimWhitespaceAndComma(innerToken);

	// Add all sub-vectors to propValueVector using recursion
	while(innerToken == "[") 
	{
		propValueVector->Add(CreateVector(jsonFile));
		std::getline(jsonFile, innerToken);
		innerToken = utilities->TrimWhitespaceAndComma(innerToken);
	}

	// Add all values to the sub/initial vector
	while (innerToken != "]")
	{
		// Add an object to the vector recursively
		if (innerToken == "{") 
		{
			while (innerToken == "{")
			{
				propValueVector->Add(CreateObject(jsonFile));
				std::getline(jsonFile, innerToken);
				innerToken = utilities->TrimWhitespaceAndComma(innerToken);
			}
		}
		// Add a sub-vector to the vector recursively
		else 
		{
			propValueVector->Add(AssignPropValue(innerToken));
			std::getline(jsonFile, innerToken);
			innerToken = utilities->TrimWhitespaceAndComma(innerToken);
		}
	}

	return propValueVector;
}


Data* JSONObject::CoverComplicatedCasses(std::string secondArg, std::ifstream& jsonFile) 
{
	// VECTOR CASE
	if (secondArg == "[")
	{
		Data* propValueVector = new VectorData();
		propValueVector = CreateVector(jsonFile);
		return propValueVector;
	}
	// OBJECT CASE
	else if (secondArg == "{")
	{
		Data* propValueObject = new JSONData();
		propValueObject =  CreateObject(jsonFile);
		return propValueObject;
	}
	// PRIMITIVE TYPE AND STRING CASE
	else
	{
		Data* propValue = AssignPropValue(secondArg);
		return propValue;
	}
}

Data* JSONObject::CreateObject(std::ifstream& jsonFile)
{
	JSONData* propValueObject = new JSONData();

	std::string outerToken;
	std::getline(jsonFile, outerToken);
	outerToken = utilities->TrimWhitespaceAndComma(outerToken);

	while (outerToken != "}")
	{
		std::vector<std::string> args = utilities->SplitAndPassArguments(outerToken);

		// Asign
		std::string firstArg = utilities->TrimWhitespaceAndComma(args[0]);
		std::string secondArg = utilities->TrimWhitespaceAndComma(args[1]);

		std::string propName = AssignPropName(firstArg);
		Data* propValue;

		// VECTOR CASE
		if (secondArg == "[")
		{
			propValue = CreateVector(jsonFile);
		}
		// OBJECT CASE
		else if (secondArg == "{")
		{
			propValue = CreateObject(jsonFile);
		}
		// PRIMITIVE TYPE AND STRING CASE
		else
		{
			propValue = AssignPropValue(secondArg);
		}

		std::getline(jsonFile, outerToken);
		outerToken = utilities->TrimWhitespaceAndComma(outerToken);

		propValueObject->AddPair(std::make_pair(propName, propValue));
	}

	return propValueObject;
}

// JSON PARSER
void JSONObject::JSONParse(std::ifstream& jsonFile) 
{
	std::string token;
	while (std::getline(jsonFile, token))
	{
		if (token == "{" || token == "}")
		{
			continue;
		}

		// Asign
		std::vector<std::string> args = utilities->SplitAndPassArguments(token);

		std::string firstArg = utilities->TrimWhitespaceAndComma(args[0]);
		std::string secondArg = utilities->TrimWhitespaceAndComma(args[1]);

		std::string propName = AssignPropName(firstArg);
		Data* propValue = CoverComplicatedCasses(secondArg, jsonFile);
		
		this->jsonData.insert(std::make_pair(propName, propValue));
	}
}
