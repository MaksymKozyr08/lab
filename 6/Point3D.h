#pragma once
#include <iostream>
#include <cmath>

struct Point3D {
    double x, y, z;

    Point3D(double _x = 0, double _y = 0, double _z = 0) : x(_x), y(_y), z(_z) {}

    // Метод підрахунку відстані від початку координат
    double distance() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    // Для уникнення похибок з плаваючою комою краще порівнювати квадрати відстаней
    double distanceSq() const {
        return x*x + y*y + z*z;
    }

    // Перевантажені оператори для сортування як звичайних чисел
    bool operator<(const Point3D& other) const {
        double d1 = distanceSq();
        double d2 = other.distanceSq();
        if (d1 != d2) return d1 < d2;
        // якщо відстані однакові, сортуємо по координатах, щоб set не видаляв "дублікати"
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }

    bool operator>(const Point3D& other) const {
        return other < *this; // просто реюзаємо менше
    }

    bool operator==(const Point3D& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    // Зручний вивід у потік (перевантаження <<)
    friend std::ostream& operator<<(std::ostream& os, const Point3D& pt) {
        os << "(" << pt.x << ", " << pt.y << ", " << pt.z << ")";
        return os;
    }
};
