#ifndef SHAPE_H
#define SHAPE_H
#include<vector>
#include<stdexcept>
#include<cmath>
#include"Type.h"
#include"Matrix.h"
#include"Ray.h"
#include"Transform.h"
#include"Interaction.h"
#include"Utility.h"

namespace unreal
{

    class Shape
    {
    public:
        const Transform ObjectToWorld, WorldToObject;
        const bool reverseOrientation, transformSwapsHandedness;
    public:
        Shape(const Transform &ObjectToWorld, const Transform &WorldToObject,bool reverseOrientation)
            : ObjectToWorld(ObjectToWorld), WorldToObject(WorldToObject),
                reverseOrientation(reverseOrientation),transformSwapsHandedness(ObjectToWorld.swapsHandedness()) {}
           virtual ~Shape()=default;

           virtual bool intersect(const Ray &ray, Float *tHit,SurfaceInteraction *isect) const = 0;
           virtual bool intersectP(const Ray &ray) const {
               return intersect(ray, nullptr, nullptr);
           }
    };

    class Sphere : public Shape
    {
        public:
            Sphere(const Transform &o2w, bool ro, Float rad,Float z0, Float z1, Float pm)
                : Shape(o2w,o2w.getInverse(), ro)
            {
                radius = rad;
                zmin = clamp(std::min(z0, z1), -radius, radius);
                zmax = clamp(std::max(z0, z1), -radius, radius);
                thetaMin = std::acos(clamp(zmin/radius,0,1));
                thetaMax = std::acos(clamp(zmax/radius,0,1));
                phiMax = radians(clamp(pm, 0.0f, 360.f));
            }
            virtual bool intersect(const Ray &ray, Float *tHit,SurfaceInteraction *isect) const override
            {

                //<Transform Ray to object space>
                Ray r=WorldToObject.transform(ray);
                //<Compute quadratic sphere coefficients>
                Float A = r.direction.x*r.direction.x + r.direction.y*r.direction.y + r.direction.z*r.direction.z;
                Float B = 2 * (r.direction.x*r.origin.x + r.direction.y*r.origin.y + r.direction.z*r.origin.z);
                Float C = r.origin.x*r.origin.x + r.origin.y*r.origin.y + r.origin.z*r.origin.z - radius * radius;

                //<Solve quadratic equation for t values>
                Float t0, t1;
                if(!quadratic(A, B, C, &t0, &t1))
                    return false;

                //<Compute intersection distance along ray>
                //两个解在一侧的情况
                if (t0 > r.maxt || t1 < r.mint)
                    return false;

                //两个解在两侧的情况
                if (t0 < r.mint&&t1 > r.maxt)
                {
                    return false;
                }
                Float thit = t0;
                //只有远端相交的情况
                if(t0 < r.mint&&t1 < r.maxt)
                    thit = t1;

                Point3f phit;
                Float phi;
                //<Compare sphere hit position and Φ>
                phit = r.getPoint(thit);//交点
                phi = std::atan2(phit.y, phit.x);//Φ
                if(phi < 0.0) phi += 2.0 * PI;

                //<Test sphere intersection against clipping parameters>
                if (phit.z < zmin || phit.z > zmax || phi > phiMax)
                {
                    //只有一个解是t1（被裁剪）
                    if(thit == t1) return false;
                    //只有一个解是t0（被裁剪）
                    if(t1 > r.maxt) return false;
                    //有两个解，t0被裁剪，测试t1
                    thit = t1;
                    //<Compare sphere hit position and Φ>
                    phit = r.getPoint(thit);//交点
                    phi = std::atan2(phit.y, phit.x);//Φ
                    if(phi < 0.0) phi += 2.0 * PI;
                    //测试t1是否也被裁剪
                    if (phit.z < zmin || phit.z > zmax || phi > phiMax)
                        return false;
                }

                //<Finding parametric representation of sphere hit>
                Float u = phi / phiMax;
                Float theta = std::acos(phit.z / radius);
                Float v = (theta - thetaMin) / (thetaMax - thetaMin);
                //<Compute sphere dp/du and dp/dv>

                Float zradius = sqrtf(phit.x * phit.x + phit.y * phit.y);
                Float invzradius = 1.f / zradius;
                Float cosphi = phit.x * invzradius;
                Float sinphi  = phit.y * invzradius;
                Vector3f dpdu(-phiMax * phit.y,phiMax * phit.x,0);

                Vector3f dpdv=Vector3f(phit.z * cosphi,
                                   phit.z * sinphi,
                                   -radius * std::sin(thetaMin+v*(thetaMax-thetaMin)))
                        *(thetaMax - thetaMin);
                //<Compute sphere dn/du and dn/dv>
                Vector3f d2Pduu =  Vector3f(phit.x, phit.y, 0.0f)* -phiMax * phiMax;
                Vector3f d2Pduv  = Vector3f(-sinphi, cosphi, 0.0f)*
                                    (thetaMax - thetaMin) * phit.z * phiMax;
                Vector3f d2Pdvv  = Vector3f(phit.x, phit.y, phit.z)*
                                    -(thetaMax - thetaMin) * (thetaMax - thetaMin);
                //< Compute coefficients for fundamental forms>
                Float E = dpdu.dot(dpdu);
                Float F = dpdu.dot(dpdv);
                Float G = dpdv.dot(dpdv);
                Vector3f N = dpdu.cross(dpdv);
                Float e = N.dot(d2Pduu);
                Float f = N.dot(d2Pduv);
                Float g = N.dot(d2Pdvv);
                //<Compute dn/du and dn/dv from fundamental form coefficients>
                Float invEGF2 = 1.f / (E * G - F * F);
                Vector3f dndu = dpdu *(f * F - e * G) * invEGF2  + dpdv * (e * F - f * E) * invEGF2 ;
                Vector3f dndv = dpdu * (g* F - f * G) * invEGF2  +  dpdv * (f * F - g * E) * invEGF2;


                //<Initialize DifferentialGeometry from parametric information>
                isect->pHit=phit;
                isect->dpdu=dpdu;
                isect->dpdv=dpdv;
                isect->dndu=(Normal3f)dndu;
                isect->dndv=(Normal3f)dndv;
                isect->uv={u,v};
                //isect->shape=this;

                //〈Update tHit for quadric intersection>
                *tHit = thit;
                return true;
            }
            virtual bool intersectP(const Ray &r) const override
            {

                //<Transform Ray to object space>
                Ray ray=WorldToObject.transform(r);
                //<Compute quadratic sphere coefficients>
                Float A = ray.direction.x*ray.direction.x + ray.direction.y*ray.direction.y + ray.direction.z*ray.direction.z;
                Float B = 2 * (ray.direction.x*ray.origin.x + ray.direction.y*ray.origin.y + ray.direction.z*ray.origin.z);
                Float C = ray.origin.x*ray.origin.x + ray.origin.y*ray.origin.y + ray.origin.z*ray.origin.z - radius * radius;

                //<Solve quadratic equation for t values>
                Float t0, t1;
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
                Float thit = t0;
                //只有远端相交的情况
                if(t0 < ray.mint&&t1 < ray.maxt)
                    thit = t1;

                Point3f phit;
                Float phi;
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
            Float area() const
            {
                return phiMax * radius * (zmax - zmin);
            }
        private:
            Float radius;
            Float phiMax;
            Float zmin, zmax;
            Float thetaMin, thetaMax;

    };


    class TriangleMesh : public Shape
    {
    public:
        //<TriangleMesh Public Methods>
        TriangleMesh(const Transform &o2w, bool ro, int nt, int nv,
                const int *vi, const Point3f *P, const Normal3f *N, const  Vector3f *S, Float *uv)
                : Shape(o2w,o2w.getInverse(), ro)
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
        Point3f *p;
        Normal3f *n;
        Vector3f *s;
        Float *uvs;
    };

}




#endif // SHAPE_H
