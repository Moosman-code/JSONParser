#include <iostream>
#include "JSONObject.h"

class JSONData : public Data
{
private:
	JSONObject* obj;
public:
	JSONData()
	{
		this->obj = new JSONObject();
	}

	~JSONData()
	{
		delete this->obj;
	}

	void AddPair(std::pair<std::string, Data*> pair)
	{
		this->obj->GetData().insert(pair);
	}

	void print() override
	{
		std::cout << "-~| Object |~-" << std::endl;
		for (const auto& pair : this->obj->GetData())
		{
			std::cout << "Key: " << pair.first << std::endl;
			pair.second->print();
			std::cout << std::endl;
		}

		std::cout << "-~| |~-" << std::endl;
	}

};