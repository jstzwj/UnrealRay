#pragma once
#ifndef UNREALRAY_CAMERA
#define UNREALRAY_CAMERA

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
        Camera(const Transform &world2cam, double hither, double yon,
                    double sopen, double sclose, Film * film);
        virtual ~Camera()=default;
        virtual double generateRay(const Sample &sample, Ray *ray) const = 0;
    public:
		Transform WorldToCamera, CameraToWorld;
        double ClipHither, ClipYon;   // Hither: ½üµÄ£» Yon:Ô¶µÄ
        double ShutterOpen, ShutterClose;
        Film * film;
	};
    class ProjectiveCamera : public Camera
    {
    public:
        //< ProjectiveCamera Public Methods>
    ProjectiveCamera(const Transform &CameraToWorld,
                     const Transform &CameraToScreen,
                     const Bounds2f &screenWindow, Float shutterOpen,
                     double shutterClose, double lensr, double focald, Film *film)
       : Camera(w2c, hither, yon, sopen, sclose, f)
    {
        // Initialize depth of field parameters
        lensRadius = lensr;
        focalDistance = focald;

        // Compute projective camera transformations

        // Compute projective camera screen transformations
        ScreenToRaster =
            Transform::scale(film->xResolution, film->yResolution, 1) *
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
        double lensRadius, focalDistance;
    };

    class OrthoCamera : public ProjectiveCamera
    {
    public:
        //<OrthoCamera Public Methods>
        OrthoCamera(const Transform &world2cam,
                const double Screen[4], double hither, double yon,
                double sopen, double sclose, double lensr,
                double focald, Film *f)
            : ProjectiveCamera(world2cam, orthographic(hither, yon),
                    Screen, hither, yon, sopen, sclose,
                    lensr, focald, f)
        {
        }
        Transform orthographic(double znear, double zfar)
        {
            return Transform::scale(1.0, 1.0, 1.0/(zfar-znear)) *
                    Transform::translate(Vector3f(0.0, 0.0, -znear));
        }
        virtual double generateRay(const Sample &sample, Ray *ray) const override
        {
            //<Generate raster and camera samples>
            Point3f Pras(sample.imageX, sample.imageY, 0);
            Point3f Pcamera;
            Pcamera=RasterToCamera.transform(Pras);

            ray->origin = Pcamera;
            ray->direction = Vector3f(0, 0, 1);
            //<Set ray time value>
            //<Modify ray for depth of field>
            ray->mint = 0;
            ray->maxt = ClipYon - ClipHither;
            ray->direction = ray->direction.normalize();
            *ray=CameraToWorld.transform(*ray);
            return 1.0;
        }
    };
}



#endif // !UNREALRAY_CAMERA



