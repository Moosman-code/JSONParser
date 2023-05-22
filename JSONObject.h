#pragma once
#include <iostream>
#include <unordered_map>
#include <any>
#include <fstream>
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

	void JSONParse(std::ifstream& jsonFile);

public:
	JSONObject();
	JSONObject(const JSONObject& other);
};
