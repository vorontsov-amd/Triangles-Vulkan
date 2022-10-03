#include "triangle.hpp"

int main()
{
    Geomitric::Triangle fir {
        Geomitric::Vector {0,0,0},
        Geomitric::Vector {1,1,0},
        Geomitric::Vector {0,1,1},
    };

    Geomitric::Triangle sec {
        Geomitric::Vector {0,0,0},
        Geomitric::Vector {1,0,0},
        Geomitric::Vector {0.5, 2, 0.5},
    };

    std::cout << std::boolalpha << TrianglesIntersect(fir, sec);
}
