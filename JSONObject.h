#pragma once
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <stack>
#include "DataTypes.cpp"
using hash_map = std::unordered_map<std::string, Data*>;

class JSONObject 
{
private:
	std::unordered_map<std::string, Data*> jsonData;
public:
	virtual std::unordered_map<std::string, Data*>& GetData();
	void SetData(hash_map& other);
	std::string AssignPropName(std::string propName);
	Data* AssignPropValue(std::string propValue);
	Data* CoverComplicatedCasses(std::string secondArg, std::ifstream& jsonFile);

	Data* CreateVector(std::ifstream& jsonFile);
	Data* CreateObject(std::ifstream& jsonFile);

	bool Validate(std::ifstream& ifFile);
	bool CheckBrackets(std::stack<std::string>& brackets, std::string line, int lineNumber);
	bool CheckIfKeyIsValid(std::string key);
	bool CheckIfPropertyIsValid(std::string prop, int lineNumber);

	void Print();

	JSONObject* Search(std::string searchedKey);
	void VectorSearch(std::string searchedKey, Data* _vectorData, VectorData* returnValue);
	void ObjectSearch(std::string searchedKey, Data* _objectData, VectorData* returnValue);

	void Set(std::string path, std::string input);
	std::vector<std::string> GetPathVector(std::string path);
	bool CheckIfPathExists(std::vector<std::string> pathStrings, hash_map& map, unsigned int index, std::string input);

	void JSONParse(std::ifstream& jsonFile);

public:
	JSONObject();
	JSONObject(const JSONObject& other);
};
