#pragma once
#ifndef UNREALRAY_LIGHT
#define UNREALRAY_LIGHT


#include<algorithm>

#include"Transform.h"
#include"Spectrum.h"
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

        virtual Spectrum sample_Li(const Interaction &ref, const Point2f &u, Vector3f *wi, Float *pdf) const = 0;
        virtual Spectrum power() const = 0;
        virtual bool isDeltaLight() const = 0;
        virtual Spectrum le(const Ray &r) const=0;

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

		virtual Spectrum sample_Li(const Interaction &ref, const Point2f &u, Vector3f *wi, Float *pdf) const
		{
			*wi = ((Vector3f)(pLight - ref.pHit)).normalize();
			*pdf = 1.0f;
			// visibility->SetSegment(p, lightPos);
			return I / distanceSquared(pLight, ref.pHit);
		}

		virtual Spectrum power() const
		{
			return I * 4.0 * PI;
		}
		virtual bool isDeltaLight() const
		{
			return true;
		}

		virtual Spectrum le(const Ray &r) const
		{
			return Spectrum(0.0f);
		}

      private:
        // PointLight Private Data
        const Point3f pLight;
        const Spectrum I;
    };
}

#endif // !UNREALRAY_LIGHT


