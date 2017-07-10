#ifndef SPECTRUM_H
#define SPECTRUM_H

#include<cmath>
#include"Type.h"
#include"Utility.h"

namespace unreal
{
    //<Global Constants>
#define COLOR_SAMPLES 3
    class Spectrum
    {
    public:
        Float XWeight[COLOR_SAMPLES] = {0.412453f, 0.357580f, 0.180423f};
        Float YWeight[COLOR_SAMPLES] = {0.212671f, 0.715160f, 0.072169f};
        Float ZWeight[COLOR_SAMPLES] = {0.019334f, 0.119193f, 0.950227f};

        static const int CIEStart = 360;
        static const int CIEEnd = 360;
        static const int nCIE = CIEEnd - CIEStart + 1;
        static const Float CIE_X[nCIE];
        static const Float CIE_Y[nCIE];
        static const Float CIE_Z[nCIE];


        Spectrum(Float v = 0.0)
        {
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                c[ i ] = v;
        }
        Spectrum(Float cs[COLOR_SAMPLES])
        {
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                c[ i ]= cs[ i ];
        }
        Spectrum &operator += (const Spectrum &other)
        {
                for (int i = 0; i < COLOR_SAMPLES; ++i)
                    c[ i ] += other.c[ i ];
                return *this;
        }

        Spectrum operator + (const Spectrum &other)  const
        {
            Spectrum ret = *this;
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                ret.c[ i ]  += other.c[ i ];
            return ret;
        }
        Spectrum operator*(Float a) const
        {
            Spectrum ret = *this;
            for (int i = 0; i < 16; ++i)
                ret.c[i] *= a;
            return ret;
        }
        void addWeighted(Float w, const Spectrum &s)
        {
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                c[ i ]  += w * s.c[ i ];
        }
        bool operator == (const Spectrum &sp) const
        {
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                if(c [ i ] != sp.c[ i ]) return false;
            return true;
        }
        bool black () const
        {
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                if(c[ i ]  != 0.0) return false;
            return true;
        }
        Spectrum sqrt () const
        {
            Spectrum ret;
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                ret.c [ i ] = std::sqrt(c[ i ]);
            return ret;
        }
        Spectrum pow (const Spectrum &e) const
        {
            Spectrum ret;
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                ret.c [ i ]= c [ i ] > 0 ? std::pow(c[ i ], e.c[ i ]) : 0.f;
            return ret;
        }
        Spectrum clamp (Float low = 0.0, Float hight = INFINITY) const
        {
            Spectrum ret;
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                ret.c [ i ] = ::unreal::clamp(c[ i ], low, hight);
            return ret;
        }
        bool isNaN () const
        {
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                if(std::isnan(c[ i ])) return false;
            return true;
        }
        void ToRGB(Float rgb[3]) const
        {
            Float xyz[3];
            ToXYZ(xyz);
            XYZToRGB(xyz, rgb);
        }
        static void XYZToRGB(const Float xyz[3], Float rgb[3])
        {
            rgb[0] = 3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
            rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
            rgb[2] = 0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
        }
        void ToXYZ(Float xyz[3]) const
        {
            xyz[0] = xyz[1] = xyz[2] = 0.f;
            for(int i = 0; i < COLOR_SAMPLES; ++i)
            {
                xyz[0] += XWeight[i] * c[i];
                xyz[1] += YWeight[i] * c[i];
                xyz[2] += ZWeight[i] * c[i];
            }
        }
        static Spectrum FromXYZ(Float x, Float y, Float z)
        {
            Float c[3];
            c[0] = 3.240479f * x + -1.537150f * y + -0.498535f * z;
            c[1] = -0.969256f * x + 1.875991f * y + 0.041556f * z;
            c[2] = 0.055648f * x + -0.204043f * y + 1.057311f * z;
            return Spectrum(c);
        }
        Float y() const
        {
            Float v = 0.;
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                v += YWeight[i] * c[i];
            return v;
        }
        bool operator <(const Spectrum &s2) const
        {
            return y() <s2.y();
        }

    private:
        Float c[COLOR_SAMPLES];

    };
}
#endif // SPECTRUM_H
