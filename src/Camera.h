#pragma once
#ifndef UNREALRAY_CAMERA
#define UNREALRAY_CAMERA

#include"Sampler.h"
#include"Ray.h"
#include"Transform.h"
#include"Matrix.h"
#include"Film.h"


namespace unreal
{
	class Camera
	{
	public:
        Camera(const Transform &world2cam, double hither, double yon,
                    double sopen, double sclose, Film * film);
		~Camera();
        virtual double GenerateRay(const Sampler &sample, Ray *ray) const = 0;
	protected:
		Transform WorldToCamera, CameraToWorld;
        double ClipHither, ClipYon;   // Hither: ½üµÄ£» Yon:Ô¶µÄ
        double ShutterOpen, ShutterClose;
        Film * film;
	};
    class ProjectiveCamera : public Camera
    {
        public:
            //< ProjectiveCamera Public Methods>
        protected:
            //< ProjectiveCamera Protected Data>
    };
}



#endif // !UNREALRAY_CAMERA



