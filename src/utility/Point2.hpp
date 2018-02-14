/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace mc {

template<class T>
class Point2 {

 public:
                Point2(T x, T y);
    virtual     ~Point2() = default;

    void        SetX(T newX);
    void        SetY(T newY);

    T           GetX() const;
    T           GetY() const;

    Point2<T>   operator+(const Point2<T>& rhs) const;
    Point2<T>&  operator+=(const Point2<T>& rhs);
    Point2<T>&  operator=(const Point2<T>& rhs);
    T&          operator[](uint8_t index);
    const T&    operator[](uint8_t index) const;

    bool        operator<(const Point2<T>& rhs) const;
 protected:
    T           value[2];
};

template<class T>
std::ostream& operator<<(std::ostream& os, const Point2<T>& p) {
    os << p[0] << "/" << p[1];
    return os;
}

template<class T>
inline void Point2<T>::SetX(T newX) {
    value[0] = newX;
}
template<class T>
inline void Point2<T>::SetY(T newY) {
    value[1] = newY;
}

template<class T>
inline T Point2<T>::GetX() const {
    return value[0];
}

template<class T>
inline T Point2<T>::GetY() const {
    return value[1];
}

template<class T>
Point2<T>::Point2(T x, T y):
    value { x, y } {

}

template<class T>
Point2<T> Point2<T>::operator+(const Point2<T>& rhs) const {
    return Point2<T>(value[0] + rhs.value[0],
                     value[1] + rhs.value[1]);
}

template<class T>
Point2<T>& Point2<T>::operator+=(const Point2<T>& rhs) {
    for (int i = 0; i < 2; i++) {
        value[i] += rhs.value[i];
    }
    return *this;
}

template<class T>
Point2<T>& Point2<T>::operator=(const Point2<T>& rhs) {
    for (int i = 0; i < 2; i++) {
        value[i] = rhs.value[i];
    }
    return *this;
}

template<class T>
T& Point2<T>::operator[](uint8_t index) {
    if (index < 2) {
        return value[index];
    }
    throw std::out_of_range("Point2 index must be between 0 and 1");
}

template<class T>
const T& Point2<T>::operator[](uint8_t index) const {
    if (index < 2) {
        return value[index];
    }
    throw std::out_of_range("Point3 index must be between 0 and 1");
}

template<class T>
bool Point2<T>::operator<(const Point2<T>& rhs) const {
    for (int i = 0; i < 2; i++) {
        if (value[i] < rhs.value[i]) {
            return true;
        } else if (value[i] > rhs.value[i]) {
            return false;
        }
    }
    return false;
}


}       // namespace mc
