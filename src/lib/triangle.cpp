#include "triangle.hpp"
#include <vector>
#include <cstring>


using namespace GeomObj;

std::ostream& operator<<(std::ostream& out, const Triangle& trian) {
    return out << trian.P0 << "\t" << trian.P1 << "\t" << trian.P2;
}

std::istream& operator>>(std::istream& in, Triangle& trian) {
    Vector vec1, vec2, vec3;
    in >> vec1 >> vec2 >> vec3;

    Triangle ret{vec1, vec2, vec3};
    trian = ret;

    return in;
}



