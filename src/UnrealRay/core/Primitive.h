#pragma once
#ifndef UNREALRAY_PRIMITIVE
#define UNREALRAY_PRIMITIVE


#include<vector>
#include<memory>
#include<stdexcept>
#include"Ray.h"
#include"Transform.h"
#include"Interaction.h"
#include"Shape.h"
#include"Material.h"


namespace unreal
{
	class Primitive
	{
	public:
        Primitive()=default;
        virtual ~Primitive()=default;
        virtual bool canIntersect() const{return true;}
        virtual bool intersect (const Ray &r,SurfaceInteraction *isect) const = 0 ;
        virtual bool intersectP (const Ray &r) const = 0 ;
	};

    class GeometricPrimitive : public Primitive
    {
    public:
        GeometricPrimitive(const std::shared_ptr<Shape> &s, const std::shared_ptr<Material> &m)
            :shape(s), material(m){}

        virtual ~GeometricPrimitive()=default;

        virtual bool canIntersect() const override{return true;}
        virtual bool intersect (const Ray &r,SurfaceInteraction *isect) const override
        {
            Float tHit;
            if (!shape->intersect(r, &tHit, isect)) return false;
			material->computeScatteringFunctions(isect, TransportMode::Importance);
            //isect->primitive = this;
            return true;
        }
        virtual bool intersectP (const Ray &r) const override
        {
            return shape->intersectP(r);
        }
    private:
        std::shared_ptr<Shape> shape;
        std::shared_ptr<Material> material;
    };

    class InstancePrimitive : public Primitive {
    public:
        InstancePrimitive(Primitive &i, const Transform &i2w)
        {
            instance = &i;
            InstanceToWorld = i2w;
            WorldToInstance = i2w.getInverse();
        }
        virtual ~InstancePrimitive()=default;
    private:
        Primitive * instance;
        Transform InstanceToWorld, WorldToInstance;
    };
    class Aggregate : public Primitive
    {
    public:
        //<Aggregate Public Methods>
        Aggregate()=default;
        virtual ~Aggregate()=default;

    };

    class GridAccel:public Aggregate
    {
    public:
        GridAccel(const std::vector<std::shared_ptr<Primitive>> &p)
            :primitives(p){}
        virtual ~GridAccel()=default;

        virtual bool canIntersect() const override{return true;}

        virtual bool intersect(const Ray &ray, SurfaceInteraction *isect) const override
        {
            bool min_rst=false;
            Float min_distant=INFINITY;
            SurfaceInteraction min_isect;
            for(const auto &each:primitives)
            {
                SurfaceInteraction isect;
                Float dis;
                bool rst;
                rst=each->intersect(ray,&isect);
                dis=::unreal::distance(ray.origin,isect.pHit);
                if(dis<min_distant)
                {
                    min_distant=dis;
                    min_isect=isect;
                    min_rst=rst;
                }
            }

			*isect = min_isect;

            return min_rst;
        }
        virtual bool intersectP(const Ray &ray) const override
        {
            bool rst=false;
            for(const auto &each:primitives)
            {
                if(rst==false)
                    rst=each->intersectP(ray);
                else
                    break;
            }
            return rst;
        }

    public:
        std::vector<std::shared_ptr<Primitive>> primitives;
    };





}







#endif // !UNREALRAY_PRIMITIVE
