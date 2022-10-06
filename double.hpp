#include <iostream>
#include <cmath>

class Double
{
private:
    constexpr const static double EPS = 1e-7;
    double cx;
public:
    Double(double x) : cx(x) {}

    Double& operator*=(const Double& right) { cx *= right.cx; return *this; }
    Double& operator/=(const Double& right) { cx /= right.cx; return *this; }
    Double& operator+=(const Double& right) { cx += right.cx; return *this; }
    Double& operator-=(const Double& right) { cx -= right.cx; return *this; }

    bool operator==(const Double& right) const { return std::abs(cx - right.cx) < EPS; }

    operator double() const { return cx; }

    friend std::ostream& operator<<(std::ostream& out, const Double& num) { return out << num.cx; }
    friend std::istream& operator>>(std::istream& in ,       Double& num) { return in  >> num.cx; }
};

inline Double operator*(const Double& left, const Double& right)
{   
    Double res {left};
    res *= right;
    return res;
}

inline Double operator*(Double&& left, Double&& right)
{   
    return left *= right;
}

inline Double operator*(Double&& left, const Double&& right)
{   
    return left *= right;
}

inline Double operator*(const Double& left, Double&& right)
{   
    return right *= left;
}

inline Double operator/(const Double& left, const Double& right)
{   
    Double res {left};
    res /= right;
    return res;
}

inline Double operator/(Double&& left, const Double& right)
{   
    return left /= right;
}

inline Double operator/(Double&& left, Double&& right)
{   
    return left /= right;
}

inline Double operator+(const Double& left, const Double& right)
{   
    Double res {left};
    res += right;
    return res;
}

inline Double operator+(Double&& left, Double&& right)
{   
    return left += right;
}

inline Double operator+(Double&& left, const Double& right)
{   
    return left += right;
}

inline Double operator+(const Double& left, Double&& right)
{   
    return right += left;
}

inline Double operator-(const Double& left, const Double& right)
{   
    Double res {left};
    res -= right;
    return res;
}

inline Double operator-(Double&& left, Double&& right)
{   
    return left -= right;
}

inline Double operator-(Double&& left, const Double& right)
{   
    return left -= right;
}

inline Double operator-(const Double& left, Double&& right)
{   
    return right -= left;
}

inline Double operator-(const Double& num)
{   
    Double res {num};
    res *= -1;
    return res;
}

inline Double operator-(Double&& num)
{   
    return num *= -1;
}

inline Double operator^(const Double& base, int deg)
{
    if (deg == 0) return 1; 
    if ((deg & 1) != 0) return (base ^ (deg - 1)) * base;
    
    Double&& result = base ^ (deg / 2);

    return result * result;
}

inline bool operator!=(const Double& left, const Double& right)
{
    return not (left == right); 
}


