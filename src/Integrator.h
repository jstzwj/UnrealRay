#ifndef INTEGRATOR_H
#define INTEGRATOR_H


#include<memory>

#include"Scene.h"
#include"Sampler.h"
#include"Camera.h"

namespace unreal
{
    class Integrator
    {
    public:
        virtual void render(const Scene &scene) = 0;
    };

    class SamplerIntegrator : public Integrator
    {
    public:
        SamplerIntegrator(std::shared_ptr<Camera> camera,std::shared_ptr<Sampler> sampler)
            : camera(camera), sampler(sampler) { }
        virtual void preprocess(const Scene &scene, Sampler &sampler) { }
        virtual void render(const Scene &scene) override
        {
            preprocess(scene, *sampler);
            // Render image tiles in parallel
            {
                Sample * sample=nullptr;
                while(sampler->getNextSample(sample))
                {
                    // Generate camera ray for current sample
                    Ray ray;
                    double rayWeight =camera->generateRay(sample, &ray);

                    // Evaluate radiance along camera ray
                    Spectrum L(0.0);
                    if (rayWeight > 0) L = li(ray, scene, *tileSampler, arena);

                    // Issue warning if unexpected radiance value returned
                    if (L.HasNaNs()) {
                        L = Spectrum(0.0);
                    } else if (L.y() < -1e-5) {
                        L = Spectrum(0.0);
                    } else if (std::isinf(L.y())) {
                        L = Spectrum(0.0);
                    }
                    // Add camera ray's contribution to image
                    filmTile->AddSample(cameraSample.pFilm, L, rayWeight);
                }
            }
            // Merge image tile into _Film_
            camera->film->mergeFilmTile(std::move(filmTile));
            // Save final image after rendering
            camera->film->writeImage();

        }
    protected:
        std::shared_ptr<Camera> camera;
    private:
        std::shared_ptr<Sampler> sampler;
    };

    class WhittedIntegrator : public SamplerIntegrator
    {
      public:
        // WhittedIntegrator Public Methods
        WhittedIntegrator(int maxDepth, std::shared_ptr<const Camera> camera,std::shared_ptr<Sampler> sampler)
            : SamplerIntegrator(camera, sampler), maxDepth(maxDepth) {}
        Spectrum Li(const Ray &ray, const Scene &scene,Sampler &sampler, int depth) const
        {

        }

      private:
        // WhittedIntegrator Private Data
        const int maxDepth;
    };
}
#endif // INTEGRATOR_H
