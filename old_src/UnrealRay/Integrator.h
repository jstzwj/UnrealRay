#ifndef INTEGRATOR_H
#define INTEGRATOR_H


#include<memory>

#include"Scene.h"
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
        virtual void preprocess(const Scene &scene, Sampler &sampler) { }
        virtual void render(const Scene &scene) override
        {
            preprocess(scene, *sampler);
            // Render image tiles in parallel
            {
                Film *film=camera->film;
                while(sampler->startNextSample())
                {
                    // Generate camera ray for current sample
                    Ray ray;
                    double rayWeight =camera->generateRay(*sampler, &ray);

                    // Evaluate radiance along camera ray
                    Spectrum L(0.0);
                    if (rayWeight > 0) L = li(ray, scene, *sampler);

                    // Issue warning if unexpected radiance value returned
                    if (L.isNaN()) {
                        L = Spectrum(0.0);
                    } else if (L.y() < -1e-5) {
                        L = Spectrum(0.0);
                    } else if (std::isinf(L.y())) {
                        L = Spectrum(0.0);
                    }
                    // Add camera ray's contribution to image
                    film.addSample(*sample,ray, L, rayWeight);
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
        Spectrum li(const Ray &ray, const Scene &scene,Sampler &sampler) const
        {
            SurfaceInteraction inract;
            Spectrum L(0.0);
            if(!scene.intersect(ray, &inract))
            {
                //<Handle ray with nointersection>
                for (const auto& each_light:scene.lights)
                    L  +=  each_light->Le(ray);
                return L;
            }
            //<Compute emitted and reflected light at ray intersection point>

            //<Evaluate BSDF at hit point>
            //<Initialize common variables for Whittedintegrator>
            //<Compute emiited light if ray hit an area lightsource>
            L += inract.Le(wo);
            //<Add contribution of each light source>
            Vector wi;
            for(const auto& each_light:scene.lights)
            {
                Spectrum Li = each_light->sample_Li(p, &wi, &visibility);
                if(Li.black()) continue;
                Spectrum f = bsdf->f(wo, wi);
                if(!f.black() && visibility.unoccluded(scene))
                    L += f * Li * AbsDot(wi, n) * visibility.Transmittance(scene);
            }
            return L;
        }
      private:
        // WhittedIntegrator Private Data
        int maxDepth;
    };
}
#endif // INTEGRATOR_H
