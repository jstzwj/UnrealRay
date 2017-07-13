#ifndef BSDF_H
#define BSDF_H

#include<algorithm>
#include<cmath>
#include<vector>


#include"Type.h"
#include"Ray.h"
#include"Normal.h"
#include"Utility.h"
#include"Interaction.h"
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
        BSDF_TRANSMISSION    = 1 << 1,
        BSDF_DIFFUSE        = 1 << 2,
        BSDF_GLOSSY        = 1 << 3,
        BSDF_SPECULAR    = 1 << 4,
        BSDF_ALL_TYPES = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR,
        BSDF_ALL_REFLECTION = BSDF_REFLECTION | BSDF_ALL_TYPES,
        BSDF_ALL_TRANSMISSION = BSDF_TRANSMISSION | BSDF_ALL_TYPES,
        BSDF_ALL = BSDF_ALL_REFLECTION | BSDF_ALL_TRANSMISSION
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
        virtual Spectrum sample_f(const Vector3f &wo, /*out*/Vector3f *wi, const Point2f &sample, /*out*/Float *pdf) const
        {
            // Cosine-sample the hemisphere, flipping the direction if necessary
            //*wi = CosineSampleHemisphere(u);
            //if (wo.z < 0) wi->z *= -1;
            //*pdf = Pdf(wo, *wi);
            return f(wo, *wi);
        }
        virtual Spectrum rho(const Vector3f &w, int nSamples, const Point2f *u) const
        {
            Spectrum r(0.f);
            for (int i = 0; i < nSamples; ++i)
            {
                Vector3f wi;
                Float pdf = 0.0f;
                Spectrum f = sample_f(w, &wi, u[i], &pdf);
                if (pdf > 0) r += f * AbsCosTheta(wi) / pdf;
            }
            return r / nSamples;
        }
        virtual Spectrum rho(int nSamples, const Point2f *u1, const Point2f *u2) const
        {
            Spectrum r(0.f);
            /*
            for (int i = 0; i < nSamples; ++i)
            {
                // Estimate one term of $\rho_\roman{hh}$
                Vector3f wo, wi;
                wo = UniformSampleHemisphere(u1[i]);
                Float pdfo = UniformHemispherePdf(), pdfi = 0;
                Spectrum f = Sample_f(wo, &wi, u2[i], &pdfi);
                if (pdfi > 0)
                    r += f * AbsCosTheta(wi) * AbsCosTheta(wo) / (pdfo * pdfi);
            }
            */
            return r / (PI * nSamples);
        }

        //<BxDF Public Data>
        const BxDFType type;
    };


    class Lambertian : public BxDF
    {
    public:
        //<Lambertian Public Methods>
        Lambertian(const Spectrum &reflectance)
                : BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)),
                    R(reflectance){}
        virtual Spectrum f(const Vector3f &wo, const Vector3f &wi) const override
        {
            return R * INV_PI;
        }
        virtual Spectrum rho(const Vector3f &w, int nSamples, const Point2f *u) const override
        {
            return R;
        }
        virtual Spectrum rho(int nSamples, const Point2f *u1, const Point2f *u2) const override { return R; }
    private:
        //<Lambertian Private Data>
        Spectrum R;
    };


    class BSDF
    {
    public:
        const SurfaceInteraction dgShading;
        const Float eta;
        const Normal3f ns, ng;
        const Vector3f ss, ts;
    public:
        BSDF(const SurfaceInteraction &si,Float eta=1.0f)
            :eta(eta),
              ns(si.shading.n),
              ng(si.n),
              ss(si.shading.dpdu.normalize()),
              ts(ns.cross(ss)){}
        void add(BxDF *b)
        {
            bxdfs.push_back(b);
        }
        int numComponents() const { return bxdfs.size(); }
        int numComponents(BxDFType flags) const
        {
            int num = 0;
            for (const auto &each:bxdfs)
                if (each->matchesFlags(flags)) ++num;
            return num;
        }
        Vector3f WorldToLocal(const Vector3f &v) const
        {
            return Vector3f(v.dot(ss), v.dot(ts), v.dot(ns));
        }
        Vector3f LocalToWorld(const Vector3f &v) const
        {
            return Vector3f(ss.x * v.x + ts.x * v.y + ns.x * v.z,
                            ss.y * v.x + ts.y * v.y + ns.y * v.z,
                            ss.z * v.x + ts.z * v.y + ns.z * v.z);
        }
        bool HasShadingGeometry() const
        {
            return(nn.x != ng.x || nn.y != ng.y || nn.z != ng.z);
        }
        Spectrum f(const Vector3f &woW, const Vector3f &wiW,BxDFType flags = BSDF_ALL) const
        {
            Vector3f wi = WorldToLocal(wiW), wo = WorldToLocal(woW);
            if (wo.z == 0) return 0.0f;
            bool reflect = wiW.dot(ng) * woW.dot(ng) > 0;
            Spectrum f(0.0f);
            for (const auto & each:bxdfs)
            {
                if (each->matchesFlags(flags) &&
                    ((reflect && (each->type & BSDF_REFLECTION)) ||
                     (!reflect && (each->type & BSDF_TRANSMISSION))))
                    f += each->f(wo, wi);
            }
            return f;
        }
        Spectrum rho(int nSamples, const Point2f *samples1, const Point2f *samples2,BxDFType flags = BSDF_ALL) const
        {
            Spectrum ret(0.0f);
            for (const auto & each:bxdfs)
                if (each->matchesFlags(flags))
                    ret += each->rho(nSamples, samples1, samples2);
            return ret;
        }
        Spectrum rho(const Vector3f &wo, int nSamples, const Point2f *samples,BxDFType flags = BSDF_ALL) const
        {
            Spectrum ret(0.0f);
            for (const auto & each:bxdfs)
                if (each->matchesFlags(flags))
                    ret += each->rho(wo, nSamples, samples);
            return ret;
        }
        Spectrum sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u,Float *pdf, BxDFType type = BSDF_ALL,BxDFType *sampledType = nullptr) const
        {

        }
    private:
        std::vector<BxDF *> bxdfs;
    };


}
#endif // BSDF_H
