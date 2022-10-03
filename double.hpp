#include <iostream>
#include <cmath>

class Double
{
private:
    constexpr static double EPS = 1e-7;
    double cx;
public:
    Double(double x) : cx(x) {}

    Double& operator*=(Double right) { cx *= right.cx; return *this; }
    Double& operator+=(Double right) { cx += right.cx; return *this; }
    Double& operator-=(Double right) { cx -= right.cx; return *this; }

    bool operator==(Double right) const { return std::abs(cx - right.cx) < EPS; }

    bool operator<=(Double right) const { return cx <= right.cx; }

    operator double() const { return cx; }

    friend std::ostream& operator<<(std::ostream& out, const Double& num) { return out << num.cx; }
};

inline Double operator*(Double left, Double right)
{   
    left *= right;
    return left;
}

inline Double operator+(Double left, Double right)
{   
    left += right;
    return left;
}

inline Double operator-(Double left, Double right)
{   
    left -= right;
    return left;
}

inline Double operator^(Double base, int deg)
{
    if (deg == 0) return Double(1); 
    if ((deg & 1) != 0) return (base ^ (deg - 1)) * base;
    
    Double result = base ^ (deg / 2);

    return result * result;
}

inline bool operator!=(Double left, Double right)
{
    return not (left == right); 
}


