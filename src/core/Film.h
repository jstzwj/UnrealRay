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
#include"Bounds.h"

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




    struct Pixel
    {
        Pixel () : L(0.0f)
        {
            alpha = 0.0f;
            weightSum =0;
        }
        Spectrum L;
        Float alpha;
        int weightSum;
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

    class QImageFilm : public Film
    {
    public:
            //< ImageFilm public Method>
        QImageFilm(const Point2i& res,const std::string &fn)
           : Film(res),img(res.x,res.y,QImage::Format_ARGB32)
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
            for(int y=yPixelStart;y<yPixelStart+yPixelCount;++y)
            {
                for(int x=xPixelStart;x<xPixelStart+xPixelCount;++x)
                {
                    Float rgb[3];
                    Spectrum cur_spectrum=pixels[y*xPixelCount+x].L;
                    cur_spectrum/=(Float)pixels[y*xPixelCount+x].weightSum;
                    cur_spectrum.ToRGB(rgb);
                    img.setPixelColor(QPoint(x,y),QColor::fromRgbF(qreal(rgb[0]),qreal(rgb[1]),qreal(rgb[2])) );
                }
            }
        }
        QImage getImage()
        {
            return img;
        }
    private:
        std::string filename;
        //Float cropWindow[4];

        int  xPixelStart, xPixelCount, yPixelStart, yPixelCount;
        std::vector<Pixel> pixels;
        QImage img;
        //std::vector<Float> filterTable;
    };

}




#endif // FILM_H
