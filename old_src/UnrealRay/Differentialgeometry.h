#ifndef DIFFERENTIALGEOMETRY_H
#define DIFFERENTIALGEOMETRY_H


#include"Point.h"
#include"Normal.h"
#include"Shape.h"

namespace unreal
{
    class DifferentialGeometry
    {
    public:
        Point p;
        Normal nn;
        double u, v;
        const Shape *shape;

        Vector dpdu, dpdv;
        Vector dndu, dndv;

    public:
        DifferentialGeometry(const Point &p,
                const Vector &DPDU, const Vector &DPDV,
                const Vector &DNDU, const Vector &DNDV,
                double uu, double vv, const Shape *sh)
                :p(P), dpdu(DPDU), dpdv(DPDV), dndu(DNDU), dndv(DNDV)
        {

            nn = Normal(Normalize(Cross(dpdu, dpdv)));
            u = uu;
            v = vv;
            shape = sh;

            if( shape->reverseOrientation ^ shape->transformSwapsHandedness)
                nn *= -1.f;
        }


    };
}




#endif // DIFFERENTIALGEOMETRY_H
