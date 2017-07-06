#ifndef UTILITY_H
#define UTILITY_H

#include"Type.h"
#include"Point.h"
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

    const Float PI= 3.1415926f;
    template<class T>
    Float radians(T n)
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
    inline bool quadratic(Float A, Float B, Float C, Float *t0, Float *t1)
    {
        Float discrim = B * B - 4.0 * A * C;
        if( discrim < 0 ) return false;
        Float rootDiscrim = std::sqrt(discrim);
        *t0=(-B-rootDiscrim)*0.5/A;
        *t1=(-B+rootDiscrim)*0.5/A;
        return true;
    }

    template <typename T>
    Point3<T> lerp(Float t, const Point3<T> &p0, const Point3<T> &p1) {
        return (1 - t) * p0 + t * p1;
    }
}

#endif // UTILITY_H
