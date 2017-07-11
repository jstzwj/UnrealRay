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
        Integrator()=default;
        virtual Spectrum li(const Ray &ray, const Scene &scene,Sampler &sampler) const=0;
        virtual void render(const Scene &scene) = 0;
        virtual ~Integrator()=default;
    };

    class SamplerIntegrator : public Integrator
    {
    public:
        SamplerIntegrator(std::shared_ptr<Camera> camera,std::shared_ptr<Sampler> sampler)
            : Integrator(),camera(camera), sampler(sampler) { }
        virtual ~SamplerIntegrator()=default;
        virtual Spectrum li(const Ray &ray, const Scene &scene,Sampler &sampler) const=0;
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
                    //if (rayWeight > 0) L = li(ray, scene, *sampler);

                    //如果相交
                    SurfaceInteraction isect;
                    if(scene.intersect(ray,&isect))
                    {
                        L=Spectrum(0.9f);
                    }

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
        virtual Spectrum li(const Ray &ray, const Scene &scene,Sampler &sampler) const override
        {
            SurfaceInteraction isect;
            Spectrum L(0.0f);
            bool  hitSomething;
            hitSomething = scene.intersect(ray, &isect);
            if(!hitSomething)
            {
                //<Handle ray with nointersection>
                for ( const auto& each_light:scene.lights)
                    L  +=  each_light.Le(ray);
                //if(alpha && !L.Black()) *alpha = 1.;
                return L;
            }
            else
            {
                //<Initialize alpha for ray hit>
                //if (alpha) *alpha = 1.0f;

                //<Compute emitted and reflected light at ray intersection point>
                //<Evaluate BSDF at hit point>
                BSDF *bsdf = isect.getBSDF(ray);
                //<Initialize common variables for Whitted integrator>
                const Point3f &p = bsdf->dgShading.p;
                const Normal3f &n = bsdf->dgShading.nn;
                Vector3f wo = -ray.d;
                //<Compute emitted light if ray hit an area light source>
                //<Add contribution of each light source>
                if (rayDepth++ <  maxDepth)
                {
                    //<Trace rays for specular reflection and refraction>
                }
                --rayDepth;
            }
           return L;
        }
      private:
        // WhittedIntegrator Private Data
        int maxDepth;
        mutable int rayDepth;
    };
}
#endif // INTEGRATOR_H
