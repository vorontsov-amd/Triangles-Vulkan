#pragma once
#include <cmath>
#include <algorithm>
#include <string>
#include <stdexcept>

inline bool isEqual(double a, double b) {
    const static double EPS = 1e-5;
    return std::abs(a - b) < EPS;
}
