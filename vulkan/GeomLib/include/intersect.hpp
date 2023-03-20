#pragma once
#include "triangle.hpp"
#include <vector>

namespace GeomObj {
    bool IntersectTriangles(const Triangle& first, const Triangle& second);
    std::pair<std::vector<Triangle>, std::vector<bool>> GetTriangles();
}


