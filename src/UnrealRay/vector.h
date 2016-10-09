#pragma once
#ifndef UNREALRAY_VECTOR
#define UNREALRAY_VECTOR
#include"math.h"

namespace unreal
{
	class vector
	{
	public:
		double x, y, z;
	public:
		vector()
			:x(0.0), y(0.0), z(0.0) {}
		vector(double in_x, double in_y, double in_z)
			:x(in_x), y(in_y), z(in_z) {}
		//Some operators
		vector operator + (const vector & rhs)
		{
			return vector(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
		}
		vector operator + (double rhs)
		{
			return vector(this->x + rhs, this->y + rhs, this->z + rhs);
		}
		vector operator - (const vector & rhs)
		{
			return vector(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
		}
		vector operator - (double rhs)
		{
			return vector(this->x - rhs, this->y - rhs, this->z - rhs);
		}
		vector operator * (const vector & rhs)
		{
			return vector(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
		}
		vector operator * (double rhs)
		{
			return vector(this->x * rhs, this->y * rhs, this->z * rhs);
		}
		vector operator / (const vector & rhs)
		{
			return vector(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
		}
		vector operator / (double rhs)
		{
			return vector(this->x / rhs, this->y / rhs, this->z / rhs);
		}
		vector& operator += (const vector & rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		vector& operator += (double rhs)
		{
			x += rhs;
			y += rhs;
			z += rhs;
			return *this;
		}
		vector& operator -= (const vector & rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		vector& operator -= (double rhs)
		{
			x -= rhs;
			y -= rhs;
			z -= rhs;
			return *this;
		}
		vector& operator *= (const vector & rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			return *this;
		}
		vector& operator *= (double rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}

		vector operator-()
		{
			return vector(-x, -y, -z);
		}
		vector operator+()
		{
			return vector(*this);
		}
		//Some useful functions
		double sqrLength()
		{
			return x*x + y*y + z*z;
		}
		double length()
		{
			return sqrt(x*x + y*y + z*z);
		}
		vector normalize()
		{
			return vector(x/length(),y/length(),z/length());
		}
		double dot(const vector &rhs)
		{
			return x*rhs.x + y*rhs.y + z*rhs.z;
		}
		vector cross(const vector &rhs)
		{
			return vector(-z * rhs.y + y * rhs.z, z * rhs.x - x * rhs.z, -y * rhs.x + x * rhs.y);
		}
	};
}





#endif // !UNREALRAY_VECTOR
