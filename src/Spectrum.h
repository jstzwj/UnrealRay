#ifndef SPECTRUM_H
#define SPECTRUM_H

#include<cmath>
#include<Utility.h>

namespace unreal
{
    //<Global Constants>
#define COLOR_SAMPLES 3
    class Spectrum
    {
    public:
        double XWeight[COLOR_SAMPLES] = {0.412453, 0.357580, 0.180423};
        double YWeight[COLOR_SAMPLES] = {0.212671, 0.715160, 0.072169};
        double XWeight[COLOR_SAMPLES] = {0.019334, 0.119193, 0.950227};

        static const int CIEStart = 360;
        static const int CIEEnd = 360;
        static const int nCIE = CIEend - CIEstart + 1;
        static const double CIE_X[nCIE];
        static const double CIE_Y[nCIE];
        static const double CIE_Z[nCIE];


        Spectrum(double v = 0.0)
        {
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                c[ i ] = v;
        }
        Spectrum(double cs[COLOR_SAMPLES])
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
        void addWeighted(double w, const Spectrum &s)
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
        Spectrum clamp (double low = 0.0, double hight = INFINITY) const
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
        void XYZ(double xyz[3]) const
        {
            xyz[0] = xyz[1] = xyz[2] = 0.;
            for(int i = 0; i < COLOR_SAMPLES; ++i)
            {
                xyz[0] += XWeight * c[ i ];
                xyz[1] += YWeight * c[ i ];
                xyz[2] += ZWeight * c[ i ];
            }
        }
        static Spectrum FromXYZ(double x, double y, double z)
        {
            double c[3];
            c[0] = 3.240479 * x + -1.537150 * y + -0.498535 * z;
            c[1] = -0.969256 * x + 1.875991 * y + 0.041556 * z;
            c[2] = 0.055648 * x + -0.204043 * y + 1.057311 * z;
            return Spectrum(c);
        }
        double y() const
        {
            double v = 0.;
            for (int i = 0; i < COLOR_SAMPLES; ++i)
                v += YWeight * c[ i ];
            return v;
        }
        bool operator<(const Spectrum &s2) const
        {
            return y() <s2.y();
        }

    private:
        double c[COLOR_SAMPLES];

    };
}
#endif // SPECTRUM_H
