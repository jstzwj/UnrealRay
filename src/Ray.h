#pragma once
#ifndef UNREALRAY_RAY
#define UNREALRAY_RAY
#include"Point.h"
#include"Vector.h"
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



	};



}

#endif // !UNREALRAY_RAY
