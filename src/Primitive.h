#pragma once
#ifndef UNREALRAY_PRIMITIVE
#define UNREALRAY_PRIMITIVE


#include<vector>
#include<memory>
#include"Ray.h"
#include"Transform.h"
#include"Differentialgeometry.h"


namespace unreal
{

    struct Intersection
    {
        //<Intersection Public Methods>
        DifferentialGeometry dg;
        const Primitive *primitive;
        Transform WorldToObject;
    };

	class Primitive
	{
	public:
		Primitive();
		~Primitive();
        virtual bool canIntersect() const ;
        virtual bool intersect(const Ray &r, Intersection *in) const  = 0;
        virtual bool intersect (const Ray &r) const = 0 ;
        virtual void refine(std::vector<Primitive *> &refined) const ;
        void fullyRefine(std::vector<Primitive *> &refined) const
        {
            std::vector<Primitive *> todo;
            todo.push_back(const_cast<Primitive *>(this));
            while(todo.size())
            {
                //<Refine last primitive in todo list>
                Primitive* prim = todo.back();
                todo.pop_back();
                if(prim->canIntersect())
                    refined.push_back(prim);
                else
                    prim->refine(todo);
            }
        }
        virtual const AreaLight *getAreaLight() const = 0;
        virtual BSDF *getBSDF(const DifferentialGeometry &dg, const Transform &WorldToObject)const  = 0;
	};

    class GeometricPrimitive : public Primitive
    {
    public:
        //<GeometricPrimitive Public Methods>
        GeometricPrimitive(const std::shared_ptr<Shape> &s,const std::shared_ptr<Material> &m,AreaLight *a);
        virtual bool intersect(const Ray &r, Intersection *isect) const override
        {
            double thit;
            if(!shape->intersect(r, &thit, &isect->dg))
                return false;
            isect->primitive = this;
            isect->WorldToObject = shape->WorldToObject;
            r.maxt = thit;
            return true;
        }

        virtual BSDF * GetBSDF(const DifferentialGeometry &dg,const Transform &WorldToObject) const override
        {
                DifferentialGeometry dgs;
                dgs=shape->getShadingGeometry(WorldToObject.getInverse(), dg);
                return material->getBSDF(dg, dgs);
        }
    private:
        //<GeometricPrimitive Private Data>
        std::shared_ptr<Shape> shape;
        std::shared_ptr<Material> material;
        AreaLight *areaLight;
    }

}







#endif // !UNREALRAY_PRIMITIVE
