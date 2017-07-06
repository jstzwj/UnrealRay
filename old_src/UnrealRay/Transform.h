#ifndef TRANSFORM_H
#define TRANSFORM_H


#include<cmath>
#include<cassert>
#include"Utility.h"
#include"Matrix.h"
#include"Vector.h"
#include"Normal.h"
#include"Point.h"
#include"Ray.h"

namespace unreal
{


    class Transform
    {
    private:
        Matrix4x4 m,mInv;
    public:
        Transform()=default;
        Transform(double *mat) {
            m =Matrix4x4(mat);
            mInv = m.inverse();
        }
        Transform(const Matrix4x4 &mat) {
            m =  mat;
            mInv = m.inverse();
        }

        Transform(const Matrix4x4 &mat,const Matrix4x4 &minv)
            :m(mat),mInv(minv){}
        Transform getInverse() const
        {
            return Transform(mInv, m);
        }

        static Transform translate(const Vector &delta)
        {
            Matrix4x4 m(1,0,0,delta.x,
                        0,1,0,delta.y,
                        0,0,1,delta.z,
                        0,0,0,   1  );
            Matrix4x4 mInv(1,0,0, -delta.x,
                           0,1,0, -delta.y,
                           0,0,1, -delta.z,
                           0,0,0,   1   );
            return Transform(m, mInv);
        }
        static Transform  scale(double x, double y, double z)
        {
            Matrix4x4 m(x, 0, 0, 0,
                        0, y, 0, 0,
                        0, 0, z, 0,
                        0, 0, 0, 1);
            Matrix4x4 mInv(1.0f/x,0       ,0     ,0,
                           0,     1.0f/y  ,0     ,0,
                           0,     0       ,1.0f/z,0,
                           0,     0       ,0     ,1);
                return Transform(m, mInv);
        }
        static Transform  rotateX(double  angle)
        {
            double sin_t = std::sin(radians(angle));
            double cos_t = std::cos(radians(angle));

            Matrix4x4 m(1, 0    , 0     , 0,
                        0, cos_t, -sin_t, 0,
                        0, sin_t, cos_t , 0,
                        0, 0    , 0     , 1);
            return Transform(m, m.transpose());
        }
        static Transform  rotateY(double  angle)
        {
            double sin_t = std::sin(radians(angle));
            double cos_t = std::cos(radians(angle));

            Matrix4x4 m(cos_t,0 , -sin_t, 0,
                        0    ,1 , 0     , 0,
                        sin_t,0 , cos_t , 0,
                        0    ,0 , 0     , 1);
            return Transform(m, m.transpose());
        }
        static Transform  rotateZ(double  angle)
        {
            double sin_t = std::sin(radians(angle));
            double cos_t = std::cos(radians(angle));

            Matrix4x4 m(cos_t, -sin_t,0  , 0,
                        sin_t, cos_t ,0  , 0,
                        0    , 0     ,1  , 0,
                        0    , 0     ,0  , 1);
            return Transform(m, m.transpose());
        }
        static Transform rotate(double angle, const Vector &axis)
        {
                Vector a = axis.normalize();
                double s = std::sin(radians(angle));
                double c = std::cos(radians(angle));
                double m[4][4];
                m[0][0] = a.x * a.x + (1.0 - a.x * a.x) * c;
                m[0][1] = a.x * a.y + (1.0 - c) - a.z *s;
                m[0][2] = a.x * a.z * (1.0 - c) + a.y * s;
                m[0][3] =0;

                m[1][0] = a.x * a.y * (1.0 - c) + a.z *s;
                m[1][1] = a.y* a.y + (1.0 - a.y * a.y) *c;
                m[1][2] = a.y * a.z * (1.0 - c) - a.x * s;
                m[1][3] =0;

                m[2][0] = a.x * a.z * (1.0 - c) - a.y *s;
                m[2][1] = a.y * a.z * (1.0 - c) + a.x *s;
                m[2][2] = a.z * a.z + (1.0 - a.z * a.z) * c;
                m[2][3] =0;

                m[3][0] = 0;
                m[3][1] = 0;
                m[3][2] = 0;
                m[3][3] =1;
                Matrix4x4 mat(m);
                return Transform(mat, mat.transpose());
        }
        static Transform lookAt(const Point &pos, const Point &look, const Vector &up)
        {
            double m[4][4];

            Matrix4x4 camToWorld(m);
            return Transform(camToWorld.inverse(), camToWorld);
        }
        Point transform(const Point &pt) const
        {
            double x = pt.x, y = pt.y, z = pt.z;
            double xp = m.data[0][0]*x + m.data[0][1]*y + m.data[0][2]*z + m.data[0][3];
            double yp = m.data[1][0]*x + m.data[1][1]*y + m.data[1][2]*z + m.data[1][3];
            double zp = m.data[2][0]*x + m.data[2][1]*y + m.data[2][2]*z + m.data[2][3];
            double wp = m.data[3][0]*x + m.data[3][1]*y + m.data[3][2]*z + m.data[3][3];
            assert(wp != 0);
            if(wp == 1.0)
                return Point(xp, yp, zp);
            else
                return Point(xp,yp,zp)/wp;
        }
        Vector transform(const Vector &v) const
        {
            double x = v.x, y = v.y, z = v.z;
            return Vector(m.data[0][0]*x + m.data[0][1]*y + m.data[0][2]*z,
                          m.data[1][0]*x + m.data[1][1]*y + m.data[1][2]*z,
                          m.data[2][0]*x + m.data[2][1]*y + m.data[2][2]*z);
        }
        Normal transform(const Normal &n) const
        {
            double x = n.x, y = n.y, z = n.z;
            return Normal(mInv.data[0][0] *x + mInv.data[1][0]*y + mInv.data[0][0]*z,
                          mInv.data[0][1] *x + mInv.data[1][1]*y + mInv.data[0][1]*z,
                          mInv.data[0][2] *x + mInv.data[1][2]*y + mInv.data[0][2]*z);
        }
        Ray transform(const Ray &r) const
        {
            Ray result;
            result.origin=this->transform(r.origin);
            result.direction=this->transform(r.direction);
            result.mint = r.mint;
            result.maxt = r.maxt;
            return result;
        }
        Transform operator *(const Transform &t) const
        {
            Matrix4x4 m1 = Matrix4x4::times(m, t.m);
            Matrix4x4 m2 = Matrix4x4::times(t.mInv, mInv);
            return Transform(m1,m2);
        }

        bool swapsHandedness() const
        {
            double det = (( m.data[0][0] *(m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1]))
                    -(m.data[0][1] *(m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0])) +
                        (m.data[0][2] *(m.data[1][0] * m.data[2][1] -m.data[1][1] * m.data[2][0])));
            return det < 0.f;
        }

    };
}

#endif // TRANSFORM_H
