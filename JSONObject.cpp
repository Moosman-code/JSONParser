#include <iostream>
#include "JSONObject.h"
using hash_map = std::unordered_map<std::string, std::any>&;

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

void JSONObject::JSONParse(std::ifstream jsonFile) 
{

}