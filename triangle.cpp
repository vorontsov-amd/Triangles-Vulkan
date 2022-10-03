#include "triangle.hpp"

namespace Geomitric 
{
    bool TrianglesIntersect(Triangle& first, Triangle& second)
    {
        Plane first_plane {first}, second_plane {second};
        
        if (first_plane || second_plane)
        {
            if (first_plane.d != second_plane.d)
            {
                return false;
            }

            // do something
            return TrianglesIntersect2D(first, second);
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
            
        // Line plane_intersection = LinePlaneIntersect(first, second);
        // Triangle first_project  = ProjectionToLine(first, plane_intersection);
        // Triangle second_project = ProjectionToLine(second, plane_intersection);

        // Segment first_segment  = CalcSegmentIntersect(first, first_distance, first_project);   
        // Segment second_segment = CalcSegmentIntersect(second, second_distance, second_project);

        // return SegmentIntersect(first_segment, second_segment);  
    }

    Line LinePlaneIntersect(const Plane& first, const Plane& second)
    {
        Line result;
        Vector direction = cross(first.normal(), second.normal());
                
        result.direction = direction;

        Double s1 = first.d, s2 = second.d;

        Double n1n2 = first.normal() * second.normal();
        Double n1normsqr = first.normal()  * first.normal();
        Double n2normsqr = second.normal() * second.normal();

        Double a = (s2 * n1n2 - s1 * n2normsqr) / ((n1n2 ^ 2) - n1normsqr * n2normsqr);
        Double b = (s1 * n1n2 - s2 * n2normsqr) / ((n1n2 ^ 2) - n1normsqr * n2normsqr);
        result.entry = a * second.normal() + b * first.normal();

        return result;
    }

    bool TrianglesIntersect2D(Triangle& first, Triangle& second)
    {
        // do something
        std::cout << "LOL\n";
        abort();
    }

    Triangle ProjectionToLine(const Triangle& triangle, const Line& line)
    {
        Vector V1 = line.entry + triangle.P0.ProjectonTo(line.direction);
        Vector V2 = line.entry + triangle.P1.ProjectonTo(line.direction);
        Vector V3 = line.entry + triangle.P2.ProjectonTo(line.direction);
        return Triangle {V1, V2, V3};
    }

    array CalcDistance(const Triangle& triangle, const Plane& plane)
    {
        return array  {triangle.P0 * plane.normal() + plane.d,
                       triangle.P1 * plane.normal() + plane.d,
                       triangle.P2 * plane.normal() + plane.d};
    }  

    void SortTrianglePoint(Vector& distance, Triangle& triangle, Triangle& projection)
    {
        if (distance.x < 0 and distance.y < 0 and distance.z > 0 or
            distance.x > 0 and distance.y > 0 and distance.z < 0)
        {
            auto temp_point       = triangle.P1;
            auto temp_project     = projection.P1;
            auto temp_distance    = distance.y;

            triangle.P1   = triangle.P2;
            distance.y    = distance.z;
            projection.P1 = projection.P2;

            triangle.P2   = triangle.P0;
            distance.z    = distance.x;
            projection.P2 = projection.P0;

            triangle.P0   = temp_point;
            distance.x    = temp_distance;
            projection.P0 = temp_project;
        }                             
        else if (distance.x > 0 and distance.y < 0 and distance.z < 0 or
                 distance.x < 0 and distance.y > 0 and distance.z > 0)
        {
            auto temp_point       = triangle.P1;
            auto temp_project     = projection.P1;
            auto temp_distance    = distance.y;

            triangle.P1   = triangle.P0;
            distance.y    = distance.x;
            projection.P1 = projection.P0;

            triangle.P0   = triangle.P2;
            distance.x    = distance.z;
            projection.P0 = projection.P2;

            triangle.P2   = temp_point;
            distance.z    = temp_distance;
            projection.P2 = temp_project;  
        }            
    }

    Segment CalcSegmentIntersect(Triangle& triangle, Vector& distances, Triangle& projection)
    {
        SortTrianglePoint(distances, triangle, projection);

        Vector P0 = projection.P0 + (projection.P1 - projection.P0) * distances.x / (distances.x - distances.y); 
        Vector P1 = projection.P2 + (projection.P1 - projection.P2) * distances.z / (distances.z - distances.y); 

        return Segment {P0, P1};
    } 


    bool SegmentIntersect(const Segment& seg_1, const Segment& seg_2) 
    {
        double T1_1 = seg_1.T1.x;
        double T1_2 = seg_1.T2.x;
        double T2_1 = seg_2.T1.x;
        double T2_2 = seg_2.T2.x;

        if (T1_1 > T1_2)
        {
            auto temp = T1_1;
            T1_1 = T1_2;
            T1_2 = temp;
        }

        if (T2_1 > T2_2)
        {
            auto temp = T2_1;
            T2_1 = T2_2;
            T2_2 = temp;
        }

        return T1_1 <= T2_1 and T2_2 <= T1_2 ||
            T2_1 <= T1_1 and T1_2 <= T2_2 ||
            T1_1 <= T2_1 and T1_2 <= T2_2 ||
            T2_1 <= T1_1 and T2_2 <= T1_2; 
                
    }

};
