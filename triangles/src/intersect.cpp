#include "intersect.hpp"
#include "octree.hpp"
#include <cassert>
#include "triangle.hpp"
#include "segment.hpp"
#include "line.hpp"
#include "plane.hpp"


namespace GeomObj {

    namespace {

        void checkSubtree(Tree::OctreeNode* octree, GeomObj::Triangle& tr, std::vector<bool>& intersectTriangleFlagArray, int& SubtreeCounter) {

            for (int i  = 0; i < 8; ++i) {
                if (!octree->child[i])
                    continue;
                
                std::list <GeomObj::Triangle> List = octree->child[i]->listOfTriangles;
                
                for (Tree::OctreeNode::ListIt It = List.begin(); It != List.end(); ++It) {
                    bool addSubtreeCounter = GeomObj::IntersectTriangles(tr, *It);
                    SubtreeCounter += addSubtreeCounter;
                    
                    if(addSubtreeCounter) {
                        intersectTriangleFlagArray[It->number] = true;
                        intersectTriangleFlagArray[tr.number] = true;
                    }
                }

                checkSubtree(octree->child[i], tr, intersectTriangleFlagArray, SubtreeCounter);

            }

        }

    //-----------------------------------------------------------------------------------------------------

        int IntersectionCounter (Tree::OctreeNode* octree, std::vector<bool>& intersectTriangleFlagArray) {

            int counter = 0;

            if (octree == nullptr) {
                return counter;
            }

            std::list <GeomObj::Triangle> List = octree->listOfTriangles;
            
            for (Tree::OctreeNode::ListIt ItSlow = List.begin(); ItSlow != List.end(); ++ItSlow) {

                Tree::OctreeNode::ListIt curIt = ItSlow;
                ++curIt;
                for (Tree::OctreeNode::ListIt ItFast = curIt; ItFast != List.end(); ++ItFast) {
                    bool addCounter = GeomObj::IntersectTriangles(*ItSlow, *ItFast);
                    counter += addCounter;

                    if(addCounter) {
                        intersectTriangleFlagArray[ItFast->number] = true;
                        intersectTriangleFlagArray[ItSlow->number] = true;
                    }
                }

                checkSubtree(octree, *ItSlow, intersectTriangleFlagArray, counter);
                
            }

            for (int i = 0; i < 8; ++i) {
                if (!(octree->child[i]))
                    continue;

                counter += IntersectionCounter(octree->child[i], intersectTriangleFlagArray);

            }
            return counter;

        }
    };

    //---------------------------------------------------------------------------------------------------------

    int GetTriangles() {

        int countTriangles = 0;
        std::cin >> countTriangles;
        assert (std::cin.good());

        Tree::Octree mainRoot {};

        std::vector<GeomObj::Triangle> trianglesArr;
        GeomObj::Triangle tr;
        for (int i = 0; i < countTriangles; ++i) {
            std::cin >> tr;
            trianglesArr.push_back (tr);

        }

        mainRoot.push(trianglesArr);

        std::vector<bool> intersectTriangleFlagArray(countTriangles);
        int countIntersection = IntersectionCounter(mainRoot.getRoot(), intersectTriangleFlagArray);

        for (int i = 0; i < countTriangles; ++i)
            if (intersectTriangleFlagArray[i]) {
                std::cout << i << std::endl;
            }

        return countIntersection;
    }
};




namespace GeomObj
{
    namespace { 

        std::vector<int> SquareMatrixLines(double matrix [][3], double column[]);
        Vector IntersectionPointOfTwoLines(const Vector& begin_1, const Vector& direct_1, const Vector& begin_2, const Vector& direct_2);
        bool CheckPointInSegment(const Vector& beginPVec, const Vector& directVec);
        bool IntersectDegenerates(const Segment& segment, const Vector& point);
        bool IntersectSegments(const Segment& segment_1, const Segment& segment_2);
        bool IntersectDegenerates(const Segment& segment_1, const Segment& segment_2);
        bool IntersectLineTriangle(const Triangle& tri, const Line& line, Vector& intersection, double& info);
        bool IntersectDegenerates(const Triangle& tri, const Vector& point);
        bool IntersectDegenerates(const Triangle& tri, const Segment& seg);
        bool HandleDegeneratedCase(const Triangle& tr1, const Triangle& tr2, int degFlag);
        bool IntersectTriangles2D(const Triangle& first, const Triangle& second, const Vector& normal);
        Triangle ProjectionToLine(const Triangle& triangle, const Line& line);
        std::vector<double> CalcDistance(const Triangle& triangle, const Plane& plane);
        void SortTrianglePoint(std::vector<double>& distance, Triangle& triangle, Triangle& projection);
        bool TestIntersection(const Triangle& C0, const Triangle& C1, component_t x, component_t y);
        void ComputeInterval(const Triangle& triangle, const Vector& vec, double& min, double& max);
        Segment CalcSegmentIntersect(const Triangle& triangle, std::vector<double>& distances, const Triangle& projection);

    //---------------------------------------------------------------------------------- 
        std::vector<int> SquareMatrixLines(double matrix [][3], double column[]) {
            // double tmp[3] = {0, 0, 0};

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

    //----------------------------------------------------------------------------------

        Vector IntersectionPointOfTwoLines(const Vector& begin_1, const Vector& direct_1, const Vector& begin_2, const Vector& direct_2) {
            
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

            double x     = detX / det_0;
            double y     = detY / det_0;
            double z     = detZ / det_0;

            Vector ret{x, y, z};
            return ret;
        }

    //----------------------------------------------------------------------------------

        bool CheckPointInSegment(const Vector& beginPVec, const Vector& directVec) {

            double vecLen = beginPVec.length();

            if (isEqual(vecLen, 0))
                return true;

            double directLen = directVec.length();
            double dir = (beginPVec * directVec) / (vecLen * directLen);

            if (isEqual(dir, 1.0) && directLen >= vecLen)
                return true;

            return false;
        }

    //----------------------------------------------------------------------------------

        bool IntersectDegenerates(const Segment &segment, const Vector &point) {

            Vector beginVec     = segment.begin;
            Vector directVec    = segment.end - segment.begin;

            Vector pBeginVec    = point - beginVec;

            Vector crossProduct = cross(pBeginVec, directVec);
            if (crossProduct == Vector{0,0,0})
                return CheckPointInSegment (pBeginVec, directVec);

            return false;

        }

    //----------------------------------------------------------------------------------

        bool IntersectSegments(const Segment& segment_1, const Segment& segment_2) {

            Vector begin_1 = segment_1.begin;
            Vector direct_1 = segment_1.end - segment_1.begin;

            Vector begin_2 = segment_2.begin;
            Vector direct_2 = segment_2.end - segment_2.begin;

            Vector cross_vec  = cross(direct_1,direct_2);

            if (cross_vec == Vector{0,0,0}) {

                if ((cross((begin_2 - begin_1), direct_1) == Vector{0,0,0}))
                {
                    if (IntersectDegenerates(segment_1, begin_2))
                        return true;
                    if (IntersectDegenerates(segment_1, begin_2 + direct_2))
                        return true;
                    if (IntersectDegenerates(segment_2, begin_1))
                        return true;
                    if (IntersectDegenerates(segment_2, begin_1 + direct_1))
                        return true;

                    return false;
                }
                return false;
            }

            Vector interPoint = IntersectionPointOfTwoLines(begin_1, direct_1, begin_2, direct_2);

            return IntersectDegenerates(segment_1, interPoint) && IntersectDegenerates(segment_2, interPoint);
        }

    //----------------------------------------------------------------------------------

        bool IntersectDegenerates(const Segment& segment_1, const Segment& segment_2) {

            Vector first  = segment_1.end - segment_1.begin;
            Vector second = segment_2.end - segment_2.begin;

            Vector connectingVec  = segment_1.begin - segment_2.begin;

            double mixedProduct   = first * cross(second, connectingVec);

            if (isEqual(mixedProduct, 0.0))
                return IntersectSegments(segment_1, segment_2);

            return false;
        }

    //----------------------------------------------------------------------------------

        bool IntersectLineTriangle(const Triangle& tri, const Line& line, Vector& intersection, double& info) {
            
            Vector e1 = tri.P1 - tri.P0;
            Vector e2 = tri.P2 - tri.P0;
            Vector p = cross(line.direction, e2);
            
            double tmp = p * e1;

            if (isEqual(tmp, 0)) {
                Segment seg{line.entry, line.entry + line.direction};
                Segment side;

                for (int i0 = 0, i1 = 2; i0 < 3; i1 = i0, i0++) {
                    side.begin = tri[i0];
                    side.end = tri[i1];
                    if (IntersectDegenerates(side, seg)) {
                        return true;
                    }
                }
                return false;
            }

            tmp = 1 / tmp;
            Vector s = line.entry - tri.P0;

            double u = tmp * s * p;
            if (u < 0 || u > 1) {
                return false;
            }

            Vector q = cross(s, e1);
            double v = tmp * line.direction * q;
            if (v < 0 || v > 1) {
                return false;
            }

            double t = tmp * e2 * q;
            intersection = line.entry + t * line.direction;
            info = t;
            return true;
        }

    //----------------------------------------------------------------------------------

        bool IntersectDegenerates(const Triangle& tri, const Vector& point) {
            Line line {{0,0,0}, point};
            Vector intersect;
            double info = 0;
            return (IntersectLineTriangle(tri, line, intersect, info)) and (intersect == point);
        }

    //----------------------------------------------------------------------------------

        bool IntersectDegenerates(const Triangle& tri, const Segment& seg) {
            Line line {seg.begin, seg.end - seg.begin};
            Vector intersect;
            double info = 0;
            return (IntersectLineTriangle(tri, line, intersect, info)) and (info <= 1);        
        }
        
    //----------------------------------------------------------------------------------

        bool HandleDegeneratedCase(const Triangle& tr1, const Triangle& tr2, int degFlag) {

            enum DEGENERATED_CASES {
                TRIANGLE_AND_TRIANGLE = 0b10,
                TRIANGLE_AND_POINT    = 0b11,
                TRIANGLE_AND_SEGMENT  = 0b101,
                SEGMENT_AND_SEGMENT   = 0b1000,
                SEGMENT_AND_POINT     = 0b110,
                POINT_AND_POINT       = 0b100
            };

            switch (degFlag) {

                case TRIANGLE_AND_POINT: {

                    if (tr1.status == Triangle::POINT) { //tr1 is a point
                        Vector point = tr1.P0;
                        return IntersectDegenerates(tr2, point);
                    }
                    else {
                        Vector point = tr2.P0;
                        return IntersectDegenerates(tr1, point);
                    }

                }
                case POINT_AND_POINT: {
                    return tr1.P0 == tr2.P0;
                }
                case TRIANGLE_AND_SEGMENT: {

                    if (tr1.status == Triangle::SEGMENT) { //tr1 is a segment
                        Segment seg{tr1};
                        return IntersectDegenerates(tr2, seg);
                    }
                    else {
                        Segment seg{tr2};
                        return IntersectDegenerates(tr1, seg);
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
                    return IntersectDegenerates(seg1, seg2);
                }
                default:
                    std::cout << "Unexpected bitmask in function " << (int) degFlag << " " << __func__ << std::endl;
                    return false;
            }
        }

    //----------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------

        Line LinePlaneIntersect(const Plane& first, const Plane& second)
        {
            Line result;

            Vector norm_1 = first.normal();
            Vector norm_2 = second.normal();

            Vector direction = cross(norm_1, norm_2);

            result.direction = direction;

            double s1 = -first.d, s2 = -second.d;

            double n1n2 = norm_1 * norm_2;
            double n1normsqr = norm_1 * norm_1;
            double n2normsqr = norm_2 * norm_2;

            double a = (s2 * n1n2 - s1 * n2normsqr) / ((n1n2 * n1n2) - n1normsqr * n2normsqr);
            double b = (s1 * n1n2 - s2 * n1normsqr) / ((n1n2 * n1n2) - n1normsqr * n2normsqr);
            result.entry = a * norm_1 + b * norm_2;
            return result;
        }

    //----------------------------------------------------------------------------------

        bool IntersectTriangles2D(const Triangle& first, const Triangle& second, const Vector& normal) {
            auto max = AbsMaxCoordComponent(normal);

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

    //----------------------------------------------------------------------------------

        Triangle ProjectionToLine(const Triangle& triangle, const Line& line) {
        
            Vector V1 = line.entry + triangle.P0.ProjectonTo(line.direction);
            Vector V2 = line.entry + triangle.P1.ProjectonTo(line.direction);
            Vector V3 = line.entry + triangle.P2.ProjectonTo(line.direction);
            return Triangle {V1, V2, V3};
        }

    //----------------------------------------------------------------------------------

        std::vector<double> CalcDistance(const Triangle& triangle, const Plane& plane) {
            
            Vector norm = plane.normal();
                        
            double D = plane.d;

            return std::vector<double> { 
                (triangle.P0 * norm + D) / norm.length(),
                (triangle.P1 * norm + D) / norm.length(),
                (triangle.P2 * norm + D) / norm.length()
            };
        }

    //----------------------------------------------------------------------------------

        void SortTrianglePoint(std::vector<double>& distance, Triangle& triangle, Triangle& projection) {
            while (!((distance[0] <= 0.0 and distance[1] >= 0.0 and distance[2] >= 0.0) or
                (distance[0] >= 0.0 and distance[1] <= 0.0 and distance[2] <= 0.0)))
            {
                swap(distance[0],   distance[1],   distance[2],   rotate_t::left);
                swap(triangle.P0,   triangle.P1,   triangle.P2,   rotate_t::left);
                swap(projection.P0, projection.P1, projection.P2, rotate_t::left);
            }
        }

    //----------------------------------------------------------------------------------

        Segment CalcSegmentIntersect(const Triangle& triangle, std::vector<double>& distances, const Triangle& projection) {
           
            Triangle copy_tri = triangle;
            Triangle copy_proj = projection;

            SortTrianglePoint(distances, copy_tri, copy_proj);

            const Vector P0 = copy_proj.P0 + (copy_proj.P1 - copy_proj.P0) * std::abs(distances[0]) / (std::abs(distances[0]) + std::abs(distances[1]));
            const Vector P1 = copy_proj.P0 + (copy_proj.P2 - copy_proj.P0) * std::abs(distances[0]) / (std::abs(distances[0]) + std::abs(distances[2]));

            return Segment {P0, P1};
        }

    //----------------------------------------------------------------------------------

        bool TestIntersection(const Triangle& C0, const Triangle& C1, component_t x, component_t y) {
          
            double min0 = NAN, min1 = NAN, max0 = NAN, max1 = NAN;

            for (int i0 = 0, i1 = 2; i0 < 3; i1 = i0, i0++) {
                const Vector side = C0[i0] - C0[i1];
                const Vector D = perp(side, x, y);
                ComputeInterval(C0, D, min0, max0);
                ComputeInterval(C1, D, min1, max1);
                if (max1 < min0 || max0 < min1)
                {
                    return false;
                }
            }

            for (int i0 = 0, i1 = 2; i0 < 3; i1 = i0, i0++)
            {
                Vector side = C1[i0] - C1[i1];
                Vector D = perp(side, x, y);
                ComputeInterval(C0, D, min0, max0);
                ComputeInterval(C1, D, min1, max1);
                if (max1 < min0 || max0 < min1)
                {
                    return false;
                }
            }

            return true;
        }

    //----------------------------------------------------------------------------------

        void ComputeInterval(const Triangle& triangle, const Vector& vec, double& min, double& max) {
          
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
    //----------------------------------------------------------------------------------

    };
};


namespace GeomObj {
    bool IntersectTriangles(const Triangle& first, const Triangle& second) {
        
        int degenereted_type = first.status + second.status;
        if (degenereted_type != 2) {
            return HandleDegeneratedCase(first, second, degenereted_type);
        }

        Plane first_plane {first}, second_plane {second};

        if (first_plane || second_plane) {
            
            if (!isEqual(first_plane.a / first_plane.d, second_plane.a / second_plane.d)) {
                return false;
            }

            return IntersectTriangles2D(first, second, first_plane.normal());
        }

        auto first_distance = CalcDistance(first, second_plane);
        if ((first_distance[0] < 0 and first_distance[1] < 0 and first_distance[2] < 0) or
            (first_distance[0] > 0 and first_distance[1] > 0 and first_distance[2] > 0)) {
                return false;
            }

        auto second_distance = CalcDistance(second, first_plane);
        if ((second_distance[0] < 0 and second_distance[1] < 0 and second_distance[2] < 0) or
            (second_distance[0] > 0 and second_distance[1] > 0 and second_distance[2] > 0)) {
                return false;
            }

        Line plane_intersection = LinePlaneIntersect(first, second);
        
        Triangle first_project  = ProjectionToLine(first, plane_intersection);
        Triangle second_project = ProjectionToLine(second, plane_intersection);
        
        Segment first_segment = CalcSegmentIntersect(first, first_distance, first_project);
        Segment second_segment = CalcSegmentIntersect(second, second_distance, second_project);

        return IntersectSegments(first_segment, second_segment);//SegmentIntersect(fs, ss, plane_intersection);
    }

};


