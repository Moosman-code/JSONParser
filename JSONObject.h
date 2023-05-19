#pragma once
#include <iostream>
#include <unordered_map>
#include <any>
#include <fstream>

class JSONObject 
{
private:
	std::unordered_map<std::string, std::any> jsonData;
public:
	virtual std::unordered_map<std::string, std::any>& GetData();
	void JSONParse(std::ifstream jsonFile);
	JSONObject();
	JSONObject(const JSONObject& other);
	~JSONObject();
};
