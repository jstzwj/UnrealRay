#pragma once
#ifndef UNREALRAY_RAY
#define UNREALRAY_RAY
#include"Point.h"
#include"Vector.h"
#include"Point.h"
#include<limits>
#include<cmath>

namespace unreal
{
    #define RAY_EPSILON (std::numeric_limits<double>::epsilon())
    #define RAY_INFINITY INFINITY
	class Ray
	{
	public:
		Point origin;
		Vector direction;
        double mint, maxt;
	public:
        Ray() : mint(RAY_EPSILON), maxt(RAY_INFINITY) {}
        Ray(const Point &origin, const Vector &direction,
                float start = RAY_EPSILON, float end = RAY_INFINITY)
                : o(origin), d(direction), mint(start), maxt(end){}

        Point getPoint(double d)
        {
            Vector normal_v=direction.normalize();
            return Point(d*normal_v.x,d*normal_v.y,d*normal_v.z);
        }

        Point operator[](double d)
        {
            Vector normal_v=direction.normalize();
            return Point(d*normal_v.x,d*normal_v.y,d*normal_v.z);
        }


	};



}

#endif // !UNREALRAY_RAY
