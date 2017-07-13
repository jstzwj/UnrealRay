#ifndef MATERIAL_H
#define MATERIAL_H


#include"Interaction.h"

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
        void ComputeScatteringFunctions(SurfaceInteraction *si, MemoryArena &arena,
                                        TransportMode mode,
                                        bool allowMultipleLobes) const;

      private:
        // MatteMaterial Private Data
        std::shared_ptr<Texture<Spectrum>> Kd;
        std::shared_ptr<Texture<Float>> sigma, bumpMap;
    };




}





#endif // MATERIAL_H
