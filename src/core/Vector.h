#pragma once
#ifndef UNREALRAY_VECTOR
#define UNREALRAY_VECTOR
#include<stdexcept>

#include"Type.h"
#include"Point.h"

namespace unreal
{
    template<class T>
    class Vector2
    {
    public:
        T x, y;
    public:
        Vector2()
            :x(), y() {}
        Vector2(T in_x, T in_y)
            :x(in_x), y(in_y) {}
        Vector2(const Point2<T>& p)
            :x(p.x), y(p.y) {}
        /*explicit Vector(const Normal &n)
               :x(n.x), y(n.y), z(n.z) { }*/
        //Some operators
        Vector2<T> operator + (const Vector2<T> & rhs) const
        {
            return Vector2<T>(this->x + rhs.x, this->y + rhs.y);
        }
        Vector2<T> operator + (T rhs) const
        {
            return Vector2<T>(this->x + rhs, this->y + rhs);
        }
        Vector2<T> operator - (const Vector2<T> & rhs) const
        {
            return Vector2<T>(this->x - rhs.x, this->y - rhs.y);
        }
        Vector2<T> operator - (T rhs) const
        {
            return Vector2<T>(this->x - rhs, this->y - rhs);
        }
        Vector2<T> operator * (const Vector2<T> & rhs) const
        {
            return Vector2<T>(this->x * rhs.x, this->y * rhs.y);
        }
        Vector2<T> operator * (T rhs) const
        {
            return Vector2<T>(this->x * rhs, this->y * rhs);
        }
        Vector2<T> operator / (const Vector2<T> & rhs) const
        {
            return Vector2<T>(this->x / rhs.x, this->y / rhs.y);
        }
        Vector2<T> operator / (T rhs) const
        {
            return Vector2<T>(this->x / rhs, this->y / rhs);
        }
        Vector2<T>& operator += (const Vector2<T> & rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }
        Vector2<T>& operator += (T rhs)
        {
            x += rhs;
            y += rhs;
            return *this;
        }
        Vector2<T>& operator -= (const Vector2<T> & rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }
        Vector2<T>& operator -= (T rhs)
        {
            x -= rhs;
            y -= rhs;
            return *this;
        }
        Vector2<T>& operator *= (const Vector2<T> & rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            return *this;
        }
        Vector2<T>& operator *= (T rhs)
        {
            x *= rhs;
            y *= rhs;
            return *this;
        }

        Vector2<T> operator-() const
        {
            return Vector2<T>(-x, -y);
        }
        Vector2<T> operator+() const
        {
            return Vector2<T>(*this);
        }
        T operator[](int i)
        {
            switch(i)
            {
            case 0:
                return x;
                break;
            case 1:
                return y;
                break;
            default:
                throw std::runtime_error("Vector2<T>:out of range");
            }
        }

        //Some useful functions
        T sqrLength()const
        {
            return x*x + y*y;
        }
        T length()const
        {
            return sqrt(x*x + y*y);
        }
        Vector2<T> normalize()const
        {
            return Vector2<T>(x/length(),y/length());
        }
        T dot(const Vector2<T> &rhs)const
        {
            return x*rhs.x + y*rhs.y;
        }
        Vector2<T> cross(const Vector2<T> &rhs)const
        {
            return Vector2<T>(x*rhs.y-y*rhs.x);
        }
    };

    template<class T>
    class Vector3
	{
	public:
        T x, y, z;
	public:
        Vector3()
			:x(0.0), y(0.0), z(0.0) {}
        Vector3(T in_x, T in_y, T in_z)
			:x(in_x), y(in_y), z(in_z) {}
        Vector3(const Point3<T>& p)
            :x(p.x), y(p.y), z(p.z){}
        /*explicit Vector3<T>(const Normal &n)
               :x(n.x), y(n.y), z(n.z) { }*/
		//Some operators
        Vector3<T> operator + (const Vector3<T> & rhs) const
		{
            return Vector3<T>(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
		}
        Vector3<T> operator + (T rhs) const
		{
            return Vector3<T>(this->x + rhs, this->y + rhs, this->z + rhs);
		}
        Vector3<T> operator - (const Vector3<T> & rhs) const
		{
            return Vector3<T>(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
		}
        Vector3<T> operator - (T rhs) const
		{
            return Vector3<T>(this->x - rhs, this->y - rhs, this->z - rhs);
		}
        Vector3<T> operator * (const Vector3<T> & rhs) const
		{
            return Vector3<T>(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
		}
        Vector3<T> operator* (T rhs) const
		{
            return Vector3<T>(x * rhs, y * rhs, z * rhs);
		}
        Vector3<T> operator / (const Vector3<T> & rhs) const
		{
            return Vector3<T>(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
		}
        Vector3<T> operator / (T rhs) const
		{
            return Vector3<T>(this->x / rhs, this->y / rhs, this->z / rhs);
		}
        Vector3<T>& operator += (const Vector3<T> & rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
        Vector3<T>& operator += (T rhs)
		{
			x += rhs;
			y += rhs;
			z += rhs;
			return *this;
		}
        Vector3<T>& operator -= (const Vector3<T> & rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
        Vector3<T>& operator -= (T rhs)
		{
			x -= rhs;
			y -= rhs;
			z -= rhs;
			return *this;
		}
        Vector3<T>& operator *= (const Vector3<T> & rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			return *this;
		}
        Vector3<T>& operator *= (T rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}

        Vector3<T> operator-()
		{
            return Vector3<T>(-x, -y, -z);
		}
        Vector3<T> operator+()
		{
            return Vector3<T>(*this);
		}
        T operator[](int i)
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
                throw std::runtime_error("Vector3<T>:out of range");
            }
        }

		//Some useful functions
        T sqrLength()const
		{
			return x*x + y*y + z*z;
		}
        T length()const
		{
			return sqrt(x*x + y*y + z*z);
		}
        Vector3<T> normalize()const
		{
            return Vector3<T>(x/length(),y/length(),z/length());
		}
        T dot(const Vector3<T> &rhs)const
		{
			return x*rhs.x + y*rhs.y + z*rhs.z;
		}
        Vector3<T> cross(const Vector3<T> &rhs)const
		{
            return Vector3<T>(-z * rhs.y + y * rhs.z, z * rhs.x - x * rhs.z, -y * rhs.x + x * rhs.y);
		}
	};

    using Vector2i=Vector2<int>;
    using Vector2f=Vector2<Float>;

    using Vector3i=Vector3<int>;
    using Vector3f=Vector3<Float>;

}





#endif // !UNREALRAY_VECTOR
