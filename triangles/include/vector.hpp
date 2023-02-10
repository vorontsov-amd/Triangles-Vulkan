#pragma once
#include <stdexcept>
#include "utility.hpp"
#include <iostream>

namespace GeomObj {
    struct Vector;

    enum component_t {
        x = 0, y = 1, z = 2,
    };
}


double operator*(const GeomObj::Vector& left, const GeomObj::Vector& right);
double operator*(const GeomObj::Vector& left, const GeomObj::Vector& right);
GeomObj::Vector operator*(double num, const GeomObj::Vector& vec);
GeomObj::Vector operator*(const GeomObj::Vector& vec, double num);
GeomObj::Vector operator+(const GeomObj::Vector& left, const GeomObj::Vector& right);
GeomObj::Vector operator-(const GeomObj::Vector& left, const GeomObj::Vector& right);
GeomObj::Vector operator-(const GeomObj::Vector& left);
GeomObj::Vector operator/(const GeomObj::Vector& vec, double num);
bool operator==(const GeomObj::Vector& left, const GeomObj::Vector& right);
bool operator!=(const GeomObj::Vector& left, const GeomObj::Vector& right);
GeomObj::Vector cross(const GeomObj::Vector& left, const GeomObj::Vector& right);
GeomObj::Vector perp(const GeomObj::Vector& side, GeomObj::component_t x, GeomObj::component_t y);
double determinant(const GeomObj::Vector& first, const GeomObj::Vector& second, const GeomObj::Vector& third);
std::istream& operator>>(std::istream& in, GeomObj::Vector& vec);
std::ostream& operator<<(std::ostream& out, const GeomObj::Vector& vec);
GeomObj::component_t AbsMaxCoordComponent(const GeomObj::Vector& vec);


namespace GeomObj {

    struct Vector final
    {
    public:
        //Data--------------------------------------------------------------------------
        double x = 0, y = 0, z = 0; 
        //------------------------------------------------------------------------------

        //Constructors------------------------------------------------------------------
        Vector() : Vector(0, 0, 0) {}
        Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
        //------------------------------------------------------------------------------

        //Methods-----------------------------------------------------------------------
        Vector ProjectonTo(const Vector& vec) const {
            return vec * ((*this * vec) / (vec * vec));
        }
    
        Vector& normalize() {
            double len = length();
            x /= len;
            y /= len;
            z /= len;
            return *this;
        }

        double squareLength() const {
            return x * x + y * y + z * z;
        }

        double length() const {
            return std::sqrt(squareLength());
        }

        double getAbsMaxCoord () const {
            return std::max({std::abs(x), std::abs(y), std::abs(z)});
        }

        Vector& operator/=(double num) {
            x /= num;
            y /= num;
            z /= num; 
            return *this; 
        }

        Vector& operator*=(double num) {
            x *= num;
            y *= num;
            z *= num; 
            return *this; 
        }

        Vector& operator-=(const Vector& vec) {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z; 
            return *this; 
        }

        Vector& operator+=(const Vector& vec) { 
            x += vec.x; 
            y += vec.y; 
            z += vec.z; 
            return *this;
        }


        const double& operator[] (int comp) const
        {
            switch (comp)
            {
            case component_t::x:
                return x;
            case component_t::y:
                return y;
            case component_t::z:
                return z;
            default:
                throw std::invalid_argument(std::to_string(comp));
            }
        }

        double& operator[] (int comp)
        {
            switch (comp)
            {
            case component_t::x:
                return x;
            case component_t::y:
                return y;
            case component_t::z:
                return z;
            default:
                throw std::invalid_argument(std::to_string(comp));
            }
        }

        //------------------------------------------------------------------------------
    };

};
