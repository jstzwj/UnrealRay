#pragma once
#ifndef UNREALRAY_CAMERA
#define UNREALRAY_CAMERA

#include<memory>

#include"Sampler.h"
#include"Ray.h"
#include"Transform.h"
#include"Matrix.h"
#include"Film.h"
#include"Bounds.h"


namespace unreal
{
	class Camera
	{
	public:
        Camera(const Transform &CameraToWorld, Float shutterOpen,Float shutterClose, std::shared_ptr<Film> film)
            :WorldToCamera(CameraToWorld.getInverse()),CameraToWorld(CameraToWorld),
              shutterOpen(shutterOpen),shutterClose(shutterClose),
              film(film){}

        virtual ~Camera()=default;
        virtual Float generateRay(const Sample &sample, Ray *ray) const = 0;
    public:
		Transform WorldToCamera, CameraToWorld;
        //double clipHither, clipYon;   // Hither: ½üµÄ£» Yon:Ô¶µÄ
        Float shutterOpen, shutterClose;
        std::shared_ptr<Film> film;
	};
    class ProjectiveCamera : public Camera
    {
    public:
        //< ProjectiveCamera Public Methods>
    ProjectiveCamera(const Transform &CameraToWorld,
                     const Transform &CameraToScreen,
                     const Bounds2f &screenWindow, Float shutterOpen,Float shutterClose,
                     Float lensr, Float focald, std::shared_ptr<Film> film)
       : Camera(CameraToWorld, shutterOpen, shutterClose, film),CameraToScreen(CameraToScreen)
    {
        // Initialize depth of field parameters
        lensRadius = lensr;
        focalDistance = focald;

        // Compute projective camera transformations

        // Compute projective camera screen transformations
        ScreenToRaster =
            Transform::scale(film->fullResolution.x, film->fullResolution.y, 1) *
            Transform::scale(1 / (screenWindow.pMax.x - screenWindow.pMin.x),
                  1 / (screenWindow.pMin.y - screenWindow.pMax.y), 1) *
            Transform::translate(Vector3f(-screenWindow.pMin.x, -screenWindow.pMax.y, 0));
        RasterToScreen = Transform::inverse(ScreenToRaster);
        RasterToCamera = Transform::inverse(CameraToScreen) * RasterToScreen;
    }
    protected:
        //< ProjectiveCamera Protected Data>
        Transform CameraToScreen, RasterToCamera;
        Transform ScreenToRaster, RasterToScreen;
        Float lensRadius, focalDistance;
    };

    class OrthoCamera : public ProjectiveCamera
    {
    public:
        //<OrthoCamera Public Methods>
        OrthoCamera(const Transform &CameraToWorld,
                               const Bounds2f &screenWindow, Float shutterOpen,
                               Float shutterClose, Float lensRadius, Float focalDistance,
                                std::shared_ptr<Film> film)
            : ProjectiveCamera(CameraToWorld, orthographic(0, 1),
                    screenWindow, shutterOpen, shutterClose,
                               lensRadius, focalDistance, film){}
        Transform orthographic(Float znear, Float zfar)
        {
            return Transform::scale(1.0f, 1.0f, 1.0f/(zfar-znear)) *
                    Transform::translate(Vector3f(0.0f, 0.0f, -znear));
        }
        virtual Float generateRay(const Sample &sample, Ray *ray) const override
        {
            // Compute raster and camera sample positions
            Point3f pFilm = Point3f(sample.pFilm.x, sample.pFilm.y, 0);
            Point3f pCamera = RasterToCamera.transform(pFilm);
            *ray = Ray(pCamera, Vector3f(0, 0, 1));
            // Modify ray for depth of field
            //TODO
            ray->time = ::unreal::lerp(sample.time, shutterOpen, shutterClose);
            *ray = CameraToWorld.transform(*ray);
            return 1;
        }
    };
}



#endif // !UNREALRAY_CAMERA



