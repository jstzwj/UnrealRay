#pragma once
#ifndef UNREALRAY_SAMPLER
#define UNREALRAY_SAMPLER


#include<vector>
namespace unreal
{

    struct Sample
    {
        //<Sample Public Methods>
        //<Sample Method Definitions>
        Sample(SurfaceIntegrator *surf, VolumeIntegrator *vol, const Scene *scene)
        {
            surf->requestSamples(this, scene);
            vol->requestSamples(this, scene);
            //<Allocate storage for sample pointers>
            int nPtrs = n1D.size() + n2D.size();
            if (!nPtrs)
            {
                oneD = twoD = nullptr;
                return;
            }
            oneD = (double **)AllocateAligned(nPtrs * sizeof(double *));
            twoD = oneD + n1D.size();
            //<Compute total number of sample values needed>
            int totSamples = 0;
            for(u_int i = 0; i < n1D.size(); ++i)
                totSamples += n1D[i];
            for(u_int i = 0; i < n2D.size(); ++i)
                totSamples += n2D[i];
            //<Allocate storage for sample values>
            double *mem = (double *) AllocAligned(totSamples * sizeof(double));
            for(unsigned int i = 0; i < n1D.size(); ++i)
            {
                oneD[i] = mem;
                mem += n1D[i];
            }
            for(unsigned int i = 0; i < n2D.size(); ++i)
            {
                twoD[i] = mem;
                mem += 2 * n2D[i];
            }

        }
        unsigned int add1D(unsigned int num)
        {
            n1D.push_back(num);
            return n1D.size() -1;
        }
        unsigned int add2D(unsigned int num)
        {
            n2D.push_back(num);
            return n2D.size() -1;
        }

        //<Camera Sample Data>
        //<Integrator Sample Data> =
        std::vector<unsigned int> n1D, n2D;
        double **oneD, **twoD;
        //<Camera Sample Data>
        double imageX, imageY;
        double lensU, lensV;
        double time;
    };



	class Sampler
	{
	public:
        Sampler(int xstart, int xend, int ystart, int yend, int spp)
        {
            xPixelStart = xstart;
            xPixelEnd = xend;
            yPixelStart = ystart;
            yPixelEnd = yend;
            SamplesPerPixel = spp;
        }
        virtual ~Sampler()=default;
        //<Sampler Interface>
        virtual int roundSize(int size) const = 0;
        //<Sampler Interface>
        virtual bool GetNextSample(Sampler *sample) = 0;
        //<Sampler Interface>
        int totalSamples() const
        {
            return samplesPerPixel *
                (xPixelEnd - xPixelStart) * (yPixelEnd - yPixelStart);
        }

        //<Sampler Public Data>
        int xPixelStart, xPixelEnd, yPixelStart, yPixelEnd;
        int samplesPerPixel;
	};
}



#endif // !UNREALRAY_SAMPLER


