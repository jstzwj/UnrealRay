#ifndef MATRIX_H
#define MATRIX_H

#include<cstring>
#include<cmath>
#include<limits>
namespace unreal
{
    class Matrix4x4
    {
    public:
        double data[16];
    public:
        Matrix4x4()=default;
        Matrix4x4(double * mat)
        {
            std::memcpy(data,mat,16*sizeof(double));
        }
        Matrix4x4(double d1,double d2,double d3,double d4,
                  double d5,double d6,double d7,double d8,
                  double d9,double d10,double d11,double d12,
                  double d13,double d14,double d15,double d16)
        {
            data[1]=d1;
            data[2]=d2;
            data[3]=d3;
            data[4]=d4;
            data[5]=d5;
            data[6]=d6;
            data[7]=d7;
            data[8]=d8;
            data[9]=d9;
            data[10]=d10;
            data[11]=d11;
            data[12]=d12;
            data[13]=d13;
            data[14]=d14;
            data[15]=d15;
            data[16]=d16;
        }
        Matrix4x4 inverse()
        {
            Matrix4x4 m(*this);
            Matrix4x4 result;
            for(int i=0;i<4;++i)
                result.data[i*4+i]=1.0;
            for (int i = 0; i < 4; ++i)
            {
                double scale = m[i*4 + i];
                //Unitized row
                for (int j = 0; j < 4; ++j)
                {
                    m[j*4 + i] /= scale;
                    result[j*4 + i] /= scale;
                }
                //Elementary row transformation
                for (int j = 0; j < 4; ++j)
                {
                    if (j != i)
                    {
                        double scale_each = m[i*4 + j] / m[i*4 + i];
                        for (int k = 0; k < 4; ++k)
                        {
                            m[k*4 + j] = m[k*4 + j] - scale_each*m[k*4 + i];
                            result[k*4 + j] = result[k*4 + j] - scale_each*result[k*4 + i];
                        }
                    }
                }
            }
            return result;
        }
        Matrix4x4 tramspose()
        {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    result.data[i*4 + j] = data[j*4 + i];
                }
            }
            return result;
        }
        static Matrix4x4 times(const Matrix4x4& a,const Matrix4x4& b)
        {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i)//a row
            {
                for (int j = 0; j < 4; ++j)//a col
                {
                    if (std::abs(a[j*4 + i]) <= std::numeric_limits<T>::epsilon())
                    {
                        continue;
                    }
                    for (int k = 0; k < 4; ++k)//b col
                    {
                        result[k*4 + i] += a[j*4 + i] * b[k*4 + j];
                    }
                }
            }
            return result;
        }
    };
}


#endif // MATRIX_H
