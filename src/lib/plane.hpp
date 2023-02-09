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
        Plane(const Triangle& triangle) :
            a{(triangle.P1.y - triangle.P0.y) * (triangle.P2.z - triangle.P0.z) - 
              (triangle.P2.y - triangle.P0.y) * (triangle.P1.z - triangle.P0.z)},

            b{-(triangle.P1.x - triangle.P0.x) * (triangle.P2.z - triangle.P0.z) +
               (triangle.P2.x - triangle.P0.x) * (triangle.P1.z - triangle.P0.z)},
    
            c{(triangle.P1.x - triangle.P0.x) * (triangle.P2.y - triangle.P0.y) -
              (triangle.P2.x - triangle.P0.x) * (triangle.P1.y - triangle.P0.y)},
            
            d{-(triangle.P0.x * a + triangle.P0.y * b + triangle.P0.z * c)} {

                if (d != 0) {
                    a /= d;
                    b /= d;
                    c /= d;
                    d  = 1;
                } 
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
