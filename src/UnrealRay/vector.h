#pragma once
#ifndef UNREALRAY_VECTOR
#define UNREALRAY_VECTOR
#include"math.h"

namespace unreal
{
	class Vector
	{
	public:
		double x, y, z;
	public:
		Vector()
			:x(0.0), y(0.0), z(0.0) {}
		Vector(double in_x, double in_y, double in_z)
			:x(in_x), y(in_y), z(in_z) {}
		//Some operators
		Vector operator + (const Vector & rhs)
		{
			return Vector(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
		}
		Vector operator + (double rhs)
		{
			return Vector(this->x + rhs, this->y + rhs, this->z + rhs);
		}
		Vector operator - (const Vector & rhs)
		{
			return Vector(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
		}
		Vector operator - (double rhs)
		{
			return Vector(this->x - rhs, this->y - rhs, this->z - rhs);
		}
		Vector operator * (const Vector & rhs)
		{
			return Vector(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
		}
		Vector operator * (double rhs)
		{
			return Vector(this->x * rhs, this->y * rhs, this->z * rhs);
		}
		Vector operator / (const Vector & rhs)
		{
			return Vector(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
		}
		Vector operator / (double rhs)
		{
			return Vector(this->x / rhs, this->y / rhs, this->z / rhs);
		}
		Vector& operator += (const Vector & rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		Vector& operator += (double rhs)
		{
			x += rhs;
			y += rhs;
			z += rhs;
			return *this;
		}
		Vector& operator -= (const Vector & rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		Vector& operator -= (double rhs)
		{
			x -= rhs;
			y -= rhs;
			z -= rhs;
			return *this;
		}
		Vector& operator *= (const Vector & rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			return *this;
		}
		Vector& operator *= (double rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}

		Vector operator-()
		{
			return Vector(-x, -y, -z);
		}
		Vector operator+()
		{
			return Vector(*this);
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
		Vector normalize()
		{
			return Vector(x/length(),y/length(),z/length());
		}
		double dot(const Vector &rhs)
		{
			return x*rhs.x + y*rhs.y + z*rhs.z;
		}
		Vector cross(const Vector &rhs)
		{
			return Vector(-z * rhs.y + y * rhs.z, z * rhs.x - x * rhs.z, -y * rhs.x + x * rhs.y);
		}
	};
}





#endif // !UNREALRAY_VECTOR
