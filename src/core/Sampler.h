#pragma once
#ifndef UNREALRAY_SAMPLER
#define UNREALRAY_SAMPLER


#include<vector>
#include<memory>
#include<cstring>
#include<cstdint>
#include<random>

#include"Point.h"
namespace unreal
{
    struct Sample
    {
        Point2f pFilm;
        Float time;
    };

	class Sampler
	{
	public:
        // Sampler Interface
        virtual ~Sampler()=default;
        Sampler(int64_t samplesPerPixel):samplesPerPixel(samplesPerPixel) {}
        Sample getSample(const Point2i &pRaster)
        {
            Sample cs;
            cs.pFilm = (Point2f)pRaster + get2D();
            cs.time = get1D();
            return cs;
        }

        virtual void startPixel(const Point2i &p)
        {
            currentPixel = p;
            currentPixelSampleIndex = 0;
            // Reset array offsets for next pixel sample
            array1DOffset = array2DOffset = 0;
        }

        virtual Float get1D() = 0;
        virtual Point2f get2D() = 0;
        virtual void request1DArray()
        {
            //samples1DArraySizes.push_back(n);
            sampleArray1D=std::vector<Float>(samplesPerPixel);
        }

        virtual void request2DArray()
        {
            //samples2DArraySizes.push_back(n);
            sampleArray2D=std::vector<Point2f>(samplesPerPixel);
        }

        virtual int roundCount(int n) const { return n; }
        const Float *get1DArray(int n)
        {
            if (array1DOffset == sampleArray1D.size()) return nullptr;
            return &sampleArray1D[0];
        }

        const Point2f *get2DArray(int n)
        {
            if (array2DOffset == sampleArray2D.size()) return nullptr;
            return &sampleArray2D[0];
        }

        virtual bool startNextSample()
        {
            // Reset array offsets for next pixel sample
            array1DOffset = array2DOffset = 0;
            return ++currentPixelSampleIndex < samplesPerPixel;
        }
        //virtual std::unique_ptr<Sampler> clone(int seed) = 0;
        virtual bool setSampleNumber(int64_t sampleNum)
        {
            // Reset array offsets for next pixel sample
            array1DOffset = array2DOffset = 0;
            currentPixelSampleIndex = sampleNum;
            return currentPixelSampleIndex < samplesPerPixel;
        }

        int64_t currentSampleNumber() const { return currentPixelSampleIndex; }

        // Sampler Public Data
        const int64_t samplesPerPixel;

    protected:
        // Sampler Protected Data
        Point2i currentPixel;
        int64_t currentPixelSampleIndex;
        //std::vector<int> samples1DArraySizes, samples2DArraySizes;
        std::vector<Float> sampleArray1D;
        std::vector<Point2f> sampleArray2D;

      private:
        // Sampler Private Data
        size_t array1DOffset, array2DOffset;
	};
    class AverageSampler:public Sampler
    {
    public:
        AverageSampler(int64_t samplesPerPixel):Sampler(samplesPerPixel){}

        virtual void startPixel(const Point2i &p)
        {
            Sampler::startPixel(p);
            request1DArray();
            request2DArray();
        }
        virtual void request1DArray()
        {
            sampleArray1D=std::vector<Float>(samplesPerPixel);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0, 1);
            for (auto & each:sampleArray1D)
            {
                each=dis(gen);
            }

        }

        virtual void request2DArray()
        {
            sampleArray2D=std::vector<Point2f>(samplesPerPixel);
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0, 1);
            for (auto & each:sampleArray2D)
            {
                each=Point2f(dis(gen),dis(gen));
            }
        }

        Float get1D()
        {
            return sampleArray1D[currentPixelSampleIndex];
        }

        Point2f get2D()
        {
            return sampleArray2D[currentPixelSampleIndex];
        }
    };

}



#endif // !UNREALRAY_SAMPLER


