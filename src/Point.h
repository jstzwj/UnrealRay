#pragma once
#ifndef UNREALRAY_POINT
#define UNREALRAY_POINT
namespace unreal
{
	class Point
	{
	public:
		double x, y, z;
	public:
		Point() 
			:x(0.0),y(0.0),z(0.0){}
		Point(double in_x, double in_y, double in_z)
		:x(in_x),y(in_y),z(in_z){}
		//Some operators
		Point operator+(double n) { return Point(x + n, y + n, z + n); }
		Point operator-(double n) { return Point(x - n, y - n, z - n); }
		Point operator*(double n) { return Point(x * n, y * n, z * n); }
		Point operator/(double n) { return Point(x / n, y / n, z / n); }

		Point & operator+=(double n) 
		{
			x += n;
			y += n;
			z += n;
			return *this; 
		}
		Point & operator-=(double n) 
		{
			x -= n;
			y -= n;
			z -= n;
			return *this; 
		}
		Point & operator*=(double n) 
		{
			x *= n;
			y *= n;
			z *= n;
			return *this;
		}
		Point & operator/=(double n) 
		{
			x /= n;
			y /= n;
			z /= n;
			return *this; 
		}
		//Some useful functions
		void move(double x, double y, double z)
		{
			this->x += x;
			this->y += y;
			this->z += z;
		}

	};



}


#endif // !UNREALRAY_POINT
