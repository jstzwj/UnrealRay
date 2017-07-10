#ifndef BSDF_H
#define BSDF_H

#include<algorithm>
#include<cmath>


#include"Type.h"
#include"Ray.h"
#include"Normal.h"
#include"Utility.h"
#include"Differentialgeometry.h"
#include"Spectrum.h"


namespace unreal
{
    // BSDF Inline Functions
    inline Float CosTheta(const Vector3f &w) { return w.z; }
    inline Float Cos2Theta(const Vector3f &w) { return w.z * w.z; }
    inline Float AbsCosTheta(const Vector3f &w) { return std::abs(w.z); }
    inline Float Sin2Theta(const Vector3f &w)
    {
        return std::max(0.0, 1.0 - Cos2Theta(w));
    }

    inline Float SinTheta(const Vector3f &w) { return std::sqrt(Sin2Theta(w)); }

    inline Float TanTheta(const Vector3f &w) { return SinTheta(w) / CosTheta(w); }

    inline Float Tan2Theta(const Vector3f &w)
    {
        return Sin2Theta(w) / Cos2Theta(w);
    }

    inline Float CosPhi(const Vector3f &w)
    {
        Float sinTheta = SinTheta(w);
        return (sinTheta == 0) ? 1 : clamp(w.x / sinTheta, -1, 1);
    }

    inline Float SinPhi(const Vector3f &w)
    {
        Float sinTheta = SinTheta(w);
        return (sinTheta == 0) ? 0 : clamp(w.y / sinTheta, -1, 1);
    }

    inline Float Cos2Phi(const Vector3f &w) { return CosPhi(w) * CosPhi(w); }

    inline Float Sin2Phi(const Vector3f &w) { return SinPhi(w) * SinPhi(w); }

    inline Float CosDPhi(const Vector3f &wa, const Vector3f &wb)
    {
        return clamp(
            (wa.x * wb.x + wa.y * wb.y) / std::sqrt((wa.x * wa.x + wa.y * wa.y) *
                                                    (wb.x * wb.x + wb.y * wb.y)),
            -1, 1);
    }
    class BSDF
    {
    public:
        //const DifferentialGeometry dgShading;
        const Float eta=0.0f;
    private:
        Normal3f nn, ng;
        Vector3f sn, tn;
    public:

    };

    enum BxDFType
    {
        BSDF_REFLECTION    = 1 << 0,
        BSDF_TRASNMISSION    = 1 << 1,
        BSDF_DIFFUSE        = 1 << 2,
        BSDF_GLOSSY        = 1 << 3,
        BSDF_SPECULAR    = 1 << 4,
        BSDF_ALL_TYPES = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR,
        BSDF_ALL_REFLECTION = BSDF_REFLECTION | BSDF_ALL_TYPES,
        BSDF_ALL_TRASNMISSION = BSDF_TRASNMISSION | BSDF_ALL_TYPES,
        BSDF_ALL = BSDF_ALL_REFLECTION | BSDF_ALL_TRASNMISSION
   };

    class BxDF
    {
    public:
        //<BxDF Interface>
        BxDF(BxDFType t) : type (t) {}
        bool matchesFlag(BxDFType flags) const
        {
            return (type & flags) == type;
        }
        virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const = 0;
        virtual Spectrum sample_f(const Vector3f &wo, Vector3f *wi, Float u1, Float u2, Float *pdf) const;
        virtual Spectrum rho(const Vector3f &wo, int nSamples = 16, Float *samples = nullptr) const;
        virtual Spectrum rho(int nSamples = 16, Float *samples = nullptr) const;



        //<BxDF Public Data>
        const BxDFType type;
    };
}
#endif // BSDF_H
