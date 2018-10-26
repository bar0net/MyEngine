#pragma once
class MovingArray
{
public:
	MovingArray(unsigned int size, unsigned int default_value);
	virtual ~MovingArray();

	unsigned int& operator[](unsigned int x);
	void push(unsigned int x);

	unsigned int* values;
	unsigned int idx = 0;
	unsigned int size;

public:
	static float Get(void* data, int x); 
};

