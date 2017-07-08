#ifndef INTEGRATOR_H
#define INTEGRATOR_H


#include<memory>

#include"Scene.h"
#include"Spectrum.h"
#include"Sampler.h"
#include"Camera.h"
#include"Bsdf.h"

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
        virtual Spectrum li(const Ray &ray, const Scene &scene,Sampler &sampler) const;
        virtual void render(const Scene &scene) override
        {
            // Render image
            Film *film=camera->film;
            Bounds2i filmBound=film->getFilmBounds();
            for(Point2i pixel:filmBound)
            {
                while(sampler->startNextSample())
                {
                    Sample sample=sampler->getSample(pixel);
                    // Generate camera ray for current sample
                    Ray ray;
                    double rayWeight =camera->generateRay(sample, &ray);

                    // Evaluate radiance along camera ray
                    Spectrum L(0.0);
                    if (rayWeight > 0) L = li(ray, scene, *sampler);

                    // Issue warning if unexpected radiance value returned
                    /*if (L.isNaN()) {
                        L = Spectrum(0.0);
                    } else if (L.y() < -1e-5) {
                        L = Spectrum(0.0);
                    } else if (std::isinf(L.y())) {
                        L = Spectrum(0.0);
                    }*/
                    // Add camera ray's contribution to image
                    film->addSample(sample,ray, L, rayWeight);
                }
            }
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
        WhittedIntegrator(int maxDepth, std::shared_ptr<Camera> camera,std::shared_ptr<Sampler> sampler)
            : SamplerIntegrator(camera, sampler), maxDepth(maxDepth) {}
        virtual Spectrum li(const Ray &ray, const Scene &scene,Sampler &sampler) const override
        {

        }
      private:
        // WhittedIntegrator Private Data
        int maxDepth;
    };
}
#endif // INTEGRATOR_H
