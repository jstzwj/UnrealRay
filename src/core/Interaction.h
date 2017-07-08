#ifndef INTERACTION_H
#define INTERACTION_H
#include"Point.h"
#include"Normal.h"
//#include"Shape.h"
//#include"Primitive.h"
#include"Spectrum.h"

namespace unreal
{


    class Interaction
    {
    public:
        Interaction() : time(0) {}
        Interaction(const Point3f &p, const Normal3f &n,
                    const Vector3f &wo, Float time)
                : pHit(p),
                  nHit(n),
                  time(time),
                  wo(wo.normalize()){}
    public:
        Point3f pHit;
        Normal3f nHit;
        Float time;
        Vector3f wo;
    };

    // SurfaceInteraction Declarations
    class SurfaceInteraction : public Interaction
    {
      public:
        SurfaceInteraction()=default;
        SurfaceInteraction(const Point3f &p, const Vector3f &wo,
                           const Vector3f &dpdu, const Vector3f &dpdv,
                           const Normal3f &dndu, const Normal3f &dndv, Float time);
        Spectrum Le(const Vector3f &w) const;

        // SurfaceInteraction Public Data
        Point2f uv;
        Vector3f dpdu, dpdv;
        Normal3f dndu, dndv;

        //const Shape *shape = nullptr;
        //const Primitive *primitive = nullptr;
        Vector3f dpdx, dpdy;
        Float dudx = 0, dvdx = 0, dudy = 0, dvdy = 0;
    };

}
#endif // INTERACTION_H
