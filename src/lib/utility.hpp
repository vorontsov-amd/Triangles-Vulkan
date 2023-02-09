#pragma once
#include <cmath>
#include <algorithm>
#include <string>
#include <stdexcept>

inline bool isEqual(double a, double b) {
    const static double EPS = 1e-5;
    return std::abs(a - b) < EPS;
}

namespace GeomObj {
    enum rotate_t {
        left,
        right,
    };
};

template <typename T>
void swap(T& first, T& second, T& third, int rotate) {
    auto temp = second;
    switch (rotate)
    {
    case GeomObj::rotate_t::left:
        second = third;
        third  = first;
        first  = temp;
        break;
    case GeomObj::rotate_t::right:
        second = first;
        first  = third;
        third  = temp;
        break;
    default:
        throw std::invalid_argument(std::to_string(rotate));
    }
} 