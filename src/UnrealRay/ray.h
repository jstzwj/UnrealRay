#pragma once
#ifndef UNREALRAY_RAY
#define UNREALRAY_RAY
#include"Point.h"
#include"Vector.h"
namespace unreal
{
	class Ray
	{
	public:
		Point origin;
		Vector direction;
	public:
		Ray() 
		:origin(),direction(){}
		Ray(const Point & in_origin, const Vector & in_direction) 
		:origin(in_origin),direction(in_direction){}


	};



}

#endif // !UNREALRAY_RAY
