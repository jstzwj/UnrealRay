#ifndef INTERACTION_H
#define INTERACTION_H
#include"Point.h"
#include"Normal.h"
#include"Shape.h"
#include"Primitive.h"
#include"Spectrum.h"

namespace unreal
{
    class Shape;
    class Interaction
    {
    public:
        Interaction() : time(0) {}
        Interaction(const Point &p, const Normal &n, const Vector &pError,
                    const Vector &wo, double time)
                : p(p),
                  time(time),
                  pError(pError),
                  wo(wo.normalize()),
                  n(n) {}
    public:
        Point p;
        double time;
        Vector pError;
        Vector wo;
        Normal n;
    };

    // SurfaceInteraction Declarations
    class SurfaceInteraction : public Interaction
    {
      public:
        // SurfaceInteraction Public Methods
        SurfaceInteraction() {}
        SurfaceInteraction(const Point &p, const Vector &pError,
                           const Point &uv, const Vector &wo,
                           const Vector &dpdu, const Vector &dpdv,
                           const Normal &dndu, const Normal &dndv, double time,
                           const Shape *shape);
        void setShadingGeometry(const Vector &dpdu, const Vector &dpdv,
                                const Normal &dndu, const Normal &dndv,
                                bool orientationIsAuthoritative);
        Spectrum Le(const Vector &w) const;

        // SurfaceInteraction Public Data
        Point uv;
        Vector dpdu, dpdv;
        Normal dndu, dndv;
        const Shape *shape = nullptr;

        const Primitive *primitive = nullptr;
        //BSDF *bsdf = nullptr;
        //BSSRDF *bssrdf = nullptr;
        Vector dpdx, dpdy;
        double dudx = 0, dvdx = 0, dudy = 0, dvdy = 0;
    };
}
#endif // INTERACTION_H
