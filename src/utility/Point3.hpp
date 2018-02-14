/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace mc {

template<class T>
class Point3 {

 public:
                Point3(T x, T y, T z);
    virtual     ~Point3() = default;

    void        SetX(T newX);
    void        SetY(T newY);
    void        SetZ(T newZ);

    T           GetX() const;
    T           GetY() const;
    T           GetZ() const;

    Point3<T>   operator+(const Point3<T>& rhs) const;
    Point3<T>&  operator+=(const Point3<T>& rhs);
    Point3<T>&  operator=(const Point3<T>& rhs);
    T&          operator[](uint8_t index);
    const T&    operator[](uint8_t index) const;

    bool        operator<(const Point3<T>& rhs) const;

 protected:
    T           value[3];
};

template<class T>
std::ostream& operator<<(std::ostream& os, const Point3<T>& p) {
    os << p[0] << "/" << p[1] << "/" << p[2];
    return os;
}

template<class T>
inline void Point3<T>::SetX(T newX) {
    value[0] = newX;
}
template<class T>
inline void Point3<T>::SetY(T newY) {
    value[1] = newY;
}
template<class T>
inline void Point3<T>::SetZ(T newZ) {
    value[2] = newZ;
}

template<class T>
inline T Point3<T>::GetX() const {
    return value[0];
}

template<class T>
inline T Point3<T>::GetY() const {
    return value[1];
}

template<class T>
inline T Point3<T>::GetZ() const {
    return value[2];
}

template<class T>
Point3<T>::Point3(T x, T y, T z):
    value { x, y, z } {

}

template<class T>
Point3<T> Point3<T>::operator+(const Point3<T>& rhs) const {
    return Point3<T>(value[0] + rhs.value[0],
                     value[1] + rhs.value[1],
                     value[2] + rhs.value[2]);
}

template<class T>
Point3<T>& Point3<T>::operator+=(const Point3<T>& rhs) {
    for (int i = 0; i < 3; i++) {
        value[i] += rhs.value[i];
    }
    return *this;
}

template<class T>
Point3<T>& Point3<T>::operator=(const Point3<T>& rhs) {
    for (int i = 0; i < 3; i++) {
        value[i] = rhs.value[i];
    }
    return *this;
}

template<class T>
T& Point3<T>::operator[](uint8_t index) {
    if (index < 3) {
        return value[index];
    }
    throw std::out_of_range("Point3 index must be between 0 and 2");
}

template<class T>
const T& Point3<T>::operator[](uint8_t index) const {
    if (index < 3) {
        return value[index];
    }
    throw std::out_of_range("Point3 index must be between 0 and 2");
}

template<class T>
bool Point3<T>::operator<(const Point3<T>& rhs) const {
    for (int i = 0; i < 3; i++) {
        if (value[i] < rhs.value[i]) {
            return true;
        } else if (value[i] > rhs.value[i]) {
            return false;
        }
    }
    return false;
}

}       // namespace mc
