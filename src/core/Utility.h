#ifndef UTILITY_H
#define UTILITY_H

#include<cmath>

namespace unreal
{


    template <typename T, typename U, typename V>
    T clamp(T val, U low, V high)
    {
        if (val < low) return low;
        else if (val > high) return high;
        else return val;
    }

    const double PI= 3.1415926;
    template<class T>
    double radians(T n)
    {
        return n*PI/180;
    }


    /*inline bool quadratic(double A, double B, double C, double *t0, double *t1)
    {
        double discrim = B * B - 4.0 * A * C;
        if( discrim < 0 ) return false;
        double rootDiscrim = std::sqrt(discrim);
        double q;
        if( B < 0)
            q = -0.5 * (B - rootDiscrim);
        else
            q = -0.5 * (B + rootDiscrim);
        *t0 = q / A;
        *t1= C / q;
        if ( * t0 > *t1) swap(*t0, *t1);
        return true;
    }*/
    inline bool quadratic(double A, double B, double C, double *t0, double *t1)
    {
        double discrim = B * B - 4.0 * A * C;
        if( discrim < 0 ) return false;
        double rootDiscrim = std::sqrt(discrim);
        *t0=(-B-rootDiscrim)*0.5/A;
        *t1=(-B+rootDiscrim)*0.5/A;
        return true;
    }
}

#endif // UTILITY_H
