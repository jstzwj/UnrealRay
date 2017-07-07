#ifndef UNREAL_H
#define UNREAL_H

#include<memory>
#include"../core/Scene.h"
#include"../core/Integrator.h"
namespace unreal
{
    class Unreal
    {
    public:
        Unreal()
        {
            GridAccel * aggregate=new GridAccel();
            std::vector<std::shared_ptr<Light>> v_light;
            scene=new Scene(aggregate,v_light);

            OrthoCamera * camera=new OrthoCamera();
            Sampler * sampler=new Sampler();
            integrator=new SamplerIntegrator(camera,sampler);
        }
        void render()
        {
            integrator->render(*scene);
        }
        virtual ~Unreal()=default;
        std::shared_ptr<Scene> scene;
        std::shared_ptr<SamplerIntegrator> integrator;
    };
}



#endif // UNREAL_H
