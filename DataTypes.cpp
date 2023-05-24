#include <iostream>
#include <vector>

class Data 
{
public:
	virtual void print() = 0;
};

// STRING
class StringData : public Data 
{
private:
	std::string value;
public:
	StringData(const std::string& _value) 
	{
		this->value = _value;
	}
	void print() override
	{
		std::cout << "Value: " << this->value << std::endl;
	}
};

// INT
class NumberData : public Data 
{
private:
	double value;
public:
	NumberData(const double _value)
	{
		this->value = _value;
	}
	void print() override
	{
		std::cout << "Value: " << this->value << std::endl;
	}
};

//NULL
class NullData : public Data 
{
public:
	void print() override
	{
		std::cout << "Value: " << "Null" << std::endl;
	}
};

//BOOL
class BoolData : public Data 
{
private:
	bool value;
public:
	BoolData(const bool _value)
	{
		this->value = _value;
	}
	void print() override
	{
		std::cout << "Value: " << ((this->value == 1) ? "true" : "false") << std::endl;
	}
};

//ARRAY
class VectorData : public Data 
{
private:
	std::vector<Data*> value;
public:
	VectorData() 
	{
		this->value = std::vector<Data*>();
	}
	
	~VectorData() 
	{
		for (Data* data : this->value) 
		{
			delete data;
		}
	}

	void print() override 
	{
		std::cout << "--<== Vector ==>--" << std::endl;
		int index = 1;
		for (Data* data : this->value)
		{
			std::cout << index << ". ";
			data->print();
			index++;
		}

		std::cout << "--<====>--" << std::endl;
	}

	void Add(Data* _data) 
	{
		this->value.push_back(_data);
	}
};