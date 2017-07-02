#ifndef FILM_H
#define FILM_H

#include<QImage>
namespace unreal
{
    class FilmBase
    {
    public:
        virtual ~FilmBase()=default;
        virtual void setPoint(int x,int y,unsigned int color)=0;

    };

    template<class T>
    class Film:public FilmBase
    {
    private:
        T data;
    public:
        Film()=default;
        Film(const T& img):data(img){}
        virtual ~Film()=default;
        virtual void setPoint(int x,int y,unsigned int color)=0;
        T& getData(){return data;}
    };

    template<>
    class Film<QImage>:public FilmBase
    {
    private:
        QImage data;
    public:
        Film()=default;
        Film(const QImage& img):data(img){}
        virtual ~Film()=default;
        virtual void setPoint(int x,int y,unsigned int color)
        {
            data.setPixel(x,y,color);
        }

        QImage& getData(){return data;}
    };

}




#endif // FILM_H
