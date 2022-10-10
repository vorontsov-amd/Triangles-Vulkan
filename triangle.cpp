#include "triangle.hpp"

namespace Geomitric 
{
    bool TrianglesIntersect(Triangle& first, Triangle& second)
    {
        Plane first_plane {first}, second_plane {second};
        
        if (first_plane || second_plane)
        {
            if (!isEqual(first_plane.d, second_plane.d))
            {
                return false;
            }
            return TrianglesIntersect2D(first, second, first_plane.normal());
        }

        auto first_distance = CalcDistance(first, second_plane);
        if (first_distance[0] < 0 and first_distance[1] < 0 and first_distance[2] < 0 or
            first_distance[0] > 0 and first_distance[1] > 0 and first_distance[2] > 0)
            {
                return false;
            }

        auto second_distance = CalcDistance(second, first_plane);
        if (second_distance[0] < 0 and second_distance[1] < 0 and second_distance[2] < 0 or
            second_distance[0] > 0 and second_distance[1] > 0 and second_distance[2] > 0)
            {
                return false;
            }
            
        Line plane_intersection = LinePlaneIntersect(first, second);

        Triangle && first_project  = ProjectionToLine(first, plane_intersection);
        Triangle && second_project = ProjectionToLine(second, plane_intersection);

        Segment && first_segment  = CalcSegmentIntersect(first, first_distance, first_project);   
        Segment && second_segment = CalcSegmentIntersect(second, second_distance, second_project);

        return SegmentIntersect(first_segment, second_segment, plane_intersection);  
    }

    Line LinePlaneIntersect(const Plane& first, const Plane& second)
    {
        Line result;

        const Vector & norm_1 = first.normal();  
        const Vector & norm_2 = second.normal();              

        Vector && direction = cross(norm_1, norm_2);
                
        result.direction = direction;

        const double & s1 = first.d, & s2 = second.d;

        double && n1n2 = norm_1 * norm_2;
        double && n1normsqr = norm_1 * norm_1;
        double && n2normsqr = norm_2 * norm_2;

        double && a = (s2 * n1n2 - s1 * n2normsqr) / ((n1n2 * n1n2) - n1normsqr * n2normsqr);
        double && b = (s1 * n1n2 - s2 * n2normsqr) / ((n1n2 * n1n2) - n1normsqr * n2normsqr);
        result.entry = a * norm_2 + b * norm_1;

        return result;
    }

    bool TrianglesIntersect2D(Triangle& first, Triangle& second, const Vector& normal)
    {
        auto && max = maxComponent(normal);

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
        Vector && V1 = line.entry + triangle.P0.ProjectonTo(line.direction);
        Vector && V2 = line.entry + triangle.P1.ProjectonTo(line.direction);
        Vector && V3 = line.entry + triangle.P2.ProjectonTo(line.direction);
        return Triangle {V1, V2, V3};
    }

    array CalcDistance(const Triangle& triangle, const Plane& plane)
    {
        const Vector & norm = plane.normal().normalize();

        return array  {triangle.P0 * norm + plane.d,
                       triangle.P1 * norm + plane.d,
                       triangle.P2 * norm + plane.d};
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
        else if (distance[0] > 0.0 and distance[1] <= 0.0 and distance[3] <= 0.0 or
                 distance[0] < 0.0 and distance[1] >= 0.0 and distance[3] >= 0.0)
        {
            swap(distance[0],   distance[1],   distance[2],   rotate_t::right);
            swap(triangle.P0,   triangle.P1,   triangle.P2,   rotate_t::right);
            swap(projection.P0, projection.P1, projection.P2, rotate_t::right);
        }            
    }

    Segment CalcSegmentIntersect(Triangle& triangle, array& distances, Triangle& projection)
    {
        SortTrianglePoint(distances, triangle, projection);

        const Vector && P0 = projection.P0 + (projection.P1 - projection.P0) * distances[0] / (distances[0] - distances[1]); 
        const Vector && P1 = projection.P2 + (projection.P1 - projection.P2) * distances[2] / (distances[2] - distances[1]); 

        return Segment {P0, P1};
    } 


    bool SegmentIntersect(const Segment& seg_1, const Segment& seg_2, const Line& line) 
    {
        auto && max = maxComponent(line.direction);
        
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
        const auto &x = vec.x, &y = vec.y, &z = vec.z; 
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
                return false;
        }

        for (int i0 = 0, i1 = 2; i0 < 3; i1 = i0, i0++) 
        {
            Vector && side = C1[i0] - C1[i1];
            Vector && D = perp(side, x, y);
            ComputeInterval(C0, D, min0, max0);
            ComputeInterval(C1, D, min1, max1);
            if (max1 < min0 || max0 < min1)
                return false;
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
