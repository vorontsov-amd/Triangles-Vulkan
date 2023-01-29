#include "triangle.hpp"
#include <vector>
#include <cstring>

namespace Geomitric
{
    std::vector<int> SquareMatrixLines(double matrix [][3], double column[]) {
        double tmp[3] = {0, 0, 0};

        Vector line_0 = {matrix[0][0], matrix[0][1], matrix[0][2]};
        Vector line_1 = {matrix[1][0], matrix[1][1], matrix[1][2]};
        Vector line_2 = {matrix[2][0], matrix[2][1], matrix[2][2]};
        Vector line_3 = {matrix[3][0], matrix[3][1], matrix[3][2]};


        if (!isEqual(determinant(line_0, line_1, line_2), 0)) {
            return {0, 1, 2};
        }
        else if (!isEqual(determinant(line_1, line_2, line_3), 0)) {
            return {1, 2, 3};
        }
        else if (!isEqual(determinant(line_0, line_2, line_3), 0)) {
            return {0, 2, 3};
        }
        else {
            return {0, 1, 3};
        }
    }

    Vector IntersectionPointOfTwoLines (const Vector &begin_1, const Vector &direct_1, const Vector &begin_2, const Vector &direct_2) {
        
        double matrix[4][3] = {
            {direct_1.y, -direct_1.x, 0},
            {0, direct_1.z, -direct_1.y},
            {direct_2.y, -direct_2.x, 0},
            {0, direct_2.z, -direct_1.y}
        };

        double column[4] = {
            direct_1.y * begin_1.x - direct_1.x * begin_1.y,
            direct_1.z * begin_1.y - direct_1.y * begin_1.z,
            direct_2.y * begin_2.x - direct_2.x * begin_2.y,
            direct_2.z * begin_2.y - direct_2.y * begin_2.z
        };

        std::vector<int> lines = SquareMatrixLines(matrix, column);
        
        Vector column_0 = {
            matrix[lines[0]][0],
            matrix[lines[1]][0],
            matrix[lines[2]][0]
        };

        Vector column_1 = {
            matrix[lines[0]][1],
            matrix[lines[1]][1],
            matrix[lines[2]][1]
        };

        Vector column_2 = {
            matrix[lines[0]][2],
            matrix[lines[1]][2],
            matrix[lines[2]][2]
        };

        Vector right_column = {
            column[lines[0]],
            column[lines[1]],
            column[lines[2]]
        };

        double det_0 = determinant(column_0, column_1, column_2);
        double detX  = determinant(right_column, column_1, column_2);
        double detY  = determinant(column_0, right_column, column_2);
        double detZ  = determinant(column_0, column_1, right_column);


        // std::cout << det_0 << " " << detX << "\n";

        double x     = detX / det_0;
        double y     = detY / det_0;
        double z     = detZ / det_0;

        // std::cout << det_0 << " " << detX << "\n";

        // double xVec = begin_1.x + x * segment_1.x;

        // std::cout << begin_1.x << " " << segment_1.x << '\n';

        // double yVec = begin_1.y + x * segment_1.y;
        // double zVec = begin_1.z + x * segment_1.z;

        Vector ret{x, y, z};

        //std::cout << ret << '\n';

        return ret;
    }


    bool CheckPointInSegment (const Vector &beginPVec, const Vector &directVec) {

        double vecLen = beginPVec.length();

        if (isEqual(vecLen, 0))
            return true;


        double directLen = directVec.length();
        double dir = (beginPVec * directVec) / (vecLen * directLen);

        if (isEqual(dir, 1.0) && directLen >= vecLen)
            return true;

        return false;
    }


    bool IntersectDegenerates (const Segment &segment, const Vector &point) {

        Vector beginVec     = segment.T1;
        Vector directVec    = segment.T2 - segment.T1;

        Vector pBeginVec    = point - beginVec;

        Vector crossProduct = cross(pBeginVec, directVec);
        if (crossProduct == Vector{0,0,0})
            return CheckPointInSegment (pBeginVec, directVec);

        return false;

    }

    bool IntersectSegments (const Segment &segment_1, const Segment &segment_2) {

        Vector begin_1 = segment_1.T1;
        Vector direct_1 = segment_1.T2;

        Vector begin_2 = segment_2.T1;
        Vector direct_2 = segment_2.T2;

        Vector cross_vec  = cross(direct_1,direct_2);
        Vector difVec = begin_2 - begin_1;

        //puts("lox_0");

        if (cross_vec == Vector{0,0,0}) {


            if ((cross((begin_2 - begin_1), direct_1) == Vector{0,0,0}))
            {
                if (IntersectDegenerates (segment_1, begin_2))
                    return true;
                if (IntersectDegenerates (segment_1, begin_2 + direct_2))
                    return true;
                if (IntersectDegenerates (segment_2, begin_1))
                    return true;
                if (IntersectDegenerates (segment_2, begin_1 + direct_1))
                    return true;

                return false;
            }
            return false;
        }

        // puts("lox_2");

        Vector a1 = direct_1 - begin_1;
        Vector a2 = direct_2 - begin_2;

        Vector interPoint = IntersectionPointOfTwoLines(begin_1, a1, begin_2, a2);

        //puts("lox_3");
        // std::cout << interPoint;
        return IntersectDegenerates(segment_1, interPoint) && IntersectDegenerates(segment_2, interPoint);
    }




    bool IntersectDegenerates (const Segment &segment1, const Segment &segment2) {

        Vector first  = segment1.T2 - segment1.T1;
        Vector second = segment2.T2 - segment2.T1;

        Vector connectingVec  = segment1.T1 - segment2.T1;

        double mixedProduct   = first * cross(second, connectingVec);

        if (isEqual(mixedProduct, 0.0))
            return IntersectSegments(segment1, segment2);

        //puts("false");

        return false;

    }



    bool LineTriangleIntersect(const Triangle& tri, const Line& line, Vector& intersection, double& info) {
        Vector e1, e2, p, s, q;
        double t, u, v, tmp;
        e1 = tri.P1 - tri.P0;
        e2 = tri.P2 - tri.P0;
        p = cross(line.direction, e2);
        tmp = p * e1;

        if (isEqual(tmp, 0)) {
            // TODO fix
            Segment seg{line.entry, line.entry + line.direction};

            Segment side;

            for (int i0 = 0, i1 = 2; i0 < 3; i1 = i0, i0++) {
                side.T1 = tri[i0];
                side.T2 = tri[i1];
                if (IntersectDegenerates(side, seg)) {
                    return true;
                }
            }

            return false;
            // Line entry {{0,0,0}, line.entry};
            // Line end   {{0,0,0}, line.entry + line.direction};
            // return LineTriangleIntersect(tri, entry, intersection, info) and (intersection == line.entry)
            //     or LineTriangleIntersect(tri, end, intersection, info) and (intersection == line.entry + line.direction);
        }

        tmp = 1 / tmp;
        s = line.entry - tri.P0;

        u = tmp * s * p;

        if (u < 0 || u > 1) {
            return false;
        }

        q = cross(s, e1);
        v = tmp * line.direction * q;

        if (v < 0 || v > 1) {
            return false;
        }

        t = tmp * e2 * q;
        intersection = line.entry + t * line.direction;
        info = t;
        //std::cout << intersection;
        return true;
    }


    bool HandleDegeneratedCase(const Triangle &tr1, const Triangle &tr2, int degFlag) {

        //There is a reason to ask why flag :   not degenerated (tr)    = 1 (0b1)
        //                                      point                   = 2 (0b10)
        //                                      segment                 = 4 (0b100)
        //
        //To clarify this let's build the table for the degFlag = tr1.getDegenerationType () | tr2.getDegenerationType ();
        //                  |   (0b1) tr    |   (0b10) point    | (0b100) segment
        // (0b1)   tr       |      0b10     |       0b11        |       0b101
        // (0b10)  point    |      0b11     |       0b100       |       0b110
        // (0b100) segment  |      0b101    |       0b110       |       0b1000
        //
        //As we can see, there's the only one value for every combination (up to permutation).

        enum {
            TRIANGLE_AND_TRIANGLE = 0b10,
            TRIANGLE_AND_POINT = 0b11,
            TRIANGLE_AND_SEGMENT = 0b101,
            SEGMENT_AND_SEGMENT = 0b1000,
            SEGMENT_AND_POINT = 0b110,
            POINT_AND_POINT = 0b100
        };

        //std::cout << degFlag;

        switch (degFlag) {


            case TRIANGLE_AND_POINT: {

                if (tr1.status == Triangle::POINT) { //tr1 is a point
                    Line line {{0,0,0}, tr1.P0};
                    Vector intersect;
                    double info = 0;
                    return (LineTriangleIntersect(tr2, line, intersect, info)) and (intersect == tr1.P0);
                }
                else {
                    Line line {{0,0,0}, tr2.P0};
                    Vector intersect;
                    double info = 0;
                    return (LineTriangleIntersect(tr1, line, intersect, info)) and (intersect == tr2.P0);
                }

            }
            case POINT_AND_POINT: {
                return tr1.P0 == tr2.P0;
            }
            case TRIANGLE_AND_SEGMENT: {

                //puts("HELP");

                if (tr1.status == Triangle::SEGMENT) { //tr1 is a segment
                    Segment seg{tr1};
                    Line line {seg.T1, seg.T2 - seg.T1};
                    Vector intersect;
                    double info = 0;
                    return (LineTriangleIntersect(tr2, line, intersect, info)) and (info <= 1);
                }
                else {
                    Segment seg{tr2};
                    Line line {seg.T1, seg.T2 - seg.T1};
                    Vector intersect;
                    double info = 0;
                    return (LineTriangleIntersect(tr1, line, intersect, info)) and (info <= 1);
                }

            }
            case SEGMENT_AND_POINT: {

                if (tr1.status == Triangle::POINT) //tr1 is a point
                {
                    Segment seg{tr2};
                    return IntersectDegenerates(seg, tr1.P0);
                }
                else
                {
                    Segment seg{tr1};
                    return IntersectDegenerates(seg, tr2.P0);
                }

            }
            case SEGMENT_AND_SEGMENT: {
                Segment seg1{tr1};
                Segment seg2{tr2};

                //std::cout << seg1 << '\n' << seg2;

                return IntersectDegenerates(seg1, seg2);
            }
            default:
                std::cout << "Unexpected bitmask in function " << (int) degFlag << " " << __func__ << std::endl;
                return false;
        }
    }

    bool TrianglesIntersect(Triangle& first, Triangle& second)
    {
        int degenereted_type = first.status + second.status;
        //std::cout << degenereted_type;
        if (degenereted_type != 2) {
            return HandleDegeneratedCase(first, second, degenereted_type);
        }

        Plane first_plane {first}, second_plane {second};

        // std::cout << "first " << first << "\t" << first_plane << "\n"
        //      << "second " << second << "\t" << second_plane << "\n";

        if (first_plane || second_plane)
        {
            //puts("par");
            if (!isEqual(first_plane.d, second_plane.d))
            {
                return false;
            }

            return TrianglesIntersect2D(first, second, first_plane.normal());
        }

        auto && first_distance = CalcDistance(first, second_plane);
        if (first_distance[0] < 0 and first_distance[1] < 0 and first_distance[2] < 0 or
            first_distance[0] > 0 and first_distance[1] > 0 and first_distance[2] > 0)
            {
                // std::cout << "distance_1 " << first_distance[0] << " "
                //                            << first_distance[1] << " "
                //                            << first_distance[2] << "\n";
                return false;
            }


        auto && second_distance = CalcDistance(second, first_plane);
        if (second_distance[0] < 0 and second_distance[1] < 0 and second_distance[2] < 0 or
            second_distance[0] > 0 and second_distance[1] > 0 and second_distance[2] > 0)
            {
                // std::cout << "distance 2" << second_distance[0] << second_distance[1] << second_distance[2];
                return false;
            }

        Line plane_intersection = LinePlaneIntersect(first, second);

        Triangle && first_project  = ProjectionToLine(first, plane_intersection);
        Triangle && second_project = ProjectionToLine(second, plane_intersection);

        Segment && first_segment  = CalcSegmentIntersect(first, first_distance, first_project);
        Segment && second_segment = CalcSegmentIntersect(second, second_distance, second_project);

        //puts("seg");
        return SegmentIntersect(first_segment, second_segment, plane_intersection);
    }

    Line LinePlaneIntersect(const Plane& first, const Plane& second)
    {
        Line result;

        const Vector& norm_1 = first.normal();
        const Vector& norm_2 = second.normal();

        Vector && direction = cross(norm_1, norm_2);

        result.direction = direction;

        double s1 = first.d, s2 = second.d;
        double n1n2 = norm_1 * norm_2;
        double n1normsqr = norm_1 * norm_1;
        double n2normsqr = norm_2 * norm_2;

        double a = (s2 * n1n2 - s1 * n2normsqr) / ((n1n2 * n1n2) - n1normsqr * n2normsqr);
        double b = (s1 * n1n2 - s2 * n2normsqr) / ((n1n2 * n1n2) - n1normsqr * n2normsqr);
        result.entry = a * norm_2 + b * norm_1;

        return result;
    }

    bool TrianglesIntersect2D(Triangle& first, Triangle& second, const Vector& normal)
    {
        auto max = maxComponent(normal);

        auto x = component_t::x, y = component_t::y;

        switch (max)
        {
        case component_t::x:
            x = component_t::y;
            y = component_t::z;
            break;
        case component_t::y:
            x = component_t::z;
            y = component_t::x;
            break;
        default:
            break;
        }

        return TestIntersection(first, second, x, y);
    }

    Triangle ProjectionToLine(const Triangle& triangle, const Line& line)
    {
        Vector&& V1 = line.entry + triangle.P0.ProjectonTo(line.direction);
        Vector&& V2 = line.entry + triangle.P1.ProjectonTo(line.direction);
        Vector&& V3 = line.entry + triangle.P2.ProjectonTo(line.direction);
        return Triangle {V1, V2, V3};
    }

    array CalcDistance(const Triangle& triangle, const Plane& plane)
    {
        Vector norm = plane.normal().normalize();
        double D = plane.d / plane.normal().length();

        return array { triangle.P0 * norm + D,
                       triangle.P1 * norm + D,
                       triangle.P2 * norm + D};
    }

    void SortTrianglePoint(array& distance, Triangle& triangle, Triangle& projection)
    {
        if (distance[0] <= 0.0 and distance[1] <= 0.0 and distance[2] > 0.0 or
            distance[0] >= 0.0 and distance[1] >= 0.0 and distance[2] < 0.0)
        {
            swap(distance[0],   distance[1],   distance[2],   rotate_t::left);
            swap(triangle.P0,   triangle.P1,   triangle.P2,   rotate_t::left);
            swap(projection.P0, projection.P1, projection.P2, rotate_t::left);
        }
        else if (distance[0] > 0.0 and distance[1] <= 0.0 and distance[2] <= 0.0 or
                 distance[0] < 0.0 and distance[1] >= 0.0 and distance[2] >= 0.0)
        {
            swap(distance[0],   distance[1],   distance[2],   rotate_t::right);
            swap(triangle.P0,   triangle.P1,   triangle.P2,   rotate_t::right);
            swap(projection.P0, projection.P1, projection.P2, rotate_t::right);
        }
    }

    Segment CalcSegmentIntersect(Triangle& triangle, array& distances, Triangle& projection)
    {
        SortTrianglePoint(distances, triangle, projection);

        const Vector&& P0 = projection.P0 + (projection.P1 - projection.P0) * distances[0] / (distances[0] - distances[1]);
        const Vector&& P1 = projection.P2 + (projection.P1 - projection.P2) * distances[2] / (distances[2] - distances[1]);

        return Segment {P0, P1};
    }


    bool SegmentIntersect(const Segment& seg_1, const Segment& seg_2, const Line& line)
    {
        auto max = maxComponent(line.direction);

        double T1_1 = seg_1.T1[max];
        double T1_2 = seg_1.T2[max];
        double T2_1 = seg_2.T1[max];
        double T2_2 = seg_2.T2[max];

        if (T1_1 > T1_2)
        {
            std::swap(T1_1, T1_2);
        }

        if (T2_1 > T2_2)
        {
            std::swap(T2_1, T2_2);
        }

        return  T1_1 <= T2_1 and T2_2 <= T1_2 or
                T2_1 <= T1_1 and T1_2 <= T2_2 or
                T1_1 <= T2_1 and T1_2 <= T2_2 or
                T2_1 <= T1_1 and T2_2 <= T1_2;

    }

    component_t maxComponent(const Vector& vec)
    {
        const auto x = std::abs(vec.x), y = std::abs(vec.y), z = std::abs(vec.z);
        const auto && max = std::max({x, y, z});

        if (isEqual(max, x)) return component_t::x;
        else if (isEqual(max, y)) return component_t::y;
        else return component_t::z;
    }

    bool TestIntersection(const Triangle& C0, const Triangle& C1, component_t x, component_t y)
    {
        double min0 = NAN, min1 = NAN, max0 = NAN, max1 = NAN;

        for (int i0 = 0, i1 = 2; i0 < 3; i1 = i0, i0++)
        {
            const Vector && side = C0[i0] - C0[i1];
            const Vector && D = perp(side, x, y);
            ComputeInterval(C0, D, min0, max0);
            ComputeInterval(C1, D, min1, max1);
            if (max1 < min0 || max0 < min1)
            {
                return false;
            }
        }

        for (int i0 = 0, i1 = 2; i0 < 3; i1 = i0, i0++)
        {
            Vector && side = C1[i0] - C1[i1];
            Vector && D = perp(side, x, y);
            ComputeInterval(C0, D, min0, max0);
            ComputeInterval(C1, D, min1, max1);
            if (max1 < min0 || max0 < min1)
            {
                return false;
            }
        }

        return true;
    }

    Vector perp(const Vector& side, component_t x, component_t y)
    {
        Vector res = {};
        res[x] = side[y];
        res[y] = -side[x];
        return res;
    }


    void ComputeInterval(const Triangle& triangle, const Vector& vec, double& min, double& max)
    {
        min = max = vec * triangle[0];
        for (int i = 1; i < 3; i++)
        {
            double value = vec * triangle[i];
            if (value < min)
                min = value;
            else if (value > max)
                max = value;
        }
    }
};
