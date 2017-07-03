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
                //两个解在一侧的情况
                if (t0 > ray.maxt || t1 < ray.mint)
                    return false;

                //两个解在两侧的情况
                if (t0 < ray.mint&&t1 > ray.maxt)
                {
                    return false;
                }
                double thit = t0;
                //只有远端相交的情况
                if(t0 < ray.mint&&t1 < ray.maxt)
                    thit = t1;

                Point phit;
                double phi;
                //<Compare sphere hit position and Φ>
                phit = ray.getPoint(thit);//交点
                phi = std::atan2(phit.y, phit.x);//Φ
                if(phi < 0.0) phi += 2.0 * PI;

                //<Test sphere intersection against clipping parameters>
                if (phit.z < zmin || phit.z > zmax || phi > phiMax)
                {
                    //只有一个解是t1（被裁剪）
                    if(thit == t1) return false;
                    //只有一个解是t0（被裁剪）
                    if(t1 > ray.maxt) return false;
                    //有两个解，t0被裁剪，测试t1
                    thit = t1;
                    //<Compare sphere hit position and Φ>
                    phit = ray.getPoint(thit);//交点
                    phi = std::atan2(phit.y, phit.x);//Φ
                    if(phi < 0.0) phi += 2.0 * PI;
                    //测试t1是否也被裁剪
                    if (phit.z < zmin || phit.z > zmax || phi > phiMax)
                        return false;
                }

                //<Finding parametric representation of sphere hit>
                double u = phi / phiMax;
                double theta = std::acos(phit.z / radius);
                double v = (theta - thetaMin) / (thetaMax - thetaMin);
                //<Compute sphere dp/du and dp/dv>

                double zradius = sqrtf(phit.x * phit.x + phit.y * phit.y);
                double invzradius = 1.f / zradius;
                double cosphi = phit.x * invzradius;
                double sinphi  = phit.y * invzradius;
                Vector dpdu(-phiMax * phit.y,phiMax * phit.x,0);

                Vector dpdv=Vector(phit.z * cosphi,
                                   phit.z * sinphi,
                                   -radius * std::sin(thetaMin+v*(thetaMax-thetaMin)))
                        *(thetaMax - thetaMin);
                //<Compute sphere dn/du and dn/dv>
                Vector d2Pduu = -phiMax * phiMax * Vector(phit.x, phit.y, 0);
                Vector d2Pduv  = (thetaMax - thetaMin) * phit.z * phiMax *
                                Vector(-sinphi, cosphi, 0.);
                Vector d2Pdvv  =  -(thetaMax - thetaMin) * (thetaMax - thetaMin) *
                                Vector(phit.x, phit.y, phit.z);
                //< Compute coefficients for fundamental forms>
                double E = dpdu.dot(dpdu);
                double F = dpdu.dot(dpdv);
                double G = dpdv.dot(dpdv);
                Vector N = dpdu.cross(dpdv);
                double e = N.dot(d2Pduu);
                double f = N.dot(d2Pduv);
                double g = N.dot(d2Pdvv);
                //<Compute dn/du and dn/dv from fundamental form coefficients>
                double invEGF2 = 1.f / (E * G - F * F);
                Vector dndu = (f * F - e * G) * invEGF2 * dpdu + (e * F - f * E) * invEGF2 * dpdv;
                Vector dndv = (g* F - f * G) * invEGF2 * dpdu + (f * F - g * E) * invEGF2 * dpdv;


                //<Initialize DifferentialGeometry from parametric information>
                *dg = DifferentialGeometry(ObjectToWorld(phit),
                                   ObjectToWorld(dpdu),
                                   ObjectToWorld(dpdv),
                                   ObjectToWorld(dndu),
                                   ObjectToWorld(dndv),
                                   u,v,this);

                //〈Update tHit for quadric intersection>
                *tHit = thit;
                return true;
            }
            virtual bool intersectP(const Ray &r) const override
            {

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
                //两个解在一侧的情况
                if (t0 > ray.maxt || t1 < ray.mint)
                    return false;

                //两个解在两侧的情况
                if (t0 < ray.mint&&t1 > ray.maxt)
                {
                    return false;
                }
                double thit = t0;
                //只有远端相交的情况
                if(t0 < ray.mint&&t1 < ray.maxt)
                    thit = t1;

                Point phit;
                double phi;
                //<Compare sphere hit position and Φ>
                phit = ray.getPoint(thit);//交点
                phi = std::atan2(phit.y, phit.x);//Φ
                if(phi < 0.0) phi += 2.0 * PI;

                //<Test sphere intersection against clipping parameters>
                if (phit.z < zmin || phit.z > zmax || phi > phiMax)
                {
                    //只有一个解是t1（被裁剪）
                    if(thit == t1) return false;
                    //只有一个解是t0（被裁剪）
                    if(t1 > ray.maxt) return false;
                    //有两个解，t0被裁剪，测试t1
                    thit = t1;
                    //<Compare sphere hit position and Φ>
                    phit = ray.getPoint(thit);//交点
                    phi = std::atan2(phit.y, phit.x);//Φ
                    if(phi < 0.0) phi += 2.0 * PI;
                    //测试t1是否也被裁剪
                    if (phit.z < zmin || phit.z > zmax || phi > phiMax)
                        return false;
                }
                return true;
            }
            double area() const
            {
                return phiMax * radius * (zmax - zmin);
            }
        private:
            double radius;
            double phiMax;
            double zmin, zmax;
            double thetaMin, thetaMax;

    };


    class TriangleMesh : public Shape
    {
    public:
        //<TriangleMesh Public Methods>
        TriangleMesh(const Transform &o2w, bool ro, int nt, int nv,
                const int *vi, const Point *P, const Normal *N, const  Vector *S, float *uv)
                : Shape(o2w, ro)
        {
                ntris = nt;
                nverts = nv;
                vertexIndex = new int[3 * ntris];
                memcpy(vertexIndex, vi, 3 * ntris * sizeof(int));
                //<Copy uv, N and S vertex data, if present>
                //<Transform mesh vertices to world space>
        }
    protected:
        //<TriangleMesh Data>
        int ntris, nverts;
        int * vertexIndex;
        Point *p;
        Normal *n;
        Vector *s;
        double *uvs;
    };

}




#endif // SHAPE_H
