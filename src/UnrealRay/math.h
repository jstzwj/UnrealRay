#pragma once
#ifndef UNREALRAY_MATH
#define UNREALRAY_MATH
#include<cmath>
namespace unreal
{
	inline double sin(double n)
	{
		return std::sin(n);
	}
	inline double cos(double n)
	{
		return std::cos(n);
	}
	inline double tan(double n)
	{
		return std::tan(n);
	}
	inline double sqrt(double n)
	{
		return std::sqrt(n);
	}
}

#endif // !UNREALRAY_MATH
