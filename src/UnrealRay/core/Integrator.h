#ifndef INTEGRATOR_H
#define INTEGRATOR_H


#include<memory>
#include<iostream>

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
        Integrator()=default;
        virtual Spectrum li(const Ray &ray, const Scene &scene,Sampler &sampler, int depth) const=0;
        virtual void render(const Scene &scene) = 0;
        virtual ~Integrator()=default;
    };

    class SamplerIntegrator : public Integrator
    {
    public:
        SamplerIntegrator(std::shared_ptr<Camera> camera,std::shared_ptr<Sampler> sampler)
            : Integrator(),camera(camera), sampler(sampler) { }
        virtual ~SamplerIntegrator()=default;
        virtual Spectrum li(const Ray &ray, const Scene &scene,Sampler &sampler,int depth) const=0;
        virtual void render(const Scene &scene) override
        {
            // Render image
            auto film=camera->film;
            Bounds2i filmBound=film->getFilmBounds();
            for(Point2i pixel:filmBound)
            {
                sampler->startPixel(pixel);
                do
                {
                    Sample sample=sampler->getSample(pixel);
                    // Generate camera ray for current sample
                    Ray ray;
                    Float rayWeight =camera->generateRay(sample, &ray);

                    // Evaluate radiance along camera ray
                    Spectrum L(0.0f);
                    if (rayWeight > 0) L = li(ray, scene, *sampler,0);

                    //如果相交
                    /*SurfaceInteraction isect;
                    if(scene.intersect(ray,&isect))
                    {
                        L=Spectrum(0.9f);
                    }*/

                    // Add camera ray's contribution to image
                    film->addSample(sample,L, rayWeight);
                }while(sampler->startNextSample());
            }
            // Save final image after rendering
            camera->film->writeImage();

        }
    public:
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
        virtual ~WhittedIntegrator()=default;
        virtual Spectrum li(const Ray &ray, const Scene &scene,Sampler &sampler,int depth) const override
        {
            Spectrum L(0.0f);
            // Find closest ray intersection or return background radiance
            SurfaceInteraction isect;
            if (!scene.intersect(ray, &isect))
            {
                for (const auto &light : scene.lights)
                    L += light->le(ray);
                return L;
            }

            // Compute emitted and reflected light at ray intersection point

            // Initialize common variables for Whitted integrator
            const Normal3f &n = isect.shading.n;
            Vector3f wo = isect.wo;

            // Compute scattering functions for surface interaction
            //isect.ComputeScatteringFunctions(ray, arena);
            //if (!isect.bsdf)
                //return Li(isect.SpawnRay(ray.d), scene, sampler, arena, depth);

            // Compute emitted light if ray hit an area light source
            L += isect.le(wo);

            // Add contribution of each light source
            for (const auto &light : scene.lights)
            {
                Vector3f wi;
                Float pdf;
                Spectrum Li =light->sample_Li(isect, sampler.get2D(), &wi, &pdf);
                if (Li.isBlack() || pdf == 0) continue;
                Spectrum f = isect.bsdf->f(wo, wi);
                if (!f.isBlack())
                    L += f * Li * std::abs(wi.dot((Vector3f)n)) / pdf;
            }
            if (depth + 1 < maxDepth)
            {
                // Trace rays for specular reflection and refraction
                //L += SpecularReflect(ray, isect, scene, sampler, arena, depth);
                //L += SpecularTransmit(ray, isect, scene, sampler, arena, depth);
            }
            return L;
        }
      private:
        // WhittedIntegrator Private Data
        int maxDepth;
    };
}
#endif // INTEGRATOR_H
