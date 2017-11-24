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
        if (val < low) return static_cast<T>(low);
        else if (val > high) return static_cast<T>(high);
        else return static_cast<T>(val);
    }

    const Float PI= 3.141592653f;
    const Float INV_PI= 0.31830988618f;
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
        /*Float discrim = B * B - 4.0f * A * C;
        if( discrim < 0 ) return false;
        Float rootDiscrim = std::sqrt(discrim);
        *t0=(-B-rootDiscrim)*0.5f/A;
        *t1=(-B+rootDiscrim)*0.5f/A;
        return true;*/

        double discrim = (double)B * (double)B - 4.0 * (double)A * (double)C;
        if (discrim < 0.0) return false;
        double rootDiscrim = std::sqrt(discrim);

        Float floatRootDiscrim=(unreal::Float)rootDiscrim;

        // Compute quadratic _t_ values
        Float q;
        if ((float)B < 0)
            q = -0.5f * (B - floatRootDiscrim);
        else
            q = -0.5f * (B + floatRootDiscrim);
        *t0 = q / A;
        *t1 = C / q;
        if ((float)*t0 > (float)*t1) std::swap(*t0, *t1);
        return true;
    }

    template <typename T>
    Point3<T> lerp(Float t, const Point3<T> &p0, const Point3<T> &p1)
    {
        return (1 - t) * p0 + t * p1;
    }

    inline Float lerp(Float t, Float v1, Float v2) { return (1 - t) * v1 + t * v2; }


    inline Float distance(const Point2f &m,const Point2f &n)
    {
        Float x=m.x-n.x;
        Float y=m.y-n.y;
        return std::sqrt(x*x+y*y);
    }

    inline Float distance(const Point3f &m,const Point3f &n)
    {
        Float x=m.x-n.x;
        Float y=m.y-n.y;
        Float z=m.z-n.z;
        return std::sqrt(x*x+y*y+z*z);
    }

	inline Float distanceSquared(const Point3f &m, const Point3f &n)
	{
		Float x = m.x - n.x;
		Float y = m.y - n.y;
		Float z = m.z - n.z;
		return x*x + y*y + z*z;
	}
}

#endif // UTILITY_H
