#ifndef BOUNDS_H
#define BOUNDS_H

#include<limits>
#include<ostream>
#include <iterator>

#include"Point.h"
#include"Vector.h"
#include"Utility.h"



namespace unreal
{
    template <typename T>
    class Bounds2
    {
      public:
        Bounds2()
        {
            T minNum = std::numeric_limits<T>::lowest();
            T maxNum = std::numeric_limits<T>::max();
            pMin = Point2<T>(maxNum, maxNum);
            pMax = Point2<T>(minNum, minNum);
        }
        Bounds2(const Point2<T> &p) : pMin(p), pMax(p) {}
        Bounds2(const Point2<T> &p1, const Point2<T> &p2)
        {
            pMin = Point2<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
            pMax = Point2<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
        }
        template <typename U>
        explicit operator Bounds2<U>() const
        {
            return Bounds2<U>((Point2<U>)pMin, (Point2<U>)pMax);
        }

        Vector2<T> diagonal() const { return pMax - pMin; }
        T area() const
        {
            Vector2<T> d = pMax - pMin;
            return d.x * d.y;
        }
        const Point2<T> &operator[](int i) const
        {
            return (i == 0) ? pMin : pMax;
        }
        inline Point2<T> &operator[](int i)
        {
            return (i == 0) ? pMin : pMax;
        }
        bool operator ==(const Bounds2<T> &b) const
        {
            return b.pMin == pMin && b.pMax == pMax;
        }
        bool operator!=(const Bounds2<T> &b) const
        {
            return b.pMin != pMin || b.pMax != pMax;
        }
        Point2<T> lerp(const Point2f &t) const
        {
            return Point2<T>(::unreal::lerp(t.x, pMin.x, pMax.x),
                             ::unreal::lerp(t.y, pMin.y, pMax.y));
        }
        Vector2<T> offset(const Point2<T> &p) const {
            Vector2<T> o = p - pMin;
            if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
            if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
            return o;
        }
        friend std::ostream &operator<<(std::ostream &os, const Bounds2<T> &b)
        {
            os << "[ " << b.pMin << " - " << b.pMax << " ]";
            return os;
        }

        Point2<T> pMin, pMax;
    };

    template <typename T>
    class Bounds3
    {
      public:
        Bounds3()
        {
            T minNum = std::numeric_limits<T>::lowest();
            T maxNum = std::numeric_limits<T>::max();
            pMin = Point3<T>(maxNum, maxNum, maxNum);
            pMax = Point3<T>(minNum, minNum, minNum);
        }
        Bounds3(const Point3<T> &p) : pMin(p), pMax(p) {}
        Bounds3(const Point3<T> &p1, const Point3<T> &p2)
            : pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
                   std::min(p1.z, p2.z)),
              pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
                   std::max(p1.z, p2.z)) {}
        const Point3<T> &operator[](int i) const;
        Point3<T> &operator[](int i);
        bool operator==(const Bounds3<T> &b) const
        {
            return b.pMin == pMin && b.pMax == pMax;
        }
        bool operator!=(const Bounds3<T> &b) const
        {
            return b.pMin != pMin || b.pMax != pMax;
        }
        Point3<T> corner(int corner) const
        {
            return Point3<T>((*this)[(corner & 1)].x,
                             (*this)[(corner & 2) ? 1 : 0].y,
                             (*this)[(corner & 4) ? 1 : 0].z);
        }
        Vector3<T> diagonal() const { return pMax - pMin; }
        T surfaceArea() const
        {
            Vector3<T> d = diagonal();
            return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
        }
        T volume() const
        {
            Vector3<T> d = diagonal();
            return d.x * d.y * d.z;
        }
        Point3<T> lerp(const Point3f &t) const
        {
            return Point3<T>(::unreal::lerp(t.x, pMin.x, pMax.x),
                             ::unreal::lerp(t.y, pMin.y, pMax.y),
                             ::unreal::lerp(t.z, pMin.z, pMax.z));
        }
        Vector3<T> offset(const Point3<T> &p) const
        {
            Vector3<T> o = p - pMin;
            if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
            if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
            if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;
            return o;
        }
        template <typename U>
        explicit operator Bounds3<U>() const
        {
            return Bounds3<U>((Point3<U>)pMin, (Point3<U>)pMax);
        }
        friend std::ostream &operator<<(std::ostream &os, const Bounds3<T> &b)
        {
            os << "[ " << b.pMin << " - " << b.pMax << " ]";
            return os;
        }
    public:
        Point3<T> pMin, pMax;
    };

    typedef Bounds2<Float> Bounds2f;
    typedef Bounds2<int> Bounds2i;
    typedef Bounds3<Float> Bounds3f;
    typedef Bounds3<int> Bounds3i;


    class Bounds2iIterator : public std::forward_iterator_tag
    {
    public:
        Bounds2iIterator(const Bounds2i &b, const Point2i &pt)
            : p(pt), bounds(&b) {}
        Bounds2iIterator operator++()
        {
            advance();
            return *this;
        }
        Bounds2iIterator operator++(int)
        {
            Bounds2iIterator old = *this;
            advance();
            return old;
        }
        bool operator==(const Bounds2iIterator &bi) const
        {
            return p == bi.p && bounds == bi.bounds;
        }
        bool operator!=(const Bounds2iIterator &bi) const
        {
            return p != bi.p || bounds != bi.bounds;
        }

        Point2i operator *() const { return p; }

      private:
        void advance()
        {
            ++p.x;
            if (p.x >= bounds->pMax.x)
            {
                p.x = bounds->pMin.x;
                ++p.y;
            }
        }
        Point2i p;
        const Bounds2i *bounds;
    };
    inline Bounds2iIterator begin(const Bounds2i &b)
    {
        return Bounds2iIterator(b, b.pMin);
    }

    inline Bounds2iIterator end(const Bounds2i &b)
    {
        return Bounds2iIterator(b, Point2i(b.pMin.x, b.pMax.y));
    }



    class Bounds3iIterator : public std::forward_iterator_tag
    {
    public:
        Bounds3iIterator(const Bounds3i &b, const Point3i &pt)
            : p(pt), bounds(&b) {}
        Bounds3iIterator operator++()
        {
            advance();
            return *this;
        }
        Bounds3iIterator operator++(int)
        {
            Bounds3iIterator old = *this;
            advance();
            return old;
        }
        bool operator==(const Bounds3iIterator &bi) const
        {
            return p == bi.p && bounds == bi.bounds;
        }
        bool operator!=(const Bounds3iIterator &bi) const
        {
            return p != bi.p || bounds != bi.bounds;
        }

        Point3i operator *() const { return p; }

      private:
        void advance()
        {
            ++p.x;
            if (p.x >= bounds->pMax.x)
            {
                p.x = bounds->pMin.x;
                ++p.y;
                if(p.y>=bounds->pMax.y)
                {
                    p.y = bounds->pMin.y;
                    ++p.z;
                }
            }
        }
        Point3i p;
        const Bounds3i *bounds;
    };

    inline Bounds3iIterator begin(const Bounds3i &b)
    {
        return Bounds3iIterator(b, Point3i(b.pMin.x, b.pMin.y, b.pMin.z));
    }

    inline Bounds3iIterator end(const Bounds3i &b)
    {
        return Bounds3iIterator(b, Point3i(b.pMin.x, b.pMin.y, b.pMax.y));
    }


}
#endif // BOUNDS_H
