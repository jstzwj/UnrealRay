#pragma once
#ifndef UNREALRAY_POINT
#define UNREALRAY_POINT

#include"Type.h"
namespace unreal
{
    template<class T>
    class Point2
	{
	public:
        T x, y;
	public:
        Point2()
            :x(),y(){}
        Point2(T in_x, T in_y)
        :x(in_x),y(in_y){}
		//Some operators
        Point2<T> operator+(T n) { return Point2<T>(x + n, y + n); }
        Point2<T> operator-(T n) { return Point2<T>(x - n, y - n); }
        Point2<T> operator*(T n) { return Point2<T>(x * n, y * n); }
        Point2<T> operator/(T n) { return Point2<T>(x / n, y / n); }

        Point2<T> & operator+=(T n)
		{
			x += n;
			y += n;
			return *this; 
		}
        Point2<T> & operator-=(T n)
		{
			x -= n;
			y -= n;
			return *this; 
		}
        Point2<T> & operator*=(T n)
		{
			x *= n;
			y *= n;
			return *this;
		}
        Point2<T> & operator/=(T n)
		{
			x /= n;
			y /= n;
			return *this; 
		}
		//Some useful functions
        void move(T x, T y)
		{
			this->x += x;
			this->y += y;
		}

	};

    template<class T>
    class Point3
    {
    public:
        T x, y, z;
    public:
        Point3()
            :x(0.0),y(0.0),z(0.0){}
        Point3(T in_x, T in_y, T in_z)
        :x(in_x),y(in_y),z(in_z){}
        //Some operators
        Point3<T> operator+(T n) { return Point3<T>(x + n, y + n, z + n); }
        Point3<T> operator-(T n) { return Point3<T>(x - n, y - n, z - n); }
        Point3<T> operator*(T n) { return Point3<T>(x * n, y * n, z * n); }
        Point3<T> operator/(T n) { return Point3<T>(x / n, y / n, z / n); }

        Point3<T> & operator+=(T n)
        {
            x += n;
            y += n;
            z += n;
            return *this;
        }
        Point3<T> & operator-=(T n)
        {
            x -= n;
            y -= n;
            z -= n;
            return *this;
        }
        Point3<T> & operator*=(T n)
        {
            x *= n;
            y *= n;
            z *= n;
            return *this;
        }
        Point3<T> & operator/=(T n)
        {
            x /= n;
            y /= n;
            z /= n;
            return *this;
        }
        //Some useful functions
        void move(T x, T y, T z)
        {
            this->x += x;
            this->y += y;
            this->z += z;
        }

    };


    using Point2i=Point2<int>;
    using Point2f=Point2<Float>;


    using Point3i=Point3<int>;
    using Point3f=Point3<Float>;

}


#endif // !UNREALRAY_POINT
