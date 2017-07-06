#ifndef BSDF_H
#define BSDF_H

#include<algorithm>
#include<cmath>

#include"Ray.h"
#include"Normal.h"
#include"Utility.h"
#include"Differentialgeometry.h"
#include"Spectrum.h"


namespace unreal
{
    // BSDF Inline Functions
    inline double CosTheta(const Vector &w) { return w.z; }
    inline double Cos2Theta(const Vector &w) { return w.z * w.z; }
    inline double AbsCosTheta(const Vector &w) { return std::abs(w.z); }
    inline double Sin2Theta(const Vector &w)
    {
        return std::max(0.0, 1.0 - Cos2Theta(w));
    }

    inline double SinTheta(const Vector &w) { return std::sqrt(Sin2Theta(w)); }

    inline double TanTheta(const Vector &w) { return SinTheta(w) / CosTheta(w); }

    inline double Tan2Theta(const Vector &w)
    {
        return Sin2Theta(w) / Cos2Theta(w);
    }

    inline double CosPhi(const Vector &w)
    {
        double sinTheta = SinTheta(w);
        return (sinTheta == 0) ? 1 : clamp(w.x / sinTheta, -1, 1);
    }

    inline double SinPhi(const Vector &w)
    {
        double sinTheta = SinTheta(w);
        return (sinTheta == 0) ? 0 : clamp(w.y / sinTheta, -1, 1);
    }

    inline double Cos2Phi(const Vector &w) { return CosPhi(w) * CosPhi(w); }

    inline double Sin2Phi(const Vector &w) { return SinPhi(w) * SinPhi(w); }

    inline double CosDPhi(const Vector &wa, const Vector &wb)
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
        const double eta;
    private:
        Normal nn, ng;
        Vector sn, tn;
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
        virtual Spectrum f(const Vector &wo, const Vector &wi) const = 0;
        virtual Spectrum sample_f(const Vector &wo, Vector *wi, double u1, double u2, double *pdf) const;
        virtual Spectrum rho(const Vector &wo, int nSamples = 16, double *samples = nullptr) const;
        virtual Spectrum rho(int nSamples = 16, double *samples = nullptr) const;



        //<BxDF Public Data>
        const BxDFType type;
    };
}
#endif // BSDF_H
