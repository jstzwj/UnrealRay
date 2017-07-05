#pragma once
#ifndef UNREALRAY_SCENE
#define UNREAlrAY_SCENE
#include<vector>
#include<memory>
#include"Light.h"
#include"Sampler.h"
#include"Film.h"
#include"Primitive.h"
#include"Spectrum.h"

namespace unreal
{
	class Scene
	{
	public:
        // Scene Public Methods
        Scene(std::shared_ptr<Primitive> aggregate, std::vector<std::shared_ptr<Light>> &lights)
            : lights(lights), aggregate(aggregate)
        {
            // Scene Constructor Implementation
            for (const auto &light : lights)
            {
                light->Preprocess(*this);
            }
        }
        bool intersect(const Ray &ray, SurfaceInteraction *isect) const
        {
            return aggregate->intersect(ray,isect);
        }
        bool intersectP(const Ray &ray) const
        {
            return aggregate->intersectP(ray);
        }
        bool intersectTr(Ray ray, Sampler &sampler, SurfaceInteraction *isect,Spectrum *transmittance) const
        {

        }

		int progress;
        std::vector<std::shared_ptr<Light> > lights;
        std::shared_ptr<Primitive> aggregate;
		
	public:
	};
}


#endif // !UNREALRAY_SCENE


