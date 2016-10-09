#pragma once
#ifndef UNREALRAY_RAY
#define UNREALRAY_RAY
#include"point.h"
#include"vector.h"
namespace unreal
{
	class ray
	{
	public:
		point origin;
		vector direction;
	public:
		ray() 
		:origin(),direction(){}
		ray(const point & in_origin, const vector & in_direction) 
		:origin(in_origin),direction(in_direction){}


	};



}

#endif // !UNREALRAY_RAY
