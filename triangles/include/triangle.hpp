#pragma once
#include <valarray>
#include <array>
#include <tuple>
#include <iostream>
#include "vector.hpp"


namespace GeomObj
{
    struct Triangle final
    {
        //Data--------------------------------------------------------------------------
        Vector P0 = {1, 0, 0};
        Vector P1 = {0, 1, 0};
        Vector P2 = {0, 0, 1};

        enum { TRIANGLE = 0b1, POINT = 0b10, SEGMENT = 0b100 } status;
        
        int number = 0;
        //------------------------------------------------------------------------------

        //Constructors------------------------------------------------------------------
        Triangle() {}

        Triangle(const Vector& p0, const Vector& p1, const Vector& p2) : P0{p0}, P1{p1}, P2{p2} {
            if ((p0 == p1) and (p1 == p2) and (p2 == p0)) {
                status = POINT;
            }
            else if ((p0 != p1) and (p1 != p2) and (p2 != p0) and !isEqual(cross(P0, P1) * P2, 0)) {
                status = TRIANGLE;
            }
            else {
                status = SEGMENT;
            }
        }
        //------------------------------------------------------------------------------

        //Methods-----------------------------------------------------------------------
        Vector& operator[] (int num) {
            switch (num)
            {
            case 0:
                return P0;
            case 1:
                return P1;
            case 2:
                return P2;
            default:
                throw std::invalid_argument(std::to_string(num));
            }
        }

        const Vector& operator[] (int num) const {
            return const_cast<const Vector&> (const_cast<Triangle&>(*this)[num]);
        }

        double getAbsMaxCoord () const  {
            return std::max({P0.getAbsMaxCoord(), 
                             P1.getAbsMaxCoord(), 
                             P2.getAbsMaxCoord()});
        }
        //------------------------------------------------------------------------------
    };

}; // namespace GeomObj


std::ostream& operator<<(std::ostream& out, const GeomObj::Triangle& trian);
std::istream& operator>>(std::istream& in, GeomObj::Triangle& trian);