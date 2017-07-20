#ifndef MATRIX_H
#define MATRIX_H


#include"Type.h"
#include<cstring>
#include<cmath>
#include<limits>
namespace unreal
{
    class Matrix4x4
    {
    public:
        Float data[4][4];
    public:
        Matrix4x4()
        {
            data[0][0] = data[1][1] = data[2][2] = data[3][3] = 1.0f;
            data[0][1] = data[0][2] = data[0][3] =
            data[1][0] = data[1][2] = data[1][3] =
            data[2][0] = data[2][1] = data[2][3] =
            data[3][0] = data[3][1] = data[3][2] = 0.0f;
        }

        Matrix4x4(Float * mat)
        {
            std::memcpy(data,mat,16*sizeof(Float));
        }
        Matrix4x4(Float (*mat)[4])
        {
            std::memcpy(data,mat,16*sizeof(Float));
        }
        Matrix4x4(Float d1,Float d2,Float d3,Float d4,
                  Float d5,Float d6,Float d7,Float d8,
                  Float d9,Float d10,Float d11,Float d12,
                  Float d13,Float d14,Float d15,Float d16)
        {
            data[0][0]=d1;
            data[0][1]=d2;
            data[0][2]=d3;
            data[0][3]=d4;
            data[1][0]=d5;
            data[1][1]=d6;
            data[1][2]=d7;
            data[1][3]=d8;
            data[2][0]=d9;
            data[2][1]=d10;
            data[2][2]=d11;
            data[2][3]=d12;
            data[3][0]=d13;
            data[3][1]=d14;
            data[3][2]=d15;
            data[3][3]=d16;
        }
        Float& operator[](int n)
        {
            return ((Float *)data)[n];
        }
        Float operator[](int n)const
        {
            return ((Float *)data)[n];
        }

        Matrix4x4 inverse()const
        {
            Matrix4x4 m(*this);
            Matrix4x4 result;
            for(int i=0;i<4;++i)
                result.data[i][i]=1.0f;
            for (int i = 0; i < 4; ++i)
            {
                Float scale = m[i*4 + i];
                //Unitized row
                for (int j = 0; j < 4; ++j)
                {
                    m[i*4 + j] /= scale;
                    result[i*4 + j] /= scale;
                }
                //Elementary row transformation
                for (int j = 0; j < 4; ++j)
                {
                    if (j != i)
                    {
                        Float scale_each = m[j*4 + i] / m[i*4 + i];
                        for (int k = 0; k < 4; ++k)
                        {
                            m[j*4 + k] = m[j*4 + k] - scale_each*m[i*4 + k];
                            result[j*4 + k] = result[j*4 + k] - scale_each*result[i*4 + k];
                        }
                    }
                }
            }
            return result;
        }
        static Matrix4x4 inverse(const Matrix4x4 & m)
        {
            return m.inverse();
        }
        Matrix4x4 transpose()const
        {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    result.data[i][j] = data[j][i];
                }
            }
            return result;
        }
        static Matrix4x4 transpose(const Matrix4x4 & m)
        {
            return m.transpose();
        }
        static Matrix4x4 times(const Matrix4x4& a,const Matrix4x4& b)
        {
            Matrix4x4 result=Matrix4x4::zero();
            for (int i = 0; i < 4; ++i)//a row
            {
                for (int j = 0; j < 4; ++j)//a col
                {
                    if (std::abs(a.data[i][j]) <= std::numeric_limits<Float>::epsilon())
                    {
                        continue;
                    }
                    for (int k = 0; k < 4; ++k)//b col
                    {
                        result.data[i][k] += a.data[i][j] * b.data[j][k];
                    }
                }
            }
            return result;
        }
        static Matrix4x4 eye()
        {
            Matrix4x4 rst;
            for(int i=0;i<4;++i)
            {
                rst.data[i][i]=1;
            }
            return rst;
        }
        static Matrix4x4 zero()
        {
            Matrix4x4 rst;
            for(int i=0;i<4;++i)
            {
                rst.data[i][i]=0.0f;
            }
            return rst;
        }
    };
}


#endif // MATRIX_H
