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

#include"../core/Material.h"
#include"../core/Texture.h"




namespace unreal
{
    class Unreal
    {
    public:

    public:
        Unreal()
        {
			std::shared_ptr<Texture<Spectrum>> constantTextureSpecturm(new ConstantTexture<Spectrum>(1.0f));
			std::shared_ptr<Texture<Float>> constantTextureFloat(new ConstantTexture<Float>(1.0f));
			std::shared_ptr<Material> matte(new MatteMaterial(constantTextureSpecturm, constantTextureFloat, constantTextureFloat));

            std::shared_ptr<Shape> sphere_shape(new Sphere(Transform::eye(),false,0.5f,-0.5f,0.5f,360));
            std::shared_ptr<Primitive> sphere_primitive(new GeometricPrimitive(sphere_shape, matte));
            std::vector<std::shared_ptr<Primitive>> shapes;
            shapes.push_back(sphere_primitive);

            std::shared_ptr<Primitive> aggregate(new GridAccel(shapes));
            std::vector<std::shared_ptr<Light>> v_light;
			Float light_color1[3] = { 0.3f,0.3f,1.9f };
			Float light_color2[3] = { 1.9f,0.3f,0.3f };
			v_light.push_back(std::make_shared<PointLight>(Transform::translate({0,1,1}), Spectrum(light_color1)));
			v_light.push_back(std::make_shared<PointLight>(Transform::translate({ 0,-1,1 }), Spectrum(light_color2)));
            scene=std::shared_ptr<Scene>(new Scene(aggregate,v_light));


            std::shared_ptr<Film> film(new BmpImageFilm({500,500},"a.bmp"));
            Transform look=Transform::lookAt({0,0,1},{0,0,0},{0,0,1});
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
