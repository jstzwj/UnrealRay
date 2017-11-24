#ifndef FILM_H
#define FILM_H

#include<string>
#include<vector>
#include<stdexcept>
#include<cmath>
#include<cstdio>
#include"Ray.h"
#include"Sampler.h"
#include"Filter.h"
#include"Spectrum.h"
#include"Bounds.h"
#include"Pixel.h"

#include"../output/Bmp.h"

namespace unreal
{
    class Film
    {
    public:
        //<Film Public Data>
        const Point2i fullResolution;
        //<Film Interface>
        Film(const Point2i &res)
            : fullResolution(res)
        {}
        //<Film Interface>
        virtual void addSample(const Sample &sample, const Spectrum &L, Float sampleWeight=1.0f) = 0;
        virtual void writeImage() = 0;
        Bounds2i getFilmBounds() const
        {return Bounds2i(Point2i(0,0),fullResolution);}
        //<Film Interface> +=
        /*virtual void getSampleExtent(int *xstart , int *xend,int *ystart , int *yend) const = 0;*/

        virtual ~Film()=default;
    };


    class ImageFilm : public Film
    {
    public:
        //< ImageFilm public Method>
        ImageFilm(const Point2i& res,const std::string &fn)
           : Film(res)
        {
            //filter = filt;
            //memcpy(cropWindow, crop, 4 * sizeof(float));
            filename = fn;
            //<Compute film image extent>
            xPixelStart = 0;
            xPixelCount = fullResolution.x;
            yPixelStart = 0;
            yPixelCount = fullResolution.y;
            //<Allocate film image storage>
            pixels = std::vector<Pixel>(xPixelCount*yPixelCount);
        }
        virtual void addSample(const Sample &sample,const Spectrum &L, Float sampleWeight=1.0f)override
        {
                //<Compute sample's raster extent>
                //<Loop over filter support and add sample to pixel arrays>
                //<Possibly write out in-progress image>
        }
        virtual void writeImage()override
        {

        }

    private:
        std::string filename;
        //Float cropWindow[4];

        int  xPixelStart, xPixelCount, yPixelStart, yPixelCount;

        std::vector<Pixel> pixels;
        std::vector<Float> filterTable;
    };

    class BmpImageFilm : public Film
    {
    public:
        //< ImageFilm public Method>
		BmpImageFilm(const Point2i& res,const std::string &fn)
           : Film(res)
        {
            filename = fn;

            xPixelStart = 0;
            xPixelCount = fullResolution.x;
            yPixelStart = 0;
            yPixelCount = fullResolution.y;

            pixels = std::vector<Pixel>(xPixelCount*yPixelCount);

        }
        virtual void addSample(const Sample &sample,const Spectrum &L, Float sampleWeight=1.0f)override
        {
            int index=(int)sample.pFilm.x+(int)(sample.pFilm.y)*xPixelCount;
            pixels[index].L+=L*sampleWeight;
            ++pixels[index].weightSum;
        }
        virtual void writeImage()override
        {
			std::FILE * file = nullptr;
#ifdef _MSC_VER
			fopen_s(&file, filename.c_str(), "wb");
#else
			file = std::fopen(path.c_str(), "wb");
#endif // _MSC_VER
			if (file == nullptr)
			{
				throw std::runtime_error("fail to open file.");
			}

			stream::FileOutputStream stream(file);
			BmpWriter writer(stream, xPixelCount, yPixelCount);
			writer.write(pixels);

			std::fclose(file);
        }
    private:
        std::string filename;
        //Float cropWindow[4];

        int  xPixelStart, xPixelCount, yPixelStart, yPixelCount;
        std::vector<Pixel> pixels;
        //std::vector<Float> filterTable;
    };

}




#endif // FILM_H
