#pragma once

template<typename T>
class TConfigVariable
{
public:
	TConfigVariable(T value) : currentValue(value)
	{
	}
	T Get()
	{
		return currentValue;
	}
	void Set(T value)
	{
		currentValue = value;
	}
private:
	T currentValue;
};