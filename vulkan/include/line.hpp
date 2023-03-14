#pragma once
#include "vector.hpp"


namespace GeomObj {
    struct Line final
    {
        Vector entry = {0, 0, 0};
        Vector direction = {1, 0, 0};
    };
};


std::ostream& operator<<(std::ostream& out, GeomObj::Line& line);

