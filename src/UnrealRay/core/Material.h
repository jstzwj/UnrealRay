#ifndef MATERIAL_H
#define MATERIAL_H


#include<memory>

#include"Interaction.h"
#include"Texture.h"

namespace unreal
{
    enum class TransportMode { Radiance, Importance };

    class Material
    {
    public:
        virtual void computeScatteringFunctions(SurfaceInteraction *si,TransportMode mode) const = 0;
        virtual ~Material()=default;
    };


    class MatteMaterial : public Material
    {
      public:
        // MatteMaterial Public Methods
        MatteMaterial(const std::shared_ptr<Texture<Spectrum>> &Kd,
                      const std::shared_ptr<Texture<Float>> &sigma,
                      const std::shared_ptr<Texture<Float>> &bumpMap)
            : Kd(Kd), sigma(sigma), bumpMap(bumpMap) {}
        virtual void computeScatteringFunctions(SurfaceInteraction *si,TransportMode mode) const override
        {
			// Perform bump mapping with _bumpMap_, if present
			// if (bumpMap) Bump(bumpMap, si);

			// Evaluate textures for _MatteMaterial_ material and allocate BRDF
			Normal3f ns= si->shading.n, ng= si->nHit;
			Vector3f ss= si->shading.dpdu.normalize(), ts= Vector3f(ns).cross(Vector3f(ss.x, ss.y, ss.z));

			si->bsdf = std::make_shared<BSDF>(ns,ng,ss,ts);
			si->bsdf->add(std::make_shared<Lambertian>(Spectrum(0.9f)));
			// si->bsdf->add(std::make_shared<SpecularReflection>(Spectrum(0.9f), std::make_shared<FresnelConductor>(0.3,0.2,0.5)));
        }

      private:
        // MatteMaterial Private Data
        std::shared_ptr<Texture<Spectrum>> Kd;
        std::shared_ptr<Texture<Float>> sigma, bumpMap;
    };




}





#endif // MATERIAL_H
