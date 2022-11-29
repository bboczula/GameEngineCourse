#pragma once

#include <cassert>
 
template<typename TElement, unsigned int MAX_SIZE>
class TFixedArray
{
public:
	void PushBack(TElement element)
	{
		assert(capacity < MAX_SIZE);
		m_elements[capacity] = element;
		capacity++;
	}
	TElement& operator[](unsigned int i)
	{
		assert(i < MAX_SIZE);
		return m_elements[i];
	}
	TElement m_elements[MAX_SIZE];
	unsigned int capacity;
};