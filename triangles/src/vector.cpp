#include "vector.hpp"
#include <fstream>

using namespace GeomObj;

std::ostream& operator<<(std::ostream& out, const Vector& vec) {
    return out << "{ " << vec.x << ", " << vec.y << ", " << vec.z << " }";
}

std::istream& operator>>(std::istream& in, Vector& vec) {
    return in >> vec.x >> vec.y >> vec.z;
}

double determinant(const Vector &first, const Vector &second, const Vector &third) {
    return (first.x * second.y * third.z - 
            first.x * second.z * third.y -
            first.y * second.x * third.z +
            first.y * second.z * third.x +
            first.z * second.x * third.y -
            first.z * second.y * third.x);
}

double operator*(const Vector& left, const Vector& right) {
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

Vector operator*(double num, const Vector& vec) {
    Vector ret = vec;
    return ret *= num;
}

Vector operator*(const Vector& vec, double num) {
    return num * vec;
}

Vector operator/(const Vector& vec, double num) {
    Vector ret = vec;
    return ret /= num;
}

Vector operator+(const Vector& left, const Vector& right) {
    Vector ret = left;
    return ret += right;
}

Vector operator-(const Vector& left, const Vector& right) {
    Vector ret = left;
    return ret -= right;
}

Vector operator-(const Vector& left) {
    Vector ret = left;
    return ret *= -1;
}


bool operator==(const Vector& left, const Vector& right) {
    return isEqual(left.x,right.x) and isEqual(left.y,right.y) and isEqual(left.z,right.z);
}

bool operator!=(const Vector& left, const Vector& right) {
    return !(left == right);
}

Vector cross(const Vector& left, const Vector& right) {
    return Vector { left.y * right.z - left.z * right.y,
                   -left.x * right.z + left.z * right.x,
                    left.x * right.y - left.y * right.x };
}


