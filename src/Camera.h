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
        virtual ~Camera()=default;
        virtual double generateRay(const Sampler &sample, Ray *ray) const = 0;
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
    ProjectiveCamera(const Transform &w2c,const Transform &proj,
                     const double Screen[4],
           double hither, double yon, double sopen,double sclose,
    double lensr, double focald, Film *f)
       : Camera(w2c, hither, yon, sopen, sclose, f)
    {
        //<Initialize depth of field parameters>
        //<Compute projective camera transformations>
        CameraToScreen = proj;
        WorldToScreen = CameraToScreen * WorldToCamera;
        //(0,1) (2,3)
        Transform ScreenToRaster = Transform::scale(film->xResolution,film->yResolution, 1.0) *
                Transform::scale(1.0/ (Screen[1] - Screen[0]),1.0/(Screen[2] - Screen[3]), 1.0) *
                Transform::translate(Vector(-Screen[0], - Screen[3], 0.0));
        RasterToScreen = ScreenToRaster.GetInverse();

        RasterToCamera = CameraToScreen.GetInverse() * RasterToScreen;
    }
    protected:
        //< ProjectiveCamera Protected Data>
        Transform CameraToScreen, WorldToScreen, RasterToCamera;
    };

    class OrthoCamera : public ProjectiveCamera
    {
    public:
        //<OrthoCamera Public Methods>
        OrthoCamera(const Transform &world2cam,
                const double Screen[4], double hither, double yon,
                double sopen, double sclose, double lensr,
                double focald, Film *f)
            : ProjectiveCamera(world2cam, Orthographic(hither, yon),
                    Screen, hither, yon, sopen, sclose,
                    lensr, focald, f)
        {
        }
        Transform orthographic(double znear, double zfar)
        {
            return Transform::scale(1.0, 1.0, 1.0/(zfar-znear)) *
                    Transform::translate(Vector(0.0, 0.0, -znear));
        }
        virtual double generateRay(const Sample &sample, Ray *ray) const override
        {
            //<Generate raster and camera samples>
            Point Pras(sample.imageX, sample.imageY, 0);
            Point Pcamera;
            RasterToCamera(Pras, &Pcamera);

            ray->origin = Pcamera;
            ray->direction = Vector(0, 0, 1);
            //<Set ray time value>
            //<Modify ray for depth of field>
            ray->mint = 0;
            ray->maxt = ClipYon - ClipHither;
            ray->direction = ray->direction.normalize();
            *ray=CameraToWorld.transform(*ray);
            return 1.f;
        }
    };
}



#endif // !UNREALRAY_CAMERA



