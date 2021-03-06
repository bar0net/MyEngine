#include "MovingArray.h"

#include <cstring>
#include <assert.h>

MovingArray::MovingArray(unsigned int size, unsigned int default_value) : size(size)
{
	values = new unsigned int[size];
	memset(values, default_value, size * sizeof(unsigned int));
}

MovingArray::~MovingArray()
{
	delete[]values;
}

unsigned int& MovingArray::operator[](unsigned int x)
{
	int pos = idx - x;
	if (pos < 0) pos += size;

	return values[pos];
}

void MovingArray::push(unsigned int x)
{
	values[idx] = x;
	if (++idx == size) idx = 0u;
}

float MovingArray::Get(void* data, int x)
{

	return (float)(*(MovingArray*)data)[x];
}