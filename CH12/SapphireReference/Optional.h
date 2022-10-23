#pragma once

template<typename T>
class Optional
{
public:
    bool IsPresent()
    {
        return isPresent;
    }
    void Set(T value)
    {
        this->value = value;
        isPresent = true;
    }
    T Get()
    {
        return value;
    }
private:
    bool isPresent = false;
    T value;
};