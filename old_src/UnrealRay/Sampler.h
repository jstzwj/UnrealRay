#pragma once
#ifndef UNREALRAY_SAMPLER
#define UNREALRAY_SAMPLER


#include<vector>
#include<memory>
#include<cstring>
#include<cstdint>

#include"Point.h"
namespace unreal
{
    struct Sample
    {
        Point pFilm;
        double time;
    };

	class Sampler
	{
	public:
        // Sampler Interface
        virtual ~Sampler()=default;
        Sampler(int64_t samplesPerPixel):samplesPerPixel(samplesPerPixel) {}
        virtual void startPixel(const Point &p)
        {
            currentPixel = p;
            currentPixelSampleIndex = 0;
            // Reset array offsets for next pixel sample
            array1DOffset = array2DOffset = 0;
        }

        virtual double get1D() = 0;
        virtual Point get2D() = 0;
        void request1DArray(int n)
        {
            samples1DArraySizes.push_back(n);
            sampleArray1D.push_back(std::vector<double>(n * samplesPerPixel));
        }

        void request2DArray(int n)
        {
            samples2DArraySizes.push_back(n);
            sampleArray2D.push_back(std::vector<Point>(n * samplesPerPixel));
        }

        virtual int roundCount(int n) const { return n; }
        const double *get1DArray(int n)
        {
            if (array1DOffset == sampleArray1D.size()) return nullptr;
            return &sampleArray1D[array1DOffset++][currentPixelSampleIndex * n];
        }

        const Point *get2DArray(int n)
        {
            if (array2DOffset == sampleArray2D.size()) return nullptr;
            return &sampleArray2D[array2DOffset++][currentPixelSampleIndex * n];
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
        Point currentPixel;
        int64_t currentPixelSampleIndex;
        std::vector<int> samples1DArraySizes, samples2DArraySizes;
        std::vector<std::vector<double>> sampleArray1D;
        std::vector<std::vector<Point>> sampleArray2D;

      private:
        // Sampler Private Data
        size_t array1DOffset, array2DOffset;
	};

}



#endif // !UNREALRAY_SAMPLER


