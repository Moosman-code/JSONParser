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
		std::cout << this->value;
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
		std::cout << this->value;
	}
};

//NULL
class NullData : public Data 
{
public:
	void print() override
	{
		std::cout << "Null";
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
		std::cout << this->value;
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
		for (Data* data : this->value)
		{
			data->print();
		}
	}

	void Add(Data* _data) 
	{
		this->value.push_back(_data);
	}
};