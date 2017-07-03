#ifndef SHAPE_H
#define SHAPE_H
#include<vector>
#include<stdexcept>
#include<cmath>
#include"Matrix.h"
#include"Ray.h"
#include"Transform.h"
#include"Differentialgeometry.h"
#include"Utility.h"

namespace unreal {
    class Shape
    {
    private:
        const Transform ObjectToWorld, WorldToObject;
        const bool reverseOrientation, transformSwapsHandedness;
    public:
        Shape(const Transform &o2w, bool ro)
            : ObjectToWorld(o2w), WorldToObject(o2w.GetInverse()),
            reverseOrientation(ro),transformSwapsHandedness(o2w.swapsHandedness()) {}
        virtual bool canIntersect() const { return true; }
        virtual void refine(std::vector<Shape> &refined) const
        {throw std::runtime_error("Unimplemented Shape::Refine() method called.");}
        virtual bool intersect(const Ray &ray, double *tHit, DifferentialGeometry *dg) const
        {
            throw std::runtime_error("Unimplemented Shape: Interact() method called.");
            return false;
        }
        virtual bool intersectP(const Ray &ray) const
        {
            throw std::runtime_error("Unimplemented Shape: InteractP() method called.");
            return false;
        }
        virtual DifferentialGeometry getShadingGeometry(const Transform &obj2world,const DifferentialGeometry &dg) const
        {
            return dg;
        }
        virtual double area() const
        {
            throw std::runtime_error("Unimplemented Shape::Area() method called.");
            return 0.0;
        }

    };

    class Sphere : public Shape
    {
        public:
            Sphere(const Transform &o2w, bool ro, double rad,double z0, double z1, double pm)
                : Shape(o2w, ro)
            {
                radius = rad;
                zmin = clamp(min(z0, z1), -radius, radius);
                zmax = clamp(max(z0, z1), -radius, radius);
                thetaMin = std::acos(clamp(zmin/radius,0,1));
                thetaMax = std::acos(clamp(zmax/radius,0,1));
                phiMax = radians(clamp(pm, 0.0f, 360.f));
            }
            virtual bool intersect(const Ray &r, double *tHit, DifferentialGeometry *dg) const override
            {
                double phi;
                Point phit;
                //<Transform Ray to object space>
                Ray ray;
                ray=WorldToObject.transform(r);
                //<Compute quadratic sphere coefficients>
                double A = ray.direction.x*ray.direction.x + ray.direction.y*ray.direction.y + ray.direction.z*ray.direction.z;
                double B = 2 * (ray.direction.x*ray.origin.x + ray.direction.y*ray.origin.y + ray.direction.z*ray.origin.z);
                double C = ray.origin.x*ray.origin.x + ray.origin.y*ray.origin.y + ray.origin.z*ray.origin.z - radius * radius;

                //<Solve quadratic equation for t values>
                double t0, t1;
                if(!quadratic(A, B, C, &t0, &t1))
                    return false;

                //<Compute intersection distance along ray>
                if (t0 > ray.maxt || t1 < ray.mint)
                    return false;
                double thit = t0;
                if (t0 < ray.mint)
                {
                    thit = t1;
                    if (thit > ray.maxt)
                        return false;
                }

                //<Compare sphere hit position and Φ>


                //<Test sphere intersection against clipping parameters>
                //<Finding parametric representation of sphere hit>
                //<Initialize DifferentialGeometry from parametric information>
                //<Update tHit for quadric intersection>
                return true;
            }
        private:
            double radius;
            double phiMax;
            double zmin, zmax;
            double thetaMin, thetaMax;

    };

}




#endif // SHAPE_H
