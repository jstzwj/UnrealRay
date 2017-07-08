#ifndef UNREAL_H
#define UNREAL_H

#include<memory>

#include"../core/Bounds.h"
#include"../core/Light.h"
#include"../core/Vector.h"
#include"../core/Point.h"
#include"../core/Matrix.h"
#include"../core/Normal.h"
#include"../core/Spectrum.h"

#include"../core/Interaction.h"
#include"../core/Shape.h"
#include"../core/Primitive.h"


#include"../core/Scene.h"
#include"../core/Integrator.h"




namespace unreal
{
    class Unreal
    {
    public:
        Unreal()
        {
            std::vector<std::shared_ptr<Primitive>> shapes;
            shapes.push_back(new Sphere());
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
