#pragma once
#ifndef UNREALRAY_POINT
#define UNREALRAY_POINT
namespace unreal
{
	class point
	{
	public:
		double x, y, z;
	public:
		point() 
			:x(0.0),y(0.0),z(0.0){}
		point(double in_x, double in_y, double in_z)
		:x(in_x),y(in_y),z(in_z){}
		//Some operators
		point operator+(double n) { return point(x + n, y + n, z + n); }
		point operator-(double n) { return point(x - n, y - n, z - n); }
		point operator*(double n) { return point(x * n, y * n, z * n); }
		point operator/(double n) { return point(x / n, y / n, z / n); }

		point & operator+=(double n) 
		{
			x += n;
			y += n;
			z += n;
			return *this; 
		}
		point & operator-=(double n) 
		{
			x -= n;
			y -= n;
			z -= n;
			return *this; 
		}
		point & operator*=(double n) 
		{
			x *= n;
			y *= n;
			z *= n;
			return *this;
		}
		point & operator/=(double n) 
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
