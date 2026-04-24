#pragma once
#include <string>

// Шаблонний інтерфейс з базовими віртуальними методами
template <typename T>
class IOrderedList {
public:
    virtual ~IOrderedList() {}
    virtual void insert(const T& val) = 0;
    virtual void clear() = 0;
    virtual std::string toJson() const = 0; // для візуалізації (замість print)
};
