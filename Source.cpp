#include <iostream>
#include <any>
#include <map>
#include "JSONObject.h"

int main()
{
	JSONObject* data = new JSONObject();
	std::ifstream file;
	file.open("File.json");
	data->JSONParse(file);
	JSONObject* innerData = new JSONObject();
	
	std::unordered_map<std::string, Data*> map = data->GetData();
	file.close();
}