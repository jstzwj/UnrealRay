#ifndef NORMAL_H
#define NORMAL_H

#include"Vector.h"

namespace unreal
{
    class Vector;
    class Normal
    {
    public:
        double x, y, z;
    public:
        Normal()
            :x(0.0), y(0.0), z(0.0) {}
        Normal(double in_x, double in_y, double in_z)
            :x(in_x), y(in_y), z(in_z) {}
        explicit Normal(const Vector &v)
                : x(v.x),  y(v.y), z(v.z) {}
        //Some operators
        Normal operator + (const Normal & rhs)
        {
            return Normal(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
        }
        Normal operator + (double rhs)
        {
            return Normal(this->x + rhs, this->y + rhs, this->z + rhs);
        }
        Normal operator - (const Normal & rhs)
        {
            return Normal(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
        }
        Normal operator - (double rhs)
        {
            return Normal(this->x - rhs, this->y - rhs, this->z - rhs);
        }
        Normal operator * (const Normal & rhs)
        {
            return Normal(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
        }
        Normal operator * (double rhs)
        {
            return Normal(this->x * rhs, this->y * rhs, this->z * rhs);
        }
        Normal operator / (const Normal & rhs)
        {
            return Normal(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
        }
        Normal operator / (double rhs)
        {
            return Normal(this->x / rhs, this->y / rhs, this->z / rhs);
        }
        Normal& operator += (const Normal & rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        Normal& operator += (double rhs)
        {
            x += rhs;
            y += rhs;
            z += rhs;
            return *this;
        }
        Normal& operator -= (const Normal & rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        Normal& operator -= (double rhs)
        {
            x -= rhs;
            y -= rhs;
            z -= rhs;
            return *this;
        }
        Normal& operator *= (const Normal & rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            return *this;
        }
        Normal& operator *= (double rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        Normal operator-()
        {
            return Normal(-x, -y, -z);
        }
        Normal operator+()
        {
            return Normal(*this);
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
                throw std::runtime_error("Normal:out of range");
            }
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
        Normal normalize()
        {
            return Normal(x/length(),y/length(),z/length());
        }
        double dot(const Normal &rhs)
        {
            return x*rhs.x + y*rhs.y + z*rhs.z;
        }
        Normal cross(const Normal &rhs)
        {
            return Normal(-z * rhs.y + y * rhs.z, z * rhs.x - x * rhs.z, -y * rhs.x + x * rhs.y);
        }
    };
}
#endif // NORMAL_H
