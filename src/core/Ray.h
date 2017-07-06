#pragma once
#ifndef UNREALRAY_RAY
#define UNREALRAY_RAY
#include"Type.h"
#include"Point.h"
#include"Vector.h"
#include"Point.h"
#include"Type.h"
#include<limits>
#include<cmath>

namespace unreal
{
    #define RAY_EPSILON (std::numeric_limits<Float>::epsilon())
    #define RAY_INFINITY INFINITY
	class Ray
	{
	public:
        Point3f origin;
        Vector3f direction;
        Float mint, maxt;
        Float time;
	public:
        Ray() : mint(RAY_EPSILON), maxt(RAY_INFINITY) {}
        Ray(const Point3f &origin, const Vector3f &direction,
                Float start = RAY_EPSILON, Float end = RAY_INFINITY)
                : origin(origin), direction(direction), mint(start), maxt(end){}

        Point3f getPoint(Float d)
        {
            Vector3f normal_v=direction.normalize();
            return Point3f(d*normal_v.x,d*normal_v.y,d*normal_v.z);
        }

        Point3f operator[](Float d)
        {
            Vector3f normal_v=direction.normalize();
            return Point3f(d*normal_v.x,d*normal_v.y,d*normal_v.z);
        }


	};



}

#endif // !UNREALRAY_RAY
