#pragma once
#ifndef UNREALRAY_CAMERA
#define UNREALRAY_CAMERA

#include"Sampler.h"
#include"ray.h"
#include""
#include"matrix.h"

namespace unreal
{
	class AbstractCamera
	{
	public:
		virtual float GenerateRay(const Sampler &sample, Ray *ray) const = 0;
	};
	class Camera
	{
	public:
		Camera();
		~Camera();
		
	protected:
		Transform WorldToCamera, CameraToWorld;
		float ClipHither, ClipYon;   // Hither: ½üµÄ£» Yon:Ô¶µÄ
		float ShutterOpen, ShutterClose;
		Film film;
	};
}



#endif // !UNREALRAY_CAMERA



