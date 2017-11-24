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

class Interaction;
class SurfaceInteraction;
struct Shading;

namespace unreal
{
    // BSDF Inline Functions
    inline Float CosTheta(const Vector3f &w) { return w.z; }
    inline Float Cos2Theta(const Vector3f &w) { return w.z * w.z; }
    inline Float AbsCosTheta(const Vector3f &w) { return std::abs(w.z); }
    inline Float Sin2Theta(const Vector3f &w)
    {
        return std::max(0.0f, 1.0f - Cos2Theta(w));
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
            return r / (unreal::Float)nSamples;
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

	inline Float FrDielectric(Float cosThetaI, Float etaI, Float etaT) 
	{
		cosThetaI = unreal::clamp(cosThetaI, -1, 1);
		// Potentially swap indices of refraction
		bool entering = cosThetaI > 0.f;
		if (!entering) {
			std::swap(etaI, etaT);
			cosThetaI = std::abs(cosThetaI);
		}

		// Compute _cosThetaT_ using Snell's law
		Float sinThetaI = std::sqrt(std::max((Float)0, 1 - cosThetaI * cosThetaI));
		Float sinThetaT = etaI / etaT * sinThetaI;

		// Handle total internal reflection
		if (sinThetaT >= 1) return 1;
		Float cosThetaT = std::sqrt(std::max((Float)0, 1 - sinThetaT * sinThetaT));
		Float Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
			((etaT * cosThetaI) + (etaI * cosThetaT));
		Float Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
			((etaI * cosThetaI) + (etaT * cosThetaT));
		return (Rparl * Rparl + Rperp * Rperp) / 2;
	}

	inline Spectrum FrConductor(Float cosThetaI, const Spectrum &etai,
		const Spectrum &etat, const Spectrum &k) 
	{
		cosThetaI = unreal::clamp(cosThetaI, -1, 1);
		Spectrum eta = etat / etai;
		Spectrum etak = k / etai;

		Float cosThetaI2 = cosThetaI * cosThetaI;
		Float sinThetaI2 = 1. - cosThetaI2;
		Spectrum eta2 = eta * eta;
		Spectrum etak2 = etak * etak;

		Spectrum t0 = eta2 - etak2 - sinThetaI2;
		Spectrum a2plusb2 = (t0 * t0 + eta2 * etak2 * 4).sqrt();
		Spectrum t1 = a2plusb2 + cosThetaI2;
		Spectrum a = ((a2plusb2 + t0) * 0.5f).sqrt();
		Spectrum t2 = a * (Float)2 * cosThetaI;
		Spectrum Rs = (t1 - t2) / (t1 + t2);

		Spectrum t3 = a2plusb2 * cosThetaI2 + sinThetaI2 * sinThetaI2;
		Spectrum t4 = t2 * sinThetaI2;
		Spectrum Rp = Rs * (t3 - t4) / (t3 + t4);

		return (Rp + Rs) * 0.5f;
	}

	class Fresnel 
	{
	public:
		// Fresnel Interface
		virtual ~Fresnel() {}
		virtual Spectrum Evaluate(Float cosI) const = 0;
	};

	class FresnelConductor : public Fresnel {
	public:
		// FresnelConductor Public Methods
		Spectrum Evaluate(Float cosThetaI) const
		{
			return FrConductor(std::abs(cosThetaI), etaI, etaT, k);
		}
		FresnelConductor(const Spectrum &etaI, const Spectrum &etaT, const Spectrum &k)
			: etaI(etaI), etaT(etaT), k(k) {}

	private:
		Spectrum etaI, etaT, k;
	};

	class FresnelDielectric : public Fresnel {
	public:
		// FresnelDielectric Public Methods
		Spectrum Evaluate(Float cosThetaI) const
		{
			return FrDielectric(cosThetaI, etaI, etaT);
		}

		FresnelDielectric(Float etaI, Float etaT) : etaI(etaI), etaT(etaT) {}

	private:
		Float etaI, etaT;
	};

	class FresnelNoOp : public Fresnel {
	public:
		Spectrum Evaluate(Float) const { return Spectrum(1.); }
	};

	class SpecularReflection : public BxDF 
	{
	public:
		// SpecularReflection Public Methods
		SpecularReflection(const Spectrum &R, const std::shared_ptr<Fresnel> fresnel)
			: BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR)),
			R(R),
			fresnel(fresnel) {}
		Spectrum f(const Vector3f &wo, const Vector3f &wi) const {
			return Spectrum(0.f);
		}

		Spectrum Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &sample, Float *pdf, BxDFType *sampledType) const
		{
			*wi = Vector3f(-wo.x, -wo.y, wo.z);
			*pdf = 1;
			return fresnel->Evaluate(CosTheta(*wi)) * R / AbsCosTheta(*wi);
		}

		Float Pdf(const Vector3f &wo, const Vector3f &wi) const { return 0; }

	private:
		// SpecularReflection Private Data
		const Spectrum R;
		std::shared_ptr<Fresnel> fresnel;
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
        // const SurfaceInteraction& dgShading;
        const Float eta;
        const Normal3f ns, ng;
        const Vector3f ss, ts;
    public:
		BSDF(const Normal3f& ns, const Normal3f& ng, const Vector3f& ss, const Vector3f& ts, Float eta = 1.0f)
			:eta(eta), ns(ns), ng(ng), ss(ss), ts(ts) {}
              // ns(si.shading.n),
              // ng(si.nHit),
              // ss(si.shading.dpdu.normalize()),
              // ts(Vector3f(ns).cross(Vector3f(ss.x,ss.y,ss.z))){}

        void add(std::shared_ptr<BxDF> b)
        {
            bxdfs.push_back(b);
        }
        int numComponents() const { return bxdfs.size(); }
        int numComponents(BxDFType flags) const
        {
            int num = 0;
            for (const auto &each:bxdfs)
                if (each->matchesFlag(flags)) ++num;
            return num;
        }
        Vector3f WorldToLocal(const Vector3f &v) const
        {
            return Vector3f(v.dot(ss), v.dot(ts), v.dot(Vector3f(ns)));
        }
        Vector3f LocalToWorld(const Vector3f &v) const
        {
            return Vector3f(ss.x * v.x + ts.x * v.y + ns.x * v.z,
                            ss.y * v.x + ts.y * v.y + ns.y * v.z,
                            ss.z * v.x + ts.z * v.y + ns.z * v.z);
        }
        bool HasShadingGeometry() const
        {
            return(ns.x != ng.x || ns.y != ng.y || ns.z != ng.z);
        }
        Spectrum f(const Vector3f &woW, const Vector3f &wiW,BxDFType flags = BSDF_ALL) const
        {
            Vector3f wi = WorldToLocal(wiW), wo = WorldToLocal(woW);
            if (wo.z == 0) return 0.0f;
            bool reflect = wiW.dot(Vector3f(ng.x,ng.y,ng.z)) * woW.dot(Vector3f(ng.x,ng.y,ng.z)) > 0;
            Spectrum f(0.0f);
            for (const auto & each:bxdfs)
            {
				if (each->matchesFlag(flags) &&
					((reflect && (each->type & BSDF_REFLECTION)) ||
					(!reflect && (each->type & BSDF_TRANSMISSION))))
				{
					f += each->f(wo, wi);
				}
            }
            return f;
        }
        Spectrum rho(int nSamples, const Point2f *samples1, const Point2f *samples2,BxDFType flags = BSDF_ALL) const
        {
            Spectrum ret(0.0f);
            for (const auto & each:bxdfs)
                if (each->matchesFlag(flags))
                    ret += each->rho(nSamples, samples1, samples2);
            return ret;
        }
        Spectrum rho(const Vector3f &wo, int nSamples, const Point2f *samples,BxDFType flags = BSDF_ALL) const
        {
            Spectrum ret(0.0f);
            for (const auto & each:bxdfs)
                if (each->matchesFlag(flags))
                    ret += each->rho(wo, nSamples, samples);
            return ret;
        }
        Spectrum sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u,Float *pdf, BxDFType type = BSDF_ALL,BxDFType *sampledType = nullptr) const
        {

        }
    private:
        std::vector<std::shared_ptr<BxDF> > bxdfs;
    };


}
#endif // BSDF_H
