#ifndef FILM_H
#define FILM_H

#include<QImage>
#include<string>
#include<vector>
#include<cmath>
#include"Ray.h"
#include"Sampler.h"
#include"Filter.h"
#include"Spectrum.h"

namespace unreal
{
    class Film
    {
    public:
        //<Film Public Data>
        const int xResolution, yResolution;
        //<Film Interface>
        Film(int xres, int yres)
            : xResolution(xres), yResolution(yres)
        {
        }
        //<Film Interface>
        virtual void addSample(const Sample &sample, const Ray &ray,const Spectrum &L, Float alpha=1.0f) = 0;
        virtual void writeImage() = 0;
        //<Film Interface> +=
        /*virtual void getSampleExtent(int *xstart , int *xend,int *ystart , int *yend) const = 0;*/

        virtual ~Film()=default;
    };




    struct Pixel
    {
        Pixel () : L(0.f)
        {
            alpha = 0.f;
            weightSum =0.f;
        }
        Spectrum L;
        double alpha, weightSum;
    };

    class ImageFilm : public Film
    {
    public:
            //< ImageFilm public Method>
        ImageFilm(int xres, int yres, const double crop[4],
                   const std::string &fn, bool premult, int wf)
           : Film(xres, yres)
        {
            //filter = filt;
            memcpy(cropWindow, crop, 4 * sizeof(float));
            filename = fn;
            premultiplyAlpha = premult;
            writeFrequency = sampleCount = wf;
            //<Compute film image extent>
            xPixelStart = std::ceil (xResolution * cropWindow[0]);
            xPixelCount = std::ceil (xResolution * cropWindow[1]) - xPixelStart;
            yPixelStart = std::ceil (yResolution * cropWindow[2]);
            yPixelCount = std::ceil (yResolution * cropWindow[3]) - yPixelStart;
            //<Allocate film image storage>
            pixels = std::vector<Pixel>(xPixelCount*yPixelCount);
            //<Precompute filter weight table>
            /*
#define FILTER_TABLE_SIZE 16
            filterTable = std::vector<double>(FILTER_TABLE_SIZE * FILTER_TABLE_SIZE);
            double *ftp = &filterTable.begin();
            for ( int y = 0; y < FILTER_TABLE_SIZE; ++y)
            {
                double fy = ( (double ) y + 0.5) *
                        filter->yWidth / FILTER_TABLE_SIZE;

                for (int x = 0; x < FILTER_TABLE_SIZE; ++x)
                {
                    double fx = ( ( double ) x + 0.5) *
                        filter->xWidth / FILTER_TABLE_SIZE;
                    *ftp++ = filter->Evaluate(fx,fy);
                }
            }
            */
        }
        virtual void addSample(const Sample &sample, const Ray &ray,const Spectrum &L, Float alpha=1.0f)override
        {
                //<Compute sample's raster extent>
                //<Loop over filter support and add sample to pixel arrays>
                //<Possibly write out in-progress image>
        }
    private:
        //<ImageFilm Private Data>
        //Filter *filter;
        int writeFrequency, sampleCount;
        std::string filename;
        bool premultiplyAlpha;
        double cropWindow[4];

        int  xPixelStart, xPixelCount, yPixelStart, yPixelCount;

        std::vector<Pixel> pixels;
        std::vector<double> filterTable;
    };

}




#endif // FILM_H
