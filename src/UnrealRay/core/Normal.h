#ifndef NORMAL_H
#define NORMAL_H

#include"Type.h"
#include"Vector.h"

template <typename T>
class Vector3;

namespace unreal
{
    template<class T>
    class Normal3
    {
    public:
        T x, y, z;
    public:
        Normal3()
            :x(0.0), y(0.0), z(0.0) {}
        Normal3(T in_x, T in_y, T in_z)
            :x(in_x), y(in_y), z(in_z) {}
        explicit Normal3(const Vector3<T> &v)
                : x(v.x),  y(v.y), z(v.z) {}
        //Some operators
        Normal3<T> operator + (const Normal3<T> & rhs)
        {
            return Normal3<T>(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
        }
        Normal3<T> operator + (T rhs)
        {
            return Normal3<T>(this->x + rhs, this->y + rhs, this->z + rhs);
        }
        Normal3<T> operator - (const Normal3<T> & rhs)
        {
            return Normal3<T>(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
        }
        Normal3<T> operator - (T rhs)
        {
            return Normal3<T>(this->x - rhs, this->y - rhs, this->z - rhs);
        }
        Normal3<T> operator * (const Normal3<T> & rhs)
        {
            return Normal3<T>(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);
        }
        Normal3<T> operator * (T rhs)
        {
            return Normal3<T>(this->x * rhs, this->y * rhs, this->z * rhs);
        }
        Normal3<T> operator / (const Normal3<T> & rhs)
        {
            return Normal3<T>(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z);
        }
        Normal3<T> operator / (T rhs)
        {
            return Normal3<T>(this->x / rhs, this->y / rhs, this->z / rhs);
        }
        Normal3<T>& operator += (const Normal3<T> & rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        Normal3<T>& operator += (T rhs)
        {
            x += rhs;
            y += rhs;
            z += rhs;
            return *this;
        }
        Normal3<T>& operator -= (const Normal3<T> & rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        Normal3<T>& operator -= (T rhs)
        {
            x -= rhs;
            y -= rhs;
            z -= rhs;
            return *this;
        }
        Normal3<T>& operator *= (const Normal3<T> & rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            return *this;
        }
        Normal3<T>& operator *= (T rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        Normal3<T> operator-()
        {
            return Normal3<T>(-x, -y, -z);
        }
        Normal3<T> operator+()
        {
            return Normal3<T>(*this);
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
                throw std::runtime_error("Normal3<T>:out of range");
            }
        }

        //Some useful functions
        T sqrLength()
        {
            return x*x + y*y + z*z;
        }
        T length()
        {
            return sqrt(x*x + y*y + z*z);
        }
        Normal3<T> normalize()
        {
            return Normal3<T>(x/length(),y/length(),z/length());
        }
        T dot(const Normal3<T> &rhs)
        {
            return x*rhs.x + y*rhs.y + z*rhs.z;
        }
        Normal3<T> cross(const Normal3<T> &rhs)
        {
            return Normal3<T>(-z * rhs.y + y * rhs.z, z * rhs.x - x * rhs.z, -y * rhs.x + x * rhs.y);
        }
    };
    using Normal3i=Normal3<int>;
    using Normal3f=Normal3<Float>;
}
#endif // NORMAL_H
