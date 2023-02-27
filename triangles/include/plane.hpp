#pragma once
#include "utility.hpp"
#include "vector.hpp"
#include "triangle.hpp"

namespace GeomObj {
    struct Plane final
    {
        //Data--------------------------------------------------------------------------
        double a = 1, b = 1, c = 1, d = 1;
        //------------------------------------------------------------------------------

        //Constructors------------------------------------------------------------------
        Plane(const Triangle& triangle) {
            auto normal = cross(triangle.P1 - triangle.P0, triangle.P2 - triangle.P0);
            a = normal.x; b = normal.y; c = normal.z; d = - normal * triangle.P0;
        }  
        //------------------------------------------------------------------------------

        //Methods-----------------------------------------------------------------------
        Vector normal() const { 
            return Vector {a, b, c}; 
        }
        //------------------------------------------------------------------------------
    
        Plane& operator/=(double num) {
            a /= num;
            b /= num;
            c /= num;
            d /= num;
            return *this;
        }

        Plane& operator*=(double num) {
            a *= num;
            b *= num;
            c *= num;
            d *= num;
            return *this;
        }
    
    };

};


std::ostream& operator<<(std::ostream& out, const GeomObj::Plane& plane);
bool operator||(const GeomObj::Plane& left, const GeomObj::Plane& right);
