#ifndef SPECTRUM_H
#define SPECTRUM_H
namespace unreal
{
    //<Global Constants>
#define COLOR_SAMPLES 3
    class Spectrum
    {
    public:
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
    private:
        double c[COLOR_SAMPLES];
    };
}
#endif // SPECTRUM_H
