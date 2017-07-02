#ifndef SHAPE_H
#define SHAPE_H
#include"Matrix.h"

namespace unreal {
    class Shape
    {
    private:
        const Transform ObjectToWorld, WorldToObject;
        const bool reverseOrientation, transformSwapsHandedness;
    public:
        Shape::Shape(const Transform &o2w, bool ro)
                : ObjectToWorld(o2w), WorldToObject(o2w.GetInverse()),
                reverseOrientation(ro),
                transformSwapsHandedness(o2w.SwapsHandedness()) {}

    };
}




#endif // SHAPE_H
