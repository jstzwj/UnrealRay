#pragma once
#ifndef UNREALRAY_VECTOR
#define UNREALRAY_VECTOR
#include<stdexcept>

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
        /*explicit Vector(const Normal &n)
               :x(n.x), y(n.y), z(n.z) { }*/
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
        double operator[](int i)
        {
            switch(i)
            {
            case 0:
                return x;
                break;
            case 1:
                return y;
                break;
            case 2:
                return z;
                break;
            default:
                throw std::runtime_error("vector:out of range");
            }
        }

		//Some useful functions
        double sqrLength()const
		{
			return x*x + y*y + z*z;
		}
        double length()const
		{
			return sqrt(x*x + y*y + z*z);
		}
        Vector normalize()const
		{
			return Vector(x/length(),y/length(),z/length());
		}
        double dot(const Vector &rhs)const
		{
			return x*rhs.x + y*rhs.y + z*rhs.z;
		}
        Vector cross(const Vector &rhs)const
		{
			return Vector(-z * rhs.y + y * rhs.z, z * rhs.x - x * rhs.z, -y * rhs.x + x * rhs.y);
		}
	};
}





#endif // !UNREALRAY_VECTOR
