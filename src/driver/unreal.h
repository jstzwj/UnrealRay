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
            scene=new Scene();
            integrator=new SamplerIntegrator();
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
