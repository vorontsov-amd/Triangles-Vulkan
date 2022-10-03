#include <valarray>
#include "double.hpp"
#include <array>
#include <tuple>

namespace Geomitric
{
    struct Vector;
    struct Line;
    struct Triangle;
    struct Plane;
    struct Segment;

    using array = std::array<Double, 3>;

    bool operator||(const Plane& left, const Plane& right);
    Double operator*(const Vector& left, const Vector& right);
    Vector operator*(Double num, const Vector& vec);
    Vector operator*(const Vector& vec, Double num);
    Vector operator+(const Vector& left, const Vector& right);
    Vector operator-(const Vector& left, const Vector& right);
    Vector operator/(const Vector& vec, Double num);
    Vector cross(const Vector& left, const Vector& right);
    bool TrianglesIntersect(Triangle& first, Triangle& second);  
    bool TrianglesIntersect2D(Triangle& first, Triangle& second);
    array CalcDistance(const Triangle& triangle, const Plane& plane);
    Line LinePlaneIntersect(const Plane& first, const Plane& second);
    Triangle ProjectionToLine(const Triangle& triangle, const Line& line);
    void SortTrianglePoint(Vector& distance, Triangle& triangle, Triangle& projection);
    Segment CalcSegmentIntersect(Triangle& triangle, Vector& distances, Triangle& projections);
    bool SegmentIntersect(const Segment& seg_1, const Segment& seg_2);

    struct Vector
    {
    private:
        double clength = NAN;
    public:
        Double x = 0, y = 0, z = 0; 

        Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

        Vector ProjectonTo(const Vector& vec) const
        {
            return vec * ((*this * vec) / (vec * vec));
        }
    
        double length()
        {
            if (std::isnan(clength))
            {
                clength = std::sqrt(x * x + y * y + z * z);
            }
            return clength;
        }
    };

    inline Double operator*(const Vector& left, const Vector& right)
    {
        return Double {left.x * right.x + left.y * right.y + left.z * right.z};
    }

    inline Vector operator*(Double num, const Vector& vec)
    {
        return Vector {vec.x * num, vec.y * num, vec.z * num};
    }

    inline Vector operator*(const Vector& vec, Double num)
    {
        return Vector {vec.x * num, vec.y * num, vec.z * num};
    }

    inline Vector operator/(const Vector& vec, Double num)
    {
        return Vector {vec.x / num, vec.y / num, vec.z / num};
    }

    inline Vector operator+(const Vector& left, const Vector& right)
    {
        return Vector {left.x + right.x, left.y + right.y, left.z + right.z};
    }

    inline Vector operator-(const Vector& left, const Vector& right)
    {
        return Vector {left.x - right.x, left.y - right.y, left.z - right.z};
    }

    inline Vector cross(const Vector& left, const Vector& right)
    {
        return Vector {left.y * right.z - left.z * right.y,
                       left.x * right.z - left.z * right.x,
                       left.x * right.y - left.y * right.x};
    }

    struct Line
    {
        Vector entry = {0, 0, 0};
        Vector direction = {1, 0, 0};
    };

    struct Triangle
    {
        Vector P0 = {1, 0, 0};
        Vector P1 = {0, 1, 0};
        Vector P2 = {0, 0, 1};
    };

    struct Segment
    {
        Vector T1 = {1, 0, 0};
        Vector T2 = {0, 1, 0};
    };

    struct Plane
    {
        Double a = 1, b = 1, c = 1, d = 1;

        Plane(const Triangle& triangle)
        {
            a = (triangle.P0.y - triangle.P0.y) * (triangle.P2.z - triangle.P0.z) - 
                (triangle.P2.y - triangle.P0.y) * (triangle.P0.z - triangle.P0.z);

            b = (triangle.P0.x - triangle.P0.x) * (triangle.P2.z - triangle.P0.z) -
                (triangle.P2.x - triangle.P0.x) * (triangle.P0.z - triangle.P0.z);
    
            c = (triangle.P0.x - triangle.P0.x) * (triangle.P2.y - triangle.P0.y) -
                (triangle.P2.x - triangle.P0.x) * (triangle.P0.y - triangle.P0.y);
            
            d = - (triangle.P0.x * a + triangle.P0.y * b + triangle.P0.z * c); 
        }

        Vector normal() const { Vector n = {a, b, c}; return n / n.length(); }
    };

    inline bool operator||(const Plane& left, const Plane& right)
    {
        return left.a == right.a && left.b == right.b && left.c == right.c;
    }

}; // namespace Geomitric