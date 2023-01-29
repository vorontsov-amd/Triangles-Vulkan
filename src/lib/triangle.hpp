#include <valarray>
#include <array>
#include <tuple>
#include <iostream>

namespace Geomitric
{
    struct Vector;
    struct Line;
    struct Triangle;
    struct Plane;
    struct Segment;
    enum class rotate_t;
    enum class component_t;

    using array = std::array<double, 3>;

    bool operator||(const Plane& left, const Plane& right);
    double operator*(const Vector& left, const Vector& right);
    Vector operator*(double num, const Vector& vec);
    Vector operator*(const Vector& vec, double num);
    Vector operator+(const Vector& left, const Vector& right);
    Vector operator-(const Vector& left, const Vector& right);
    Vector operator-(Vector&& left, Vector&& right);
    Vector operator/(const Vector& vec, double num);
    Vector operator/(Vector&& vec, double num);
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


    static inline bool isEqual(double a, double b)
    {
        const static double EPS = 1e-5;
        return std::abs(a - b) < EPS;
    }

    template <typename T> 
    void swap(T& first, T& second, T& third, rotate_t rotate);

    enum class component_t
    {
        x = 0, y = 1, z = 2,
    };

    struct Vector
    {
    private:
        double clength = NAN;
    public:
        double x = 0, y = 0, z = 0; 

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

        double length() const
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

        const double& operator[] (component_t comp) const
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

        double& operator[] (component_t comp)
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

    inline double determinant (const Vector &first, const Vector &second, const Vector &third) {

        return (first.x * second.y * third.z - 
                first.x * second.z * third.y -
                first.y * second.x * third.z +
                first.y * second.z * third.x +
                first.z * second.x * third.y -
                first.z * second.y * third.x);
    }

    inline double operator*(const Vector& left, const Vector& right)
    {
        return left.x * right.x + left.y * right.y + left.z * right.z;
    }

    inline Vector operator*(double num, const Vector& vec)
    {
        return Vector {vec.x * num, vec.y * num, vec.z * num};
    }

    inline Vector operator*(const Vector& vec, double num)
    {
        return Vector {vec.x * num, vec.y * num, vec.z * num};
    }

    inline Vector operator/(const Vector& vec, double num)
    {
        return Vector {vec.x / num, vec.y / num, vec.z / num};
    }

    inline Vector operator/(Vector&& vec, double num)
    {
        return vec /= num;
    }

    inline Vector operator+(const Vector& left, const Vector& right)
    {
        return Vector {left.x + right.x, left.y + right.y, left.z + right.z};
    }

    inline Vector operator+(Vector&& left, Vector&& right)
    {
        return left += right;
    }

    inline bool operator==(const Vector& left, const Vector& right) {
        return isEqual(left.x,right.x) and isEqual(left.y,right.y) and isEqual(left.z,right.z);
    }

    inline bool operator!=(const Vector& left, const Vector& right) {
        return !(left == right);
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
        return Vector { left.y * right.z - left.z * right.y,
                       -left.x * right.z + left.z * right.x,
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

        enum { TRIANGLE = 0b1, POINT = 0b10, SEGMENT = 0b100 } status;

        Triangle() {}

        Triangle(const Vector& p0, const Vector& p1, const Vector& p2) : P0{p0}, P1{p1}, P2{p2} {
            if ((p0 == p1) and (p1 == p2) and (p2 == p0)) {
                status = POINT;
            }
            else if ((p0 != p1) and (p1 != p2) and (p2 != p0)) {
                status = TRIANGLE;
            }
            else {
                status = SEGMENT;
            }
        }

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

        friend std::istream& operator>>(std::istream& in, Triangle& trian)
        {
            in >> trian.P0 >> trian.P1 >> trian.P2;

            if ((trian.P0 == trian.P1) and (trian.P1 == trian.P2) and (trian.P2 == trian.P0)) {
                //puts("point");
                trian.status = POINT;
            }
            else if (((trian.P0 != trian.P1) and (trian.P1 != trian.P2) and (trian.P2 != trian.P0)) 
                and !isEqual(cross(trian.P0, trian.P1) * trian.P2, 0)){
                //puts("triangle");
                trian.status = TRIANGLE;
            }
            else {
                //puts("segment");
                trian.status = SEGMENT;
            }

            return in;
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

        Segment() {}

        Segment(const Vector t1, const Vector& t2) : T1{t1}, T2{t2} {}

        Segment(const Triangle& tri) {
            if (tri.status != Triangle::SEGMENT) {
                throw std::logic_error("Triangle is not a segment");
            }

            T1 = tri.P0;

            if (tri.P1 == tri.P0) {
                T2 = tri.P2;
            }
            else if (tri.P2 == tri.P0) {
                
                T2 = tri.P1;
            }
            else if (tri.P2 == tri.P1) {
                T2 = tri.P2;
            }
            else {
                Vector candidat_1 = tri.P1 - tri.P0;
                Vector candidat_2 = tri.P2 - tri.P0;
                Vector candidat_3 = tri.P2 - tri.P1;

                if (candidat_1.length() > candidat_2.length() and
                    candidat_1.length() > candidat_3.length()) {
                        T1 = tri.P0;
                        T2 = tri.P1;
                    }
                else if (candidat_2.length() > candidat_1.length() and
                    candidat_2.length() > candidat_3.length()) {
                        T1 = tri.P0;
                        T2 = tri.P2;
                    }
                else {
                    T1 = tri.P1;
                    T2 = tri.P2;
                }
            }
        }
    };

    inline std::ostream& operator<<(std::ostream& out, const Segment& seg)
    {
        return out << seg.T1 << "\t" << seg.T2;
    }


    struct Plane 
    {
        double a = 1, b = 1, c = 1, d = 1;

        Plane(const Triangle& triangle)
        {
            a = (triangle.P1.y - triangle.P0.y) * (triangle.P2.z - triangle.P0.z) - 
                (triangle.P2.y - triangle.P0.y) * (triangle.P1.z - triangle.P0.z);

            b = -(triangle.P1.x - triangle.P0.x) * (triangle.P2.z - triangle.P0.z) -
                 (triangle.P2.x - triangle.P0.x) * (triangle.P1.z - triangle.P0.z);
    
            c = (triangle.P1.x - triangle.P0.x) * (triangle.P2.y - triangle.P0.y) -
                (triangle.P2.x - triangle.P0.x) * (triangle.P1.y - triangle.P0.y);
            
            d = -(triangle.P0.x * a + triangle.P0.y * b + triangle.P0.z * c); 

            if (d != 0) {
                a /= d;
                b /= d;
                c /= d;
                d  = 1;
            } 
        }

        Vector normal() const { return Vector {a, b, c}; }

        friend std::ostream& operator<<(std::ostream& out, const Plane& plane)
        {
            return out << "a = " << plane.a << ", b = " << plane.b << ", c = " << plane.c << ", d = " << plane.d;
        }

    };

    inline bool operator||(const Plane& left, const Plane& right)
    {
        return cross(left.normal(), right.normal()).length() == 0;
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
