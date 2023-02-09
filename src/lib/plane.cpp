#include "plane.hpp"

using namespace GeomObj;

std::ostream& operator<<(std::ostream& out, const Plane& plane) {
    return out << "a = " << plane.a << ", b = " << plane.b << ", c = " << plane.c << ", d = " << plane.d;
}


bool operator||(const Plane& left, const Plane& right) {
    return isEqual(cross(left.normal(), right.normal()).squareLength(), 0);
}