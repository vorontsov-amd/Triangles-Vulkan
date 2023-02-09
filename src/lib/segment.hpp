#pragma once
#include "vector.hpp"
#include "triangle.hpp"

namespace GeomObj {
    struct Segment final
    {
        //Data--------------------------------------------------------------------------
        Vector begin = {1, 0, 0};
        Vector end   = {0, 1, 0};
        //------------------------------------------------------------------------------

        //Constructors------------------------------------------------------------------
        Segment() {}

        Segment(const Vector begin_, const Vector& end_) : begin{begin_}, end{end_} {}

        Segment(const Triangle& tri) {
            if (tri.status != Triangle::SEGMENT) {
                throw std::logic_error("Triangle is not a segment");
            }

            begin = tri.P0;

            if (tri.P1 == tri.P0) {
                end = tri.P2;
            }
            else if (tri.P2 == tri.P0) {
                end = tri.P1;
            }
            else if (tri.P2 == tri.P1) {
                end = tri.P2;
            }
            else {
                Vector candidat_1 = tri.P1 - tri.P0;
                Vector candidat_2 = tri.P2 - tri.P0;
                Vector candidat_3 = tri.P2 - tri.P1;

                if (candidat_1.squareLength() > candidat_2.squareLength() and
                    candidat_1.squareLength() > candidat_3.squareLength()) {
                        begin = tri.P0;
                        end = tri.P1;
                    }
                else if (candidat_2.squareLength() > candidat_1.squareLength() and
                    candidat_2.squareLength() > candidat_3.squareLength()) {
                        begin = tri.P0;
                        end = tri.P2;
                    }
                else {
                    begin = tri.P1;
                    end = tri.P2;
                }
            }
        }
    };
};
