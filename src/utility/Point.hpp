/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <cassert>
#include <array>

namespace mc {

template<typename T, size_t N>
class Point {

 public:
                template<typename... Args>
    explicit    Point(Args... args);
    explicit    Point(std::initializer_list<T> l);
                Point(const Point<T, N>& other);

    Point<T, N>&    operator=(const Point<T, N>& other);
    Point<T, N>&    operator+=(const Point<T, N>& other);
    Point<T, N>&    operator-=(const Point<T, N>& other);
    Point<T, N>&    operator*=(const Point<T, N>& other);
    Point<T, N>&    operator/=(const Point<T, N>& other);
    T&              operator[](size_t index);
    const T&        operator[](size_t index) const;

    Point<T, N> operator+(const Point<T, N>& rhs) const;
    Point<T, N> operator-(const Point<T, N>& rhs) const;
    Point<T, N> operator*(const Point<T, N>& rhs) const;
    Point<T, N> operator/(const Point<T, N>& rhs) const;

    Point<T, N> operator/(T scalar) const;

    bool        operator<(const Point<T, N>& rhs) const;
    bool        operator==(const Point<T, N>& rhs) const;

 protected:
    std::array<T, N>    value;
};

//TODO type check of args elements
template<typename T, size_t N>
template<typename... Args>
Point<T, N>::Point(Args... args):
    value { { args... } } {
}

template<typename T, size_t N>
Point<T, N>::Point(const Point<T, N>& other):
    value { other.value } {
}

template<typename T, size_t N>
Point<T, N>& Point<T, N>::operator=(const Point<T, N>& other) {
    value = other.value;
    return *this;
}

template<typename T, size_t N>
Point<T, N>& Point<T, N>::operator+=(const Point<T, N>& other) {
    for (decltype(N) i = 0; i < N; i++) {
        value[i] += other[i];
    }
    return *this;
}

template<typename T, size_t N>
Point<T, N>& Point<T, N>::operator-=(const Point<T, N>& other) {
    for (decltype(N) i = 0; i < N; i++) {
        value[i] -= other[i];
    }
    return *this;
}

template<typename T, size_t N>
Point<T, N>& Point<T, N>::operator*=(const Point<T, N>& other) {
    for (decltype(N) i = 0; i < N; i++) {
        value[i] *= other[i];
    }
    return *this;
}

template<typename T, size_t N>
Point<T, N>& Point<T, N>::operator/=(const Point<T, N>& other) {
    for (decltype(N) i = 0; i < N; i++) {
        value[i] /= other[i];
    }
    return *this;
}

template<typename T, size_t N>
T& Point<T, N>::operator[](size_t index) {
    assert(index < N);
    return value[index];
}

template<typename T, size_t N>
const T& Point<T, N>::operator[](size_t index) const {
    assert(index < N);
    return value[index];
}

template<typename T, size_t N>
Point<T, N> Point<T, N>::operator+(const Point<T, N>& rhs) const {
    Point<T, N> result = *this;
    for (decltype(N) i = 0; i < N; i++) {
        result[i] += rhs[i];
    }
    return result;
}

template<typename T, size_t N>
Point<T, N> Point<T, N>::operator-(const Point<T, N>& rhs) const {
    Point<T, N> result = *this;
    for (decltype(N) i = 0; i < N; i++) {
        result[i] -= rhs[i];
    }
    return result;
}

template<typename T, size_t N>
Point<T, N> Point<T, N>::operator*(const Point<T, N>& rhs) const {
    Point<T, N> result = *this;
    for (decltype(N) i = 0; i < N; i++) {
        result[i] *= rhs[i];
    }
    return result;
}

template<typename T, size_t N>
Point<T, N> Point<T, N>::operator/(const Point<T, N>& rhs) const {
    Point<T, N> result = *this;
    for (decltype(N) i = 0; i < N; i++) {
        result[i] /= rhs[i];
    }
    return result;
}

template<typename T, size_t N>
Point<T, N> Point<T, N>::operator/(T scalar) const {
    Point<T, N> result = *this;
    for (decltype(N) i = 0; i < N; i++) {
        result[i] /= 2;
    }
    return result;
}

template<typename T, size_t N>
bool Point<T, N>::operator<(const Point<T, N>& rhs) const {
    for (decltype(N) i = 0; i < N; i++) {
        if (value[i] < rhs.value[i]) {
            return true;
        } else if (value[i] > rhs.value[i]) {
            return false;
        }
    }
    return false;
}

template<typename T, size_t N>
bool Point<T, N>::operator==(const Point<T, N>& rhs) const {
    for (decltype(N) i = 0; i < N; i++) {
        if (value[i] != rhs.value[i]) {
            return false;
        }
    }
    return true;
}

template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const Point<T, N>& point) {
    os << point[0];
    for (decltype(N) i = 1; i < N; i++) {
        os << "/" << point[i];
    }
    return os;
}




}       // namespace mc