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
    enum class rotate_t;
    enum class component_t;

    using array = std::array<Double, 3>;

    bool operator||(const Plane& left, const Plane& right);
    Double operator*(const Vector& left, const Vector& right);
    Vector operator*(const Double& num, const Vector& vec);
    Vector operator*(const Vector& vec, const Double& num);
    Vector operator+(const Vector& left, const Vector& right);
    Vector operator-(const Vector& left, const Vector& right);
    Vector operator-(Vector&& left, Vector&& right);
    Vector operator/(const Vector& vec, const Double& num);
    Vector operator/(Vector&& vec, Double&& num);
    Vector cross(const Vector& left, const Vector& right);
    bool TrianglesIntersect(Triangle& first, Triangle& second);  
    bool TrianglesIntersect2D(Triangle& first, Triangle& second, const Vector& normal);
    array CalcDistance(const Triangle& triangle, const Plane& plane);
    Line LinePlaneIntersect(const Plane& first, const Plane& second);
    Triangle ProjectionToLine(const Triangle& triangle, const Line& line);
    void SortTrianglePoint(array& distance, Triangle& triangle, Triangle& projection);
    Segment CalcSegmentIntersect(Triangle& triangle, array& distances, Triangle& projections);
    bool SegmentIntersect(const Segment& seg_1, const Segment& seg_2, const Line& line);
    component_t maxComponent(const Vector& vec);
    bool TestIntersection(const Triangle& C0, const Triangle& C1, component_t x, component_t y);
    Vector perp(const Vector& side, component_t x, component_t y);
    void ComputeInterval(const Triangle& triangle, const Vector& vec, double& min, double& max);


    template <typename T> 
    void swap(T& first, T& second, T& third, rotate_t rotate);

    enum class component_t
    {
        x, y, z,
    };

    struct Vector
    {
    private:
        double clength = NAN;
    public:
        Double x = 0, y = 0, z = 0; 

        Vector() : Vector(0, 0, 0) {}
        Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}

        Vector ProjectonTo(const Vector& vec) const
        {
            return vec * ((*this * vec) / (vec * vec));
        }
    
        Vector& normalize()
        {
            double len = length();
            x /= len;
            y /= len;
            z /= len;
            return *this;
        }

        double length()
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        Vector& operator/=(double num) { x /= num; y /= num; z /= num; return *this; }
        Vector& operator-=(const Vector& vec) { x -= vec.x; y -= vec.y; z -= vec.z; return *this; }
        Vector& operator+=(const Vector& vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }

        friend std::ostream& operator<<(std::ostream& out, const Vector& vec)
        {
            return out << "{ " << vec.x << ", " << vec.y << ", " << vec.z << " }";
        }

        friend std::istream& operator>>(std::istream& in, Vector& vec)
        {
            return in >> vec.x >> vec.y >> vec.z;
        }

        const Double& operator[] (component_t comp) const
        {
            switch (comp)
            {
            case component_t::x:
                return x;
                break;
            case component_t::y:
                return y;
                break;
            case component_t::z:
                return z;
                break;
            default:
                break;
            }
            throw "wtf";
        }

        Double& operator[] (component_t comp)
        {
            switch (comp)
            {
            case component_t::x:
                return x;
                break;
            case component_t::y:
                return y;
                break;
            case component_t::z:
                return z;
                break;
            default:
                break;
            }
            throw "wtf";
        }
    };

    inline Double operator*(const Vector& left, const Vector& right)
    {
        return Double {left.x * right.x + left.y * right.y + left.z * right.z};
    }

    inline Vector operator*(const Double& num, const Vector& vec)
    {
        return Vector {vec.x * num, vec.y * num, vec.z * num};
    }

    inline Vector operator*(const Vector& vec, const Double& num)
    {
        return Vector {vec.x * num, vec.y * num, vec.z * num};
    }

    inline Vector operator/(const Vector& vec, const Double& num)
    {
        return Vector {vec.x / num, vec.y / num, vec.z / num};
    }

    inline Vector operator/(Vector&& vec, Double&& num)
    {
        return vec /= num;
    }

    inline Vector operator+(const Vector& left, const Vector& right)
    {
        return Vector {left.x + right.x, left.y + right.y, left.z + right.z};
    }

    inline Vector operator+(Vector& left, Vector& right)
    {
        return left += right;
    }

    inline Vector operator-(const Vector& left, const Vector& right)
    {
        return Vector {left.x - right.x, left.y - right.y, left.z - right.z};
    }

    inline Vector operator-(Vector&& left, Vector&& right)
    {
        return left -= right;
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

        const Vector& operator[] (int num) const
        {
            switch (num)
            {
            case 0:
                return P0;
                break;
            case 1:
                return P1;
                break;
            case 2:
                return P2;
                break;
            default:
                break;
            }
            throw "LOL";
        }

        // Vector side (int num) const
        // {
        //     switch (num)
        //     {
        //     case 0:
        //         return P1 - P0;
        //         break;
        //     case 1:
        //         return P2 - P1;
        //         break;
        //     case 2:
        //         return P0 - P2;
        //         break;
        //     default:
        //         break;
        //     }
        //     throw "LOL";
        // }

        friend std::istream& operator>>(std::istream& in, Triangle& trian)
        {
            return in >> trian.P0 >> trian.P1 >> trian.P2;
        }

        friend std::ostream& operator<<(std::ostream& out, const Triangle& trian)
        {
            return out << trian.P0 << "\t" << trian.P1 << "\t" << trian.P2;
        }
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
            a = (triangle.P1.y - triangle.P0.y) * (triangle.P2.z - triangle.P0.z) - 
                (triangle.P2.y - triangle.P0.y) * (triangle.P1.z - triangle.P0.z);

            b = -(triangle.P1.x - triangle.P0.x) * (triangle.P2.z - triangle.P0.z) -
                 (triangle.P2.x - triangle.P0.x) * (triangle.P1.z - triangle.P0.z);
    
            c = (triangle.P1.x - triangle.P0.x) * (triangle.P2.y - triangle.P0.y) -
                (triangle.P2.x - triangle.P0.x) * (triangle.P1.y - triangle.P0.y);
            
            d = -(triangle.P0.x * a + triangle.P0.y * b + triangle.P0.z * c); 
        }

        Vector normal() const { return Vector {a, b, c}; }

        friend std::ostream& operator<<(std::ostream& out, const Plane& plane)
        {
            return out << "a = " << plane.a << ", b = " << plane.b << ", c = " << plane.c << ", d = " << plane.d;
        }

    };

    inline bool operator||(const Plane& left, const Plane& right)
    {
        return left.a == right.a && left.b == right.b && left.c == right.c;
    }

    enum class rotate_t
    {
        left,
        right,
    };

    template <typename T> 
    void swap(T& first, T& second, T& third, rotate_t rotate)
    {
        auto temp = second;
        switch (rotate)
        {
        case rotate_t::left:
            second = third;
            third  = first;
            first  = temp;
            break;
        case rotate_t::right:
            second = first;
            first  = third;
            third  = temp;
            break;
        default:
            break;
        }
    } 

}; // namespace Geomitric
