#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
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
	if (!Validate(jsonFile)) 
	{
		std::cout << "JSON file is not in correct format" << std::endl;
		return;
	}

	std::string token;
	while (std::getline(jsonFile, token))
	{
		if (token == "{" || token == "}")
		{
			continue;
		}
		else if (token == "[")
		{
			VectorData* vectorData = new VectorData();
			std::string propName = "Vector";

			while (std::getline(jsonFile, token))
			{
				if (token == "]") 
				{
					continue;
				}

				token = utilities->TrimWhitespaceAndComma(token);
				Data* propValue = CoverComplicatedCasses(token, jsonFile);
				vectorData->Add(propValue);
			}

			this->jsonData.insert(std::make_pair(propName, vectorData));
			return;
		}
		else 
		{
			// Asign
			std::vector<std::string> args = utilities->SplitAndPassArguments(token);

			std::string firstArg = utilities->TrimWhitespaceAndComma(args[0]);
			std::string secondArg = utilities->TrimWhitespaceAndComma(args[1]);

			std::string propName = AssignPropName(firstArg);
			Data* propValue = CoverComplicatedCasses(secondArg, jsonFile);
			this->jsonData.insert(std::make_pair(propName, propValue));
		}

	}
}

// VALIDATE Function
bool JSONObject::Validate(std::ifstream& ifFile) 
{
	int lineNumber = 1;
	std::stack<std::string> brackets;

	std::string line;
	while (std::getline(ifFile, line)) 
	{
		if (line == "") 
		{
			std::cout << "There is a line that is a whitespace at line: " << lineNumber << std::endl;
			return false;
		}

		line = utilities->TrimWhitespaceAndComma(line);
		if (line.size() == 1) 
		{
			if(!CheckBrackets(brackets, line, lineNumber)) 
			{
				return false;
			}
		}
		else 
		{
			std::vector<std::string> lineData = utilities->SplitAndPassArguments(line);
			if (!CheckIfKeyIsValid(lineData[0])) 
			{
				std::cout << "Key not in correct format at line: " << lineNumber << std::endl;
				return false;
			}
			else if (lineData[1].length() == 1 && !(utilities->CheckIfNumber(lineData[1])) && !CheckBrackets(brackets, lineData[1], lineNumber))
			{
				return false;
			}
			else if(!CheckIfPropertyIsValid(lineData[1], lineNumber))
			{
				return false;
			}
		}

		lineNumber++;
	}

	return true;
}

bool JSONObject::CheckBrackets(std::stack<std::string>& brackets, std::string line, int lineNumber)
{
	if (line == "{")
	{
		brackets.push(line);
	}
	else if (line == "}")
	{
		if (brackets.size() == 0 || brackets.top() != "{")
		{
			std::cout << "Object brackets don't match at line: " << lineNumber << std::endl;
			return false;
		}
		brackets.pop();
	}
	else if (line == "[")
	{
		brackets.push(line);
	}
	else if (line == "]")
	{
		if (brackets.size() == 0 || brackets.top() != "[")
		{
			std::cout << "Vector brackets don't match at line: " << lineNumber << std::endl;
			return false;
		}
		brackets.pop();
	}

	return true;
}

bool JSONObject::CheckIfKeyIsValid(std::string key) 
{
	if (key[0] != '\"' || key[key.length() - 1] != '\"') 
	{
		return false;
	}

	return true;
} 

bool JSONObject::CheckIfPropertyIsValid(std::string prop, int lineNumber)
{
	if ((prop[0] == '\"' && prop[prop.size() - 1] != '\"') || (prop[0] != '\"' && prop[prop.size() - 1] == '\"'))
	{
		std::cout << "Bad prop value of type string decleration at line: " << lineNumber << std::endl;
		return false;
	}
	else if (!utilities->CheckIfNumber(prop) && prop != "true" && prop != "false" && prop != "{" && prop != "}" && prop != "[" && prop != "]" && prop != "null" && prop[0] != '\"' && prop[prop.size() - 1] != '\""')
	{
		std::cout << "Incorrect prop decleration at line: " << lineNumber << std::endl;
	}

	return true;
}

// PRINT Function
void JSONObject::Print() 
{
	for (const auto& pair : this->jsonData) 
	{
		std::cout << "Key: " << pair.first << std::endl;
		pair.second->print();
		std::cout << std::endl;
	}
}

// SEARCH Function
JSONObject* JSONObject::Search(std::string searchedKey)
{
	VectorData* returnPropValue = new VectorData();

	for (auto& pair : this->jsonData) 
	{
		if (pair.first == searchedKey)
		{
			returnPropValue->Add(pair.second);
		}
		else if (dynamic_cast<VectorData*>(pair.second)) 
		{
			VectorSearch(searchedKey, pair.second, returnPropValue);
		}
		else if (dynamic_cast<JSONData*>(pair.second)) 
		{
			ObjectSearch(searchedKey, pair.second, returnPropValue);
		}
	}

	if (returnPropValue->GetVector().size() == 0)
	{
		std::cout << "No such key was found  (0_0)" << std::endl;
	}

	JSONObject* returnValue = new JSONObject();
	hash_map returnMap;
	returnMap.insert(std::make_pair("foundValues", returnPropValue));
	returnValue->SetData(returnMap);
	return returnValue;
}

void JSONObject::VectorSearch(std::string searchedKey, Data* _vectorData, VectorData* returnValue)
{
	VectorData* vectorData = dynamic_cast<VectorData*>(_vectorData);

	for (auto& element : vectorData->GetVector())
	{
		if (dynamic_cast<JSONData*>(element))
		{
			ObjectSearch(searchedKey, element, returnValue);
		}
		else if (dynamic_cast<VectorData*>(element)) 
		{
			VectorSearch(searchedKey, element, returnValue);
		}
	}
}

void JSONObject::ObjectSearch(std::string searchedKey, Data* _objectData, VectorData* returnValue) 
{
	JSONData* objectData = dynamic_cast<JSONData*>(_objectData);

	for (auto& pair : objectData->GetMap()) 
	{
		if (pair.first == searchedKey) 
		{
			returnValue->Add(pair.second);
		}
		else if (dynamic_cast<VectorData*>(pair.second)) 
		{
			VectorSearch(searchedKey, pair.second, returnValue);
		}
		else if (dynamic_cast<JSONData*>(pair.second)) 
		{
			ObjectSearch(searchedKey, pair.second, returnValue);
		}
	}
}

// DELETE Function


// SET Function
void JSONObject::Set(std::string path, std::string input) 
{
	std::vector<std::string> pathStrings = GetPathVector(path);
	if (CheckIfPathExists(pathStrings, this->jsonData, 0, input))
	{
		// Create a file that contains the input string
		JSONObject* inputObject = new JSONObject();
		std::ofstream ofFile("input.json");
		ofFile << input << std::endl;
		ofFile.close();
		std::ifstream ifFile("input.json");
		inputObject->JSONParse(ifFile);
	}
	else 
	{
		std::cout << "Path does not exists" << std::endl;
	}
}

std::vector<std::string> JSONObject::GetPathVector(std::string path) 
{
	std::vector<std::string> output;
	std::string firstArg;
	std::string secondArg;

	while (path.find('/') != -1)
	{
		size_t delimiterIndex = path.find('/');
		firstArg = path.substr(0, delimiterIndex);
		secondArg = path.substr(delimiterIndex + 1);

		output.push_back(firstArg);
		path = secondArg;
	}

	output.push_back(path);
	return output;
}

bool JSONObject::CheckIfPathExists(std::vector<std::string> pathStrings, hash_map& map, unsigned int index, std::string input)
{
	if (index >= pathStrings.size()) {
		return false; 
	}

	std::string currKey = pathStrings[index];
	hash_map* pointerMap = &map;

	// Check if the key exists in the map
	auto it = pointerMap->find(currKey);
	if (it != pointerMap->end()) {
		// Vector case
		if (auto currVectorData = dynamic_cast<VectorData*>(it->second)) {

			if (dynamic_cast<JSONData*>(currVectorData->GetVector()[0])) {

				for (int i = 0; i < currVectorData->GetVector().size(); i++) {
					hash_map currVectorMap = (static_cast<JSONData*>(currVectorData->GetVector()[i]))->GetMap();
					if (CheckIfPathExists(pathStrings, currVectorMap, index + 1, input)) {
						return true;
					}
				}
			}
		}
		// Object case
		else if (auto currObjectData = dynamic_cast<JSONData*>(it->second)) {
			hash_map tempMap = currObjectData->GetMap();
			hash_map* currMap = &tempMap;
			if (CheckIfPathExists(pathStrings, *currMap, index + 1, input)) {
				return true;
			}
		}
		// Set value
		else if (index == pathStrings.size() - 1) {
			return true;
		}
	}

	return false;
}