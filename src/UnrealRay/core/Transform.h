#ifndef TRANSFORM_H
#define TRANSFORM_H


#include<cmath>
#include<cassert>
#include"Type.h"
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
        Transform(Float *mat) {
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

        static Transform translate(const Vector3f &delta)
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
        static Transform  scale(Float x, Float y, Float z)
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
        static Transform  rotateX(Float  angle)
        {
            Float sin_t = std::sin(radians(angle));
            Float cos_t = std::cos(radians(angle));

            Matrix4x4 m(1, 0    , 0     , 0,
                        0, cos_t, -sin_t, 0,
                        0, sin_t, cos_t , 0,
                        0, 0    , 0     , 1);
            return Transform(m, m.transpose());
        }
        static Transform  rotateY(Float  angle)
        {
            Float sin_t = std::sin(radians(angle));
            Float cos_t = std::cos(radians(angle));

            Matrix4x4 m(cos_t,0 , -sin_t, 0,
                        0    ,1 , 0     , 0,
                        sin_t,0 , cos_t , 0,
                        0    ,0 , 0     , 1);
            return Transform(m, m.transpose());
        }
        static Transform  rotateZ(Float  angle)
        {
            Float sin_t = std::sin(radians(angle));
            Float cos_t = std::cos(radians(angle));

            Matrix4x4 m(cos_t, -sin_t,0  , 0,
                        sin_t, cos_t ,0  , 0,
                        0    , 0     ,1  , 0,
                        0    , 0     ,0  , 1);
            return Transform(m, m.transpose());
        }
        static Transform rotate(Float angle, const Vector3f &axis)
        {
                Vector3f a = axis.normalize();
                Float s = std::sin(radians(angle));
                Float c = std::cos(radians(angle));
                Float m[4][4];
                m[0][0] = a.x * a.x + (1.0f - a.x * a.x) * c;
                m[0][1] = a.x * a.y + (1.0f - c) - a.z *s;
                m[0][2] = a.x * a.z * (1.0f - c) + a.y * s;
                m[0][3] =0;

                m[1][0] = a.x * a.y * (1.0f - c) + a.z *s;
                m[1][1] = a.y* a.y + (1.0f - a.y * a.y) *c;
                m[1][2] = a.y * a.z * (1.0f - c) - a.x * s;
                m[1][3] =0;

                m[2][0] = a.x * a.z * (1.0f - c) - a.y *s;
                m[2][1] = a.y * a.z * (1.0f - c) + a.x *s;
                m[2][2] = a.z * a.z + (1.0f - a.z * a.z) * c;
                m[2][3] =0;

                m[3][0] = 0;
                m[3][1] = 0;
                m[3][2] = 0;
                m[3][3] =1;
                Matrix4x4 mat(m);
                return Transform(mat, mat.transpose());
        }
        static Transform lookAt(const Point3f &pos, const Point3f &look, const Vector3f &up)
        {
            Matrix4x4 cameraToWorld;
            // Initialize fourth column of viewing matrix
            cameraToWorld.data[0][3] = pos.x;
            cameraToWorld.data[1][3] = pos.y;
            cameraToWorld.data[2][3] = pos.z;
            cameraToWorld.data[3][3] = 1;

            // Initialize first three columns of viewing matrix
            Vector3f dir = (Vector3f(look - pos)).normalize();
            if (up.normalize().cross(dir).length() == 0) {
                //record error
                return Transform();
            }
            Vector3f left = up.normalize().cross(dir).normalize();
            Vector3f newUp = dir.cross(left);
            cameraToWorld.data[0][0] = left.x;
            cameraToWorld.data[1][0] = left.y;
            cameraToWorld.data[2][0] = left.z;
            cameraToWorld.data[3][0] = 0.0f;
            cameraToWorld.data[0][1] = newUp.x;
            cameraToWorld.data[1][1] = newUp.y;
            cameraToWorld.data[2][1] = newUp.z;
            cameraToWorld.data[3][1] = 0.0f;
            cameraToWorld.data[0][2] = dir.x;
            cameraToWorld.data[1][2] = dir.y;
            cameraToWorld.data[2][2] = dir.z;
            cameraToWorld.data[3][2] = 0.0f;
            return Transform(Matrix4x4::inverse(cameraToWorld), cameraToWorld);
        }
        static Transform inverse(const Transform & other)
        {
            return Transform(other.mInv, other.m);
        }

        static Transform eye()
        {
            Transform rst;
            rst.m=Matrix4x4::eye();
            rst.mInv=Matrix4x4::eye();
            return rst;
        }


        Point3f transform(const Point3f &pt) const
        {
            Float x = pt.x, y = pt.y, z = pt.z;
            Float xp = m.data[0][0]*x + m.data[0][1]*y + m.data[0][2]*z + m.data[0][3];
            Float yp = m.data[1][0]*x + m.data[1][1]*y + m.data[1][2]*z + m.data[1][3];
            Float zp = m.data[2][0]*x + m.data[2][1]*y + m.data[2][2]*z + m.data[2][3];
            Float wp = m.data[3][0]*x + m.data[3][1]*y + m.data[3][2]*z + m.data[3][3];
            assert(wp != 0);
            if(wp == 1.0)
                return Point3f(xp, yp, zp);
            else
                return Point3f(xp,yp,zp)/wp;
        }
        Vector3f transform(const Vector3f &v) const
        {
            Float x = v.x, y = v.y, z = v.z;
            return Vector3f(m.data[0][0]*x + m.data[0][1]*y + m.data[0][2]*z,
                          m.data[1][0]*x + m.data[1][1]*y + m.data[1][2]*z,
                          m.data[2][0]*x + m.data[2][1]*y + m.data[2][2]*z);
        }
        Normal3f transform(const Normal3f &n) const
        {
            Float x = n.x, y = n.y, z = n.z;
            return Normal3f(mInv.data[0][0] *x + mInv.data[1][0]*y + mInv.data[0][0]*z,
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
            Float det = (( m.data[0][0] *(m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1]))
                    -(m.data[0][1] *(m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0])) +
                        (m.data[0][2] *(m.data[1][0] * m.data[2][1] -m.data[1][1] * m.data[2][0])));
            return det < 0.f;
        }

    };
}

#endif // TRANSFORM_H
