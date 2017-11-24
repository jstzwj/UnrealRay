#ifndef TEXTURE_H
#define TEXTURE_H


#include"Interaction.h"
namespace unreal
{
    template <typename T>
    class Texture
    {
      public:
        virtual T evaluate(const SurfaceInteraction &) const = 0;
        virtual ~Texture()=default;
    };

    template <typename T>
    class ConstantTexture : public Texture<T>
    {
      public:
        ConstantTexture(const T &value) : value(value) {}
        T evaluate(const SurfaceInteraction &) const override{ return value; }
      private:
        T value;
    };
}



#endif // TEXTURE_H
