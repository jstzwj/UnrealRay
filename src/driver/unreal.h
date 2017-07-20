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

    public:
        Unreal()
        {

            std::shared_ptr<Shape> sphere_shape(new Sphere(Transform::eye(),false,0.5f,-0.5f,0.5f,270));
            std::shared_ptr<Primitive> sphere_primitive(new GeometricPrimitive(sphere_shape));
            std::vector<std::shared_ptr<Primitive>> shapes;
            shapes.push_back(sphere_primitive);

            std::shared_ptr<Primitive> aggregate(new GridAccel(shapes));
            std::vector<std::shared_ptr<Light>> v_light;
            scene=std::shared_ptr<Scene>(new Scene(aggregate,v_light));


            std::shared_ptr<Film> film(new QImageFilm({500,500},""));
            Transform look=Transform::lookAt({0,0,5},{0,0,0},{0,0,1});
            std::shared_ptr<OrthoCamera> camera(new OrthoCamera(look,Bounds2f({-1,-1},{1,1}),0,0,0,10,film));
            std::shared_ptr<Sampler> sampler(new AverageSampler(1));
            integrator=std::shared_ptr<SamplerIntegrator>(new WhittedIntegrator(10,camera,sampler));

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
