#pragma once
#ifndef UNREALRAY_LIGHT
#define UNREALRAY_LIGHT


#include<algorithm>

#include"Transform.h"
#include"Spectrum.h"
#include"Scene.h"
#include"Point.h"
#include"Primitive.h"
#include"Utility.h"

namespace unreal
{
    /*
    class VisibilityTester
    {
      public:
        VisibilityTester() {}
        // VisibilityTester Public Methods
        VisibilityTester(const Interaction &p0, const Interaction &p1)
            : p0(p0), p1(p1) {}
        const interaction &P0() const { return p0; }
        const interaction &P1() const { return p1; }
        bool unoccluded(const Scene &scene) const;
        Spectrum tr(const Scene &scene, Sampler &sampler) const;

      private:
        Interaction p0, p1;
    };
    */
    // LightFlags Declarations
    enum class LightFlags : int {
        DeltaPosition = 1,
        DeltaDirection = 2,
        Area = 4,
        Infinite = 8
    };

	class Light
	{
	public:
        Light(const Transform &l2w, int ns = 1)
                    : nSamples(std::max(1, ns)), LightToWorld(l2w),WorldToLight(l2w.getInverse()) {}
        virtual ~Light()=default;

        /*virtual Spectrum sample_L(const Point &p, Vector *wi,VisibilityTester *vis) const = 0;*/
        virtual Spectrum power() const = 0;
        virtual bool isDeltaLight() const = 0;

        const Transform LightToWorld, WorldToLight;
        const int nSamples;
	};

    // PointLight Declarations
    class PointLight : public Light
    {
      public:
        // PointLight Public Methods
        PointLight(const Transform &LightToWorld, const Spectrum &I)
            : Light(LightToWorld, (int)LightFlags::DeltaPosition),
              pLight(LightToWorld.transform(Point3f(0, 0, 0))),
              I(I){}
        /*Spectrum sample_Li(const Interaction &ref, const Point &u, Vector *wi,
                           double *pdf, VisibilityTester *vis) const;*/
        Spectrum power() const
        {
            return I * 4.0 * PI;
        }
        /*Spectrum sample_Le(const Point &u1, const Point &u2, double time,
                           Ray *ray, Normal *nLight, double *pdfPos,
                           double *pdfDir) const;*/

      private:
        // PointLight Private Data
        const Point3f pLight;
        const Spectrum I;
    };
}

#endif // !UNREALRAY_LIGHT


