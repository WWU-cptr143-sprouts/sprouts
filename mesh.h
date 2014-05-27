#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include "vector.h"
#include "color.h"
#include "matrix.h"
#include <memory>
#include <iterator>
#include <ostream>
#include "image.h"
#include "texture_descriptor.h"
#include "stream.h"
class Mesh_t;
typedef shared_ptr<Mesh_t> Mesh;

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct TextureCoord
{
    float u, v;
    TextureCoord(float u, float v)

        /**
         * @brief Write what the function does here
         *
         * @param u
         * @param v
         *
         * @return
         **/
        : u(u), v(v)
        {
        }
    TextureCoord()

        /**
         * @brief Write what the function does here
         *
         * @param 0
         * @param 0
         *
         * @return
         **/
        : TextureCoord(0, 0)
<<<<<<< HEAD
<<<<<<< HEAD
    {
    }
=======
        {
        }
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
        {
        }
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

    /**
     * @brief Write what the function does here
     *
     * @param os
     * @param t
     *
     * @return
     **/
    friend ostream & operator <<(ostream & os, const TextureCoord & t)
    {
        return os << "<" << t.u << ", " << t.v << ">";
    }
};
template <>

/**
 * @brief Write what the function does here
 *
 * @param t
 * @param a
 * @param b
 *
 * @return
 **/
inline const TextureCoord interpolate<TextureCoord>(const float t, const TextureCoord a, const TextureCoord b)
{
    return TextureCoord(interpolate(t, a.u, b.u), interpolate(t, a.v, b.v));
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct Triangle
{
    VectorF p[3];
    Color c[3];
    TextureCoord t[3];

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    Triangle()
    {
    }

    /**
     * @brief Write what the function does here
     *
     * @param p1
     * @param c1
     * @param t1
     * @param p2
     * @param c2
     * @param t2
     * @param p3
     * @param c3
     * @param t3
     *
     * @return
     **/
    Triangle(VectorF p1, Color c1, TextureCoord t1, VectorF p2, Color c2, TextureCoord t2, VectorF p3, Color c3, TextureCoord t3)
    {
        p[0] = p1;
        c[0] = c1;
        t[0] = t1;
        p[1] = p2;
        c[1] = c2;
        t[1] = t2;
        p[2] = p3;
        c[2] = c3;
        t[2] = t3;
    }

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    VectorF normal() const
    {
        return normalize(cross(p[1] - p[0], p[2] - p[0]));
    }
};

/**
 * @brief Write what the function does here
 *
 * @param m
 * @param t
 *
 * @return
 **/
inline Triangle transform(const Matrix &m, Triangle t)
{

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    /**
     * @brief Write what the function does here
     *
     * @param p
     *
     * @return
     **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    for(VectorF &p : t.p)
    {
        p = transform(m, p);
    }
    return t;
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct TransformedMesh
{
    Mesh mesh;
    Matrix tform;
    Color factor;
    TransformedMesh()

        /**
         * @brief Write what the function does here
         *
         * @param nullptr
         * @param 1
         * @param 1
         * @param 1
         * @param 1
         *
         * @return
         **/
        : mesh(nullptr), tform(Matrix::identity()), factor(1, 1, 1, 1)
        {
        }
    TransformedMesh(Mesh mesh, Matrix tform, Color factor = Color(1, 1, 1, 1))

        /**
         * @brief Write what the function does here
         *
         * @param mesh
         * @param tform
         * @param factor
         *
         * @return
         **/
        : mesh(mesh), tform(tform), factor(factor)
        {
        }
    operator Mesh() const;
};

/**
 * @brief Write what the function does here
 *
 * @param m
 * @param mesh
 *
 * @return
 **/
inline TransformedMesh transform(const Matrix &m, Mesh mesh)
{
    return TransformedMesh(mesh, m);
}

/**
 * @brief Write what the function does here
 *
 * @param m
 * @param mesh
 *
 * @return
 **/
inline TransformedMesh transform(const Matrix &m, TransformedMesh mesh)
{
    return TransformedMesh(mesh.mesh, mesh.tform.concat(m), mesh.factor);
}

/**
 * @brief Write what the function does here
 *
 * @param factor
 * @param mesh
 *
 * @return
 **/
inline TransformedMesh scaleColors(Color factor, Mesh mesh)
{
    return TransformedMesh(mesh, Matrix::identity(), factor);
}

/**
 * @brief Write what the function does here
 *
 * @param factor
 * @param mesh
 *
 * @return
 **/
inline TransformedMesh scaleColors(Color factor, TransformedMesh mesh)
{
    return TransformedMesh(mesh.mesh, mesh.tform, scale(mesh.factor, factor));
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class ImageNotSameException final : public runtime_error
{
    public:
        explicit ImageNotSameException()
<<<<<<< HEAD
<<<<<<< HEAD
            : runtime_error("can't use more than one image per mesh")
        {
        }
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            : runtime_error("can't use more than one image per mesh")
            {
            }
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class Mesh_t final
{
    private:
        vector<float> points, colors, textureCoords;
        Image textureInternal;
        size_t length;
        static constexpr size_t floatsPerPoint = 3, pointsPerTriangle = 3,
                         floatsPerColor = 4, colorsPerTriangle = 3,
                         floatsPerTextureCoord = 2, textureCoordsPerTriangle = 3;
        friend class Renderer;

        /**
         * @brief Write what the function does here
         *
         * @param reader
         * @param client
         *
         * @return
         **/
        /*    Mesh_t(Reader &reader, Client &client)
              {
              DUMP_V(Mesh_t::Mesh_t, "reading mesh");
              length = reader.readU32();
              points.resize(floatsPerPoint * pointsPerTriangle * length);
              colors.resize(floatsPerColor * colorsPerTriangle * length);
              textureCoords.resize(floatsPerTextureCoord * textureCoordsPerTriangle * length);
              textureInternal = Image::read(reader, client);
              DUMP_V(Mesh_t::Mesh_t, "reading mesh : read texture");

<<<<<<< HEAD
<<<<<<< HEAD
              for(float & v : points)
              {
              v = reader.readFiniteF32();
              }
              DUMP_V(Mesh_t::Mesh_t, "reading mesh : read points");

              for(float & v : textureCoords)
              {
              v = reader.readFiniteF32();
              }
              DUMP_V(Mesh_t::Mesh_t, "reading mesh : read textureCoords");

              for(float & v : colors)
              {
              v = reader.readFiniteF32();
              }
              DUMP_V(Mesh_t::Mesh_t, "reading mesh : read colors");
              }*/
    public:
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        /**
         * @brief Write what the function does here
         *
         * @param points
         *
         * @return
         **/
        for(float & v : points)
        {
            v = reader.readFiniteF32();
        }
        DUMP_V(Mesh_t::Mesh_t, "reading mesh : read points");

        /**
         * @brief Write what the function does here
         *
         * @param textureCoords
         *
         * @return
         **/
        for(float & v : textureCoords)
        {
            v = reader.readFiniteF32();
        }
        DUMP_V(Mesh_t::Mesh_t, "reading mesh : read textureCoords");
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

        /**
         * @brief Write what the function does here
         *
<<<<<<< HEAD
<<<<<<< HEAD
         * @return
         **/
        Mesh_t()
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
         * @param colors
         *
         * @return
         **/
        for(float & v : colors)
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        {
            length = 0;
        }
<<<<<<< HEAD
<<<<<<< HEAD

        /**
         * @brief Write what the function does here
         *
         * @param texture
         *
         * @return
         **/
        Mesh_t(Image texture, vector<Triangle> triangles = vector<Triangle>())
        {
            length = triangles.size();
            points.reserve(floatsPerPoint * pointsPerTriangle * length);
            colors.reserve(floatsPerColor * colorsPerTriangle * length);
            textureCoords.reserve(floatsPerTextureCoord * textureCoordsPerTriangle * length);
            textureInternal = texture;

            for(Triangle tri : triangles)
            {
                points.push_back(tri.p[0].x);
                points.push_back(tri.p[0].y);
                points.push_back(tri.p[0].z);
                points.push_back(tri.p[1].x);
                points.push_back(tri.p[1].y);
                points.push_back(tri.p[1].z);
                points.push_back(tri.p[2].x);
                points.push_back(tri.p[2].y);
                points.push_back(tri.p[2].z);
                colors.push_back(tri.c[0].r);
                colors.push_back(tri.c[0].g);
                colors.push_back(tri.c[0].b);
                colors.push_back(tri.c[0].a);
                colors.push_back(tri.c[1].r);
                colors.push_back(tri.c[1].g);
                colors.push_back(tri.c[1].b);
                colors.push_back(tri.c[1].a);
                colors.push_back(tri.c[2].r);
                colors.push_back(tri.c[2].g);
                colors.push_back(tri.c[2].b);
                colors.push_back(tri.c[2].a);
                textureCoords.push_back(tri.t[0].u);
                textureCoords.push_back(tri.t[0].v);
                textureCoords.push_back(tri.t[1].u);
                textureCoords.push_back(tri.t[1].v);
                textureCoords.push_back(tri.t[2].u);
                textureCoords.push_back(tri.t[2].v);
            }
        }
=======
        DUMP_V(Mesh_t::Mesh_t, "reading mesh : read colors");
}*/
public:

/**
 * @brief Write what the function does here
 *
 * @return
 **/
Mesh_t()
{
    length = 0;
}

/**
 * @brief Write what the function does here
 *
 * @param texture
 *
 * @return
 **/
Mesh_t(Image texture, vector<Triangle> triangles = vector<Triangle>())
{
    length = triangles.size();
    points.reserve(floatsPerPoint * pointsPerTriangle * length);
    colors.reserve(floatsPerColor * colorsPerTriangle * length);
    textureCoords.reserve(floatsPerTextureCoord * textureCoordsPerTriangle * length);
    textureInternal = texture;

    /**
     * @brief Write what the function does here
     *
     * @param triangles
     *
     * @return
     **/
    for(Triangle tri : triangles)
    {
        points.push_back(tri.p[0].x);
        points.push_back(tri.p[0].y);
        points.push_back(tri.p[0].z);
        points.push_back(tri.p[1].x);
        points.push_back(tri.p[1].y);
        points.push_back(tri.p[1].z);
        points.push_back(tri.p[2].x);
        points.push_back(tri.p[2].y);
        points.push_back(tri.p[2].z);
        colors.push_back(tri.c[0].r);
        colors.push_back(tri.c[0].g);
        colors.push_back(tri.c[0].b);
        colors.push_back(tri.c[0].a);
        colors.push_back(tri.c[1].r);
        colors.push_back(tri.c[1].g);
        colors.push_back(tri.c[1].b);
        colors.push_back(tri.c[1].a);
        colors.push_back(tri.c[2].r);
        colors.push_back(tri.c[2].g);
        colors.push_back(tri.c[2].b);
        colors.push_back(tri.c[2].a);
        textureCoords.push_back(tri.t[0].u);
        textureCoords.push_back(tri.t[0].v);
        textureCoords.push_back(tri.t[1].u);
        textureCoords.push_back(tri.t[1].v);
        textureCoords.push_back(tri.t[2].u);
        textureCoords.push_back(tri.t[2].v);
    }
}

/**
 * @brief Write what the function does here
 *
 * @param tex
 *
 * @return
 **/
Mesh_t(TextureDescriptor tex, vector<Triangle> triangles = vector<Triangle>())
{
    length = triangles.size();
    points.reserve(floatsPerPoint * pointsPerTriangle * length);
    colors.reserve(floatsPerColor * colorsPerTriangle * length);
    textureCoords.reserve(floatsPerTextureCoord * textureCoordsPerTriangle * length);
    textureInternal = tex.image;

    /**
     * @brief Write what the function does here
     *
     * @param triangles
     *
     * @return
     **/
    for(Triangle tri : triangles)
    {
        points.push_back(tri.p[0].x);
        points.push_back(tri.p[0].y);
        points.push_back(tri.p[0].z);
        points.push_back(tri.p[1].x);
        points.push_back(tri.p[1].y);
        points.push_back(tri.p[1].z);
        points.push_back(tri.p[2].x);
        points.push_back(tri.p[2].y);
        points.push_back(tri.p[2].z);
        colors.push_back(tri.c[0].r);
        colors.push_back(tri.c[0].g);
        colors.push_back(tri.c[0].b);
        colors.push_back(tri.c[0].a);
        colors.push_back(tri.c[1].r);
        colors.push_back(tri.c[1].g);
        colors.push_back(tri.c[1].b);
        colors.push_back(tri.c[1].a);
        colors.push_back(tri.c[2].r);
        colors.push_back(tri.c[2].g);
        colors.push_back(tri.c[2].b);
        colors.push_back(tri.c[2].a);
        textureCoords.push_back(interpolate(tri.t[0].u, tex.minU, tex.maxU));
        textureCoords.push_back(interpolate(tri.t[0].v, tex.minV, tex.maxV));
        textureCoords.push_back(interpolate(tri.t[1].u, tex.minU, tex.maxU));
        textureCoords.push_back(interpolate(tri.t[1].v, tex.minV, tex.maxV));
        textureCoords.push_back(interpolate(tri.t[2].u, tex.minU, tex.maxU));
        textureCoords.push_back(interpolate(tri.t[2].v, tex.minV, tex.maxV));
    }
}
Mesh_t(const TransformedMesh &tm)

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
: Mesh_t()
{

    /**
     * @brief Write what the function does here
     *
     * @param nullptr
     *
     * @return
     **/
    if(tm.mesh == nullptr)
    {
        return;
    }
    points = tm.mesh->points;
    colors = tm.mesh->colors;
    textureCoords = tm.mesh->textureCoords;
    textureInternal = tm.mesh->texture();
    length = tm.mesh->length;

    /**
     * @brief Write what the function does here
     *
     * @param floatsPerPoint
     *
     * @return
     **/
    for(auto i = points.begin(); i != points.end(); i += floatsPerPoint)
    {
        VectorF v;
        v.x = i[0];
        v.y = i[1];
        v.z = i[2];
        v = transform(tm.tform, v);
        i[0] = v.x;
        i[1] = v.y;
        i[2] = v.z;
    }

    /**
     * @brief Write what the function does here
     *
     * @param floatsPerColor
     *
     * @return
     **/
    for(auto i = colors.begin(); i != colors.end(); i += floatsPerColor)
    {
        Color c;
        c.r = i[0];
        c.g = i[1];
        c.b = i[2];
        c.a = i[3];
        c = scale(c, tm.factor);
        i[0] = c.r;
        i[1] = c.g;
        i[2] = c.b;
        i[3] = c.a;
    }
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const Image &texture() const
{
    return textureInternal;
}
friend class const_iterator;

/**
 * @brief Write what the function does here
 *
 * @param value_type
 * @param Triangle
 *
 * @return
 **/
class const_iterator final : public iterator<iterator_traits<vector<float>::iterator>::value_type, const Triangle, ssize_t>
{
    friend class Mesh_t;
    private:
    typedef vector<float>::const_iterator subIterator;
    mutable Triangle tri;
    subIterator pointIterator, colorIterator, textureCoordIterator;
    const_iterator(subIterator pointIterator, subIterator colorIterator, subIterator textureCoordIterator)
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

        /**
         * @brief Write what the function does here
         *
<<<<<<< HEAD
         * @param tex
         *
         * @return
         **/
        Mesh_t(TextureDescriptor tex, vector<Triangle> triangles = vector<Triangle>())
        {
            length = triangles.size();
            points.reserve(floatsPerPoint * pointsPerTriangle * length);
            colors.reserve(floatsPerColor * colorsPerTriangle * length);
            textureCoords.reserve(floatsPerTextureCoord * textureCoordsPerTriangle * length);
            textureInternal = tex.image;

            for(Triangle tri : triangles)
            {
                points.push_back(tri.p[0].x);
                points.push_back(tri.p[0].y);
                points.push_back(tri.p[0].z);
                points.push_back(tri.p[1].x);
                points.push_back(tri.p[1].y);
                points.push_back(tri.p[1].z);
                points.push_back(tri.p[2].x);
                points.push_back(tri.p[2].y);
                points.push_back(tri.p[2].z);
                colors.push_back(tri.c[0].r);
                colors.push_back(tri.c[0].g);
                colors.push_back(tri.c[0].b);
                colors.push_back(tri.c[0].a);
                colors.push_back(tri.c[1].r);
                colors.push_back(tri.c[1].g);
                colors.push_back(tri.c[1].b);
                colors.push_back(tri.c[1].a);
                colors.push_back(tri.c[2].r);
                colors.push_back(tri.c[2].g);
                colors.push_back(tri.c[2].b);
                colors.push_back(tri.c[2].a);
                textureCoords.push_back(interpolate(tri.t[0].u, tex.minU, tex.maxU));
                textureCoords.push_back(interpolate(tri.t[0].v, tex.minV, tex.maxV));
                textureCoords.push_back(interpolate(tri.t[1].u, tex.minU, tex.maxU));
                textureCoords.push_back(interpolate(tri.t[1].v, tex.minV, tex.maxV));
                textureCoords.push_back(interpolate(tri.t[2].u, tex.minU, tex.maxU));
                textureCoords.push_back(interpolate(tri.t[2].v, tex.minV, tex.maxV));
            }
        }
        Mesh_t(const TransformedMesh &tm)
            : Mesh_t()
        {

            if(tm.mesh == nullptr)
            {
                return;
            }
            points = tm.mesh->points;
            colors = tm.mesh->colors;
            textureCoords = tm.mesh->textureCoords;
            textureInternal = tm.mesh->texture();
            length = tm.mesh->length;

            for(auto i = points.begin(); i != points.end(); i += floatsPerPoint)
            {
                VectorF v;
                v.x = i[0];
                v.y = i[1];
                v.z = i[2];
                v = transform(tm.tform, v);
                i[0] = v.x;
                i[1] = v.y;
                i[2] = v.z;
            }

            for(auto i = colors.begin(); i != colors.end(); i += floatsPerColor)
            {
                Color c;
                c.r = i[0];
                c.g = i[1];
                c.b = i[2];
                c.a = i[3];
                c = scale(c, tm.factor);
                i[0] = c.r;
                i[1] = c.g;
                i[2] = c.b;
                i[3] = c.a;
            }
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const Image &texture() const
        {
            return textureInternal;
        }
        friend class const_iterator;

        /**
         * @brief Write what the function does here
         *
         * @param value_type
         * @param Triangle
         *
         * @return
         **/
        class const_iterator final : public iterator<iterator_traits<vector<float>::iterator>::value_type, const Triangle, ssize_t>
    {
        friend class Mesh_t;
        private:
        typedef vector<float>::const_iterator subIterator;
        mutable Triangle tri;
        subIterator pointIterator, colorIterator, textureCoordIterator;
        const_iterator(subIterator pointIterator, subIterator colorIterator, subIterator textureCoordIterator)
            : pointIterator(pointIterator), colorIterator(colorIterator), textureCoordIterator(textureCoordIterator)
        {
        }
        public:

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const_iterator()
        {
        }

        /**
         * @brief Write what the function does here
         *
         * @param rt
         *
         * @return
         **/
        bool operator ==(const const_iterator &rt) const
        {
            return pointIterator == rt.pointIterator;
        }

        /**
         * @brief Write what the function does here
         *
         * @param rt
         *
         * @return
         **/
        bool operator !=(const const_iterator &rt) const
        {
            return pointIterator != rt.pointIterator;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const Triangle &operator *() const
        {
            subIterator p = pointIterator, c = colorIterator, t = textureCoordIterator;
            tri.p[0] = VectorF(p[0], p[1], p[2]);
            tri.p[1] = VectorF(p[3], p[4], p[5]);
            tri.p[2] = VectorF(p[6], p[7], p[8]);
            tri.c[0] = Color(c[0], c[1], c[2], c[3]);
            tri.c[1] = Color(c[4], c[5], c[6], c[7]);
            tri.c[2] = Color(c[8], c[9], c[10], c[11]);
            tri.t[0] = TextureCoord(t[0], t[1]);
            tri.t[1] = TextureCoord(t[2], t[3]);
            tri.t[2] = TextureCoord(t[4], t[5]);
            return tri;
        }

        /**
         * @brief Write what the function does here
         *
         * @param index
         *
         * @return
         **/
        const Triangle &operator[](ssize_t index) const
        {
            return operator +(index).operator * ();
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const Triangle *operator ->() const
        {
            return &operator *();
        }

        /**
         * @brief Write what the function does here
         *
         * @param i
         *
         * @return
         **/
        const_iterator operator +(ssize_t i) const
        {
            return const_iterator(pointIterator + i * floatsPerPoint * pointsPerTriangle,
                    colorIterator + i * floatsPerColor * colorsPerTriangle,
                    textureCoordIterator + i * floatsPerTextureCoord * textureCoordsPerTriangle);
        }

        /**
         * @brief Write what the function does here
         *
         * @param i
         * @param iter
         *
         * @return
         **/
        friend const_iterator operator +(ssize_t i, const const_iterator &iter)
        {
            return iter.operator + (i);
        }

        /**
         * @brief Write what the function does here
         *
         * @param i
         *
         * @return
         **/
        const_iterator operator -(ssize_t i) const
        {
            return operator +(-i);
        }

        /**
         * @brief Write what the function does here
         *
         * @param r
         *
         * @return
         **/
        ssize_t operator -(const const_iterator &r) const
        {
            return (textureCoordIterator - r.textureCoordIterator) / (floatsPerTextureCoord * textureCoordsPerTriangle);
        }

        /**
         * @brief Write what the function does here
         *
         * @param i
         *
         * @return
         **/
        const const_iterator &operator +=(ssize_t i)
        {
            return *this = operator +(i);
        }

        /**
         * @brief Write what the function does here
         *
         * @param i
         *
         * @return
         **/
        const const_iterator &operator -=(ssize_t i)
        {
            return *this = operator -(i);
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const const_iterator &operator ++()
        {
            return operator +=(1);
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const const_iterator &operator --()
        {
            return operator -=(1);
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @return
         **/
        const_iterator operator ++(int)
        {
            const_iterator retval = *this;
            operator ++();
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @param int
         *
         * @return
         **/
        const_iterator operator --(int)
        {
            const_iterator retval = *this;
            operator --();
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @param r
         *
         * @return
         **/
        bool operator >(const const_iterator &r) const
        {
            return pointIterator > r.pointIterator;
        }

        /**
         * @brief Write what the function does here
         *
         * @param r
         *
         * @return
         **/
        bool operator >=(const const_iterator &r) const
        {
            return pointIterator >= r.pointIterator;
        }

        /**
         * @brief Write what the function does here
         *
         * @param r
         *
         * @return
         **/
        bool operator <(const const_iterator &r) const
        {
            return pointIterator < r.pointIterator;
        }

        /**
         * @brief Write what the function does here
         *
         * @param r
         *
         * @return
         **/
        bool operator <=(const const_iterator &r) const
        {
            return pointIterator <= r.pointIterator;
        }
    };
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        size_t size() const
        {
            return length;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const_iterator begin() const
        {
            return const_iterator(points.begin(), colors.begin(), textureCoords.begin());
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const_iterator end() const
        {
            return const_iterator(points.end(), colors.end(), textureCoords.end());
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const_iterator cbegin() const
        {
            return const_iterator(points.begin(), colors.begin(), textureCoords.begin());
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const_iterator cend() const
        {
            return const_iterator(points.end(), colors.end(), textureCoords.end());
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(end());
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        const_reverse_iterator crend() const
        {
            return const_reverse_iterator(begin());
        }

        /**
         * @brief Write what the function does here
         *
         * @param m
         **/
        void add(const Mesh_t &m)
        {

            if(texture())
            {

                if(m.texture() && m.texture() != texture())
                {
                    throw ImageNotSameException();
                }
            }

            else
            {
                textureInternal = m.texture();
            }
            length += m.length;
            points.insert(points.end(), m.points.begin(), m.points.end());
            colors.insert(colors.end(), m.colors.begin(), m.colors.end());
            textureCoords.insert(textureCoords.end(), m.textureCoords.begin(), m.textureCoords.end());
        }

        /**
         * @brief Write what the function does here
         *
         * @param m
         **/
        void add(Mesh m)
        {
            add(*m);
        }

        /**
         * @brief Write what the function does here
         *
         * @param m
         **/
        void add(TransformedMesh m)
        {
            Mesh_t m2(m);
            add(m2);
        }
        /*friend void writeMesh(Mesh mesh, Writer &writer, Client &client);
          friend Mesh readMesh(Reader &reader, Client &client);*/
        friend Mesh interpolateColors(Mesh dest, Mesh mesh, Color cNXNYNZ, Color cNXNYPZ, Color cNXPYNZ, Color cNXPYPZ, Color cPXNYNZ, Color cPXNYPZ, Color cPXPYNZ, Color cPXPYPZ);
        friend Mesh interpolateColors(Mesh mesh, Color cNXNYNZ, Color cNXNYPZ, Color cNXPYNZ, Color cNXPYPZ, Color cPXNYNZ, Color cPXNYPZ, Color cPXPYNZ, Color cPXPYPZ);
        friend Mesh lightColors(Mesh dest, Mesh mesh, VectorF lightDir, float ambient, float diffuse);
=======
        DUMP_V(Mesh_t::Mesh_t, "reading mesh : read colors");
}*/
public:

/**
 * @brief Write what the function does here
 *
 * @return
 **/
Mesh_t()
{
    length = 0;
}

/**
 * @brief Write what the function does here
 *
 * @param texture
 *
 * @return
 **/
Mesh_t(Image texture, vector<Triangle> triangles = vector<Triangle>())
{
    length = triangles.size();
    points.reserve(floatsPerPoint * pointsPerTriangle * length);
    colors.reserve(floatsPerColor * colorsPerTriangle * length);
    textureCoords.reserve(floatsPerTextureCoord * textureCoordsPerTriangle * length);
    textureInternal = texture;

    /**
     * @brief Write what the function does here
     *
     * @param triangles
     *
     * @return
     **/
    for(Triangle tri : triangles)
    {
        points.push_back(tri.p[0].x);
        points.push_back(tri.p[0].y);
        points.push_back(tri.p[0].z);
        points.push_back(tri.p[1].x);
        points.push_back(tri.p[1].y);
        points.push_back(tri.p[1].z);
        points.push_back(tri.p[2].x);
        points.push_back(tri.p[2].y);
        points.push_back(tri.p[2].z);
        colors.push_back(tri.c[0].r);
        colors.push_back(tri.c[0].g);
        colors.push_back(tri.c[0].b);
        colors.push_back(tri.c[0].a);
        colors.push_back(tri.c[1].r);
        colors.push_back(tri.c[1].g);
        colors.push_back(tri.c[1].b);
        colors.push_back(tri.c[1].a);
        colors.push_back(tri.c[2].r);
        colors.push_back(tri.c[2].g);
        colors.push_back(tri.c[2].b);
        colors.push_back(tri.c[2].a);
        textureCoords.push_back(tri.t[0].u);
        textureCoords.push_back(tri.t[0].v);
        textureCoords.push_back(tri.t[1].u);
        textureCoords.push_back(tri.t[1].v);
        textureCoords.push_back(tri.t[2].u);
        textureCoords.push_back(tri.t[2].v);
=======
         * @param pointIterator
         * @param colorIterator
         * @param textureCoordIterator
         *
         * @return
         **/
        : pointIterator(pointIterator), colorIterator(colorIterator), textureCoordIterator(textureCoordIterator)
        {
        }
    public:

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    const_iterator()
    {
    }

    /**
     * @brief Write what the function does here
     *
     * @param rt
     *
     * @return
     **/
    bool operator ==(const const_iterator &rt) const
    {
        return pointIterator == rt.pointIterator;
    }

    /**
     * @brief Write what the function does here
     *
     * @param rt
     *
     * @return
     **/
    bool operator !=(const const_iterator &rt) const
    {
        return pointIterator != rt.pointIterator;
    }

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    const Triangle &operator *() const
    {
        subIterator p = pointIterator, c = colorIterator, t = textureCoordIterator;
        tri.p[0] = VectorF(p[0], p[1], p[2]);
        tri.p[1] = VectorF(p[3], p[4], p[5]);
        tri.p[2] = VectorF(p[6], p[7], p[8]);
        tri.c[0] = Color(c[0], c[1], c[2], c[3]);
        tri.c[1] = Color(c[4], c[5], c[6], c[7]);
        tri.c[2] = Color(c[8], c[9], c[10], c[11]);
        tri.t[0] = TextureCoord(t[0], t[1]);
        tri.t[1] = TextureCoord(t[2], t[3]);
        tri.t[2] = TextureCoord(t[4], t[5]);
        return tri;
    }

    /**
     * @brief Write what the function does here
     *
     * @param index
     *
     * @return
     **/
    const Triangle &operator[](ssize_t index) const
    {
        return operator +(index).operator * ();
    }

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    const Triangle *operator ->() const
    {
        return &operator *();
    }

    /**
     * @brief Write what the function does here
     *
     * @param i
     *
     * @return
     **/
    const_iterator operator +(ssize_t i) const
    {
        return const_iterator(pointIterator + i * floatsPerPoint * pointsPerTriangle,
                colorIterator + i * floatsPerColor * colorsPerTriangle,
                textureCoordIterator + i * floatsPerTextureCoord * textureCoordsPerTriangle);
    }

    /**
     * @brief Write what the function does here
     *
     * @param i
     * @param iter
     *
     * @return
     **/
    friend const_iterator operator +(ssize_t i, const const_iterator &iter)
    {
        return iter.operator + (i);
    }

    /**
     * @brief Write what the function does here
     *
     * @param i
     *
     * @return
     **/
    const_iterator operator -(ssize_t i) const
    {
        return operator +(-i);
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }
}

/**
 * @brief Write what the function does here
 *
 * @param tex
 *
 * @return
 **/
Mesh_t(TextureDescriptor tex, vector<Triangle> triangles = vector<Triangle>())
{
    length = triangles.size();
    points.reserve(floatsPerPoint * pointsPerTriangle * length);
    colors.reserve(floatsPerColor * colorsPerTriangle * length);
    textureCoords.reserve(floatsPerTextureCoord * textureCoordsPerTriangle * length);
    textureInternal = tex.image;

    /**
     * @brief Write what the function does here
     *
<<<<<<< HEAD
     * @param triangles
     *
     * @return
     **/
    for(Triangle tri : triangles)
    {
        points.push_back(tri.p[0].x);
        points.push_back(tri.p[0].y);
        points.push_back(tri.p[0].z);
        points.push_back(tri.p[1].x);
        points.push_back(tri.p[1].y);
        points.push_back(tri.p[1].z);
        points.push_back(tri.p[2].x);
        points.push_back(tri.p[2].y);
        points.push_back(tri.p[2].z);
        colors.push_back(tri.c[0].r);
        colors.push_back(tri.c[0].g);
        colors.push_back(tri.c[0].b);
        colors.push_back(tri.c[0].a);
        colors.push_back(tri.c[1].r);
        colors.push_back(tri.c[1].g);
        colors.push_back(tri.c[1].b);
        colors.push_back(tri.c[1].a);
        colors.push_back(tri.c[2].r);
        colors.push_back(tri.c[2].g);
        colors.push_back(tri.c[2].b);
        colors.push_back(tri.c[2].a);
        textureCoords.push_back(interpolate(tri.t[0].u, tex.minU, tex.maxU));
        textureCoords.push_back(interpolate(tri.t[0].v, tex.minV, tex.maxV));
        textureCoords.push_back(interpolate(tri.t[1].u, tex.minU, tex.maxU));
        textureCoords.push_back(interpolate(tri.t[1].v, tex.minV, tex.maxV));
        textureCoords.push_back(interpolate(tri.t[2].u, tex.minU, tex.maxU));
        textureCoords.push_back(interpolate(tri.t[2].v, tex.minV, tex.maxV));
=======
     * @param r
     *
     * @return
     **/
    ssize_t operator -(const const_iterator &r) const
    {
        return (textureCoordIterator - r.textureCoordIterator) / (floatsPerTextureCoord * textureCoordsPerTriangle);
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }
}
Mesh_t(const TransformedMesh &tm)

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
: Mesh_t()
{

    /**
     * @brief Write what the function does here
     *
<<<<<<< HEAD
     * @param nullptr
     *
     * @return
     **/
    if(tm.mesh == nullptr)
    {
        return;
=======
     * @param i
     *
     * @return
     **/
    const const_iterator &operator +=(ssize_t i)
    {
        return *this = operator +(i);
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }
    points = tm.mesh->points;
    colors = tm.mesh->colors;
    textureCoords = tm.mesh->textureCoords;
    textureInternal = tm.mesh->texture();
    length = tm.mesh->length;

    /**
     * @brief Write what the function does here
     *
<<<<<<< HEAD
     * @param floatsPerPoint
     *
     * @return
     **/
    for(auto i = points.begin(); i != points.end(); i += floatsPerPoint)
    {
        VectorF v;
        v.x = i[0];
        v.y = i[1];
        v.z = i[2];
        v = transform(tm.tform, v);
        i[0] = v.x;
        i[1] = v.y;
        i[2] = v.z;
=======
     * @param i
     *
     * @return
     **/
    const const_iterator &operator -=(ssize_t i)
    {
        return *this = operator -(i);
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }

    /**
     * @brief Write what the function does here
     *
<<<<<<< HEAD
     * @param floatsPerColor
     *
     * @return
     **/
    for(auto i = colors.begin(); i != colors.end(); i += floatsPerColor)
    {
        Color c;
        c.r = i[0];
        c.g = i[1];
        c.b = i[2];
        c.a = i[3];
        c = scale(c, tm.factor);
        i[0] = c.r;
        i[1] = c.g;
        i[2] = c.b;
        i[3] = c.a;
=======
     * @return
     **/
    const const_iterator &operator ++()
    {
        return operator +=(1);
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }
}

<<<<<<< HEAD
/**
 * @brief Write what the function does here
 *
 * @return
 **/
const Image &texture() const
{
    return textureInternal;
}
friend class const_iterator;

/**
 * @brief Write what the function does here
 *
 * @param value_type
 * @param Triangle
 *
 * @return
 **/
class const_iterator final : public iterator<iterator_traits<vector<float>::iterator>::value_type, const Triangle, ssize_t>
{
    friend class Mesh_t;
    private:
    typedef vector<float>::const_iterator subIterator;
    mutable Triangle tri;
    subIterator pointIterator, colorIterator, textureCoordIterator;
    const_iterator(subIterator pointIterator, subIterator colorIterator, subIterator textureCoordIterator)

        /**
         * @brief Write what the function does here
         *
         * @param pointIterator
         * @param colorIterator
         * @param textureCoordIterator
         *
         * @return
         **/
        : pointIterator(pointIterator), colorIterator(colorIterator), textureCoordIterator(textureCoordIterator)
        {
        }
    public:

=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    /**
     * @brief Write what the function does here
     *
     * @return
     **/
<<<<<<< HEAD
    const_iterator()
    {
=======
    const const_iterator &operator --()
    {
        return operator -=(1);
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }

    /**
     * @brief Write what the function does here
     *
<<<<<<< HEAD
     * @param rt
     *
     * @return
     **/
    bool operator ==(const const_iterator &rt) const
    {
        return pointIterator == rt.pointIterator;
=======
     * @param int
     *
     * @return
     **/
    const_iterator operator ++(int)
    {
        const_iterator retval = *this;
        operator ++();
        return retval;
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }

    /**
     * @brief Write what the function does here
     *
<<<<<<< HEAD
     * @param rt
     *
     * @return
     **/
    bool operator !=(const const_iterator &rt) const
    {
        return pointIterator != rt.pointIterator;
=======
     * @param int
     *
     * @return
     **/
    const_iterator operator --(int)
    {
        const_iterator retval = *this;
        operator --();
        return retval;
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }

    /**
     * @brief Write what the function does here
     *
<<<<<<< HEAD
     * @return
     **/
    const Triangle &operator *() const
    {
        subIterator p = pointIterator, c = colorIterator, t = textureCoordIterator;
        tri.p[0] = VectorF(p[0], p[1], p[2]);
        tri.p[1] = VectorF(p[3], p[4], p[5]);
        tri.p[2] = VectorF(p[6], p[7], p[8]);
        tri.c[0] = Color(c[0], c[1], c[2], c[3]);
        tri.c[1] = Color(c[4], c[5], c[6], c[7]);
        tri.c[2] = Color(c[8], c[9], c[10], c[11]);
        tri.t[0] = TextureCoord(t[0], t[1]);
        tri.t[1] = TextureCoord(t[2], t[3]);
        tri.t[2] = TextureCoord(t[4], t[5]);
        return tri;
=======
     * @param r
     *
     * @return
     **/
    bool operator >(const const_iterator &r) const
    {
        return pointIterator > r.pointIterator;
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }

    /**
     * @brief Write what the function does here
     *
<<<<<<< HEAD
     * @param index
     *
     * @return
     **/
    const Triangle &operator[](ssize_t index) const
    {
        return operator +(index).operator * ();
    }

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    const Triangle *operator ->() const
    {
        return &operator *();
=======
     * @param r
     *
     * @return
     **/
    bool operator >=(const const_iterator &r) const
    {
        return pointIterator >= r.pointIterator;
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }

    /**
     * @brief Write what the function does here
     *
<<<<<<< HEAD
     * @param i
     *
     * @return
     **/
    const_iterator operator +(ssize_t i) const
    {
        return const_iterator(pointIterator + i * floatsPerPoint * pointsPerTriangle,
                colorIterator + i * floatsPerColor * colorsPerTriangle,
                textureCoordIterator + i * floatsPerTextureCoord * textureCoordsPerTriangle);
    }

    /**
     * @brief Write what the function does here
     *
     * @param i
     * @param iter
     *
     * @return
     **/
    friend const_iterator operator +(ssize_t i, const const_iterator &iter)
    {
        return iter.operator + (i);
    }

    /**
     * @brief Write what the function does here
     *
     * @param i
     *
     * @return
     **/
    const_iterator operator -(ssize_t i) const
    {
        return operator +(-i);
    }

    /**
     * @brief Write what the function does here
     *
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
     * @param r
     *
     * @return
     **/
<<<<<<< HEAD
    ssize_t operator -(const const_iterator &r) const
    {
        return (textureCoordIterator - r.textureCoordIterator) / (floatsPerTextureCoord * textureCoordsPerTriangle);
    }

    /**
     * @brief Write what the function does here
     *
     * @param i
     *
     * @return
     **/
    const const_iterator &operator +=(ssize_t i)
    {
        return *this = operator +(i);
=======
    bool operator <(const const_iterator &r) const
    {
        return pointIterator < r.pointIterator;
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }

    /**
     * @brief Write what the function does here
     *
<<<<<<< HEAD
     * @param i
     *
     * @return
     **/
    const const_iterator &operator -=(ssize_t i)
    {
        return *this = operator -(i);
    }

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    const const_iterator &operator ++()
    {
        return operator +=(1);
    }

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    const const_iterator &operator --()
    {
        return operator -=(1);
    }

    /**
     * @brief Write what the function does here
     *
     * @param int
     *
     * @return
     **/
    const_iterator operator ++(int)
    {
        const_iterator retval = *this;
        operator ++();
        return retval;
    }

    /**
     * @brief Write what the function does here
     *
     * @param int
     *
     * @return
     **/
    const_iterator operator --(int)
    {
        const_iterator retval = *this;
        operator --();
        return retval;
    }

    /**
     * @brief Write what the function does here
     *
     * @param r
     *
     * @return
     **/
    bool operator >(const const_iterator &r) const
    {
        return pointIterator > r.pointIterator;
    }

    /**
     * @brief Write what the function does here
     *
     * @param r
     *
     * @return
     **/
    bool operator >=(const const_iterator &r) const
    {
        return pointIterator >= r.pointIterator;
    }

    /**
     * @brief Write what the function does here
     *
     * @param r
     *
     * @return
     **/
    bool operator <(const const_iterator &r) const
    {
        return pointIterator < r.pointIterator;
    }

    /**
     * @brief Write what the function does here
     *
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
     * @param r
     *
     * @return
     **/
    bool operator <=(const const_iterator &r) const
<<<<<<< HEAD
    {
        return pointIterator <= r.pointIterator;
    }
};
typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

/**
 * @brief Write what the function does here
 *
 * @return
 **/
size_t size() const
{
    return length;
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_iterator begin() const
{
    return const_iterator(points.begin(), colors.begin(), textureCoords.begin());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_iterator end() const
{
    return const_iterator(points.end(), colors.end(), textureCoords.end());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_iterator cbegin() const
{
    return const_iterator(points.begin(), colors.begin(), textureCoords.begin());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_iterator cend() const
{
    return const_iterator(points.end(), colors.end(), textureCoords.end());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_reverse_iterator rbegin() const
{
    return const_reverse_iterator(end());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_reverse_iterator rend() const
{
    return const_reverse_iterator(begin());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_reverse_iterator crbegin() const
{
    return const_reverse_iterator(end());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_reverse_iterator crend() const
{
    return const_reverse_iterator(begin());
}

/**
 * @brief Write what the function does here
 *
 * @param m
 **/
void add(const Mesh_t &m)
{

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    if(texture())
    {

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        if(m.texture() && m.texture() != texture())
        {
            throw ImageNotSameException();
        }
=======
    {
        return pointIterator <= r.pointIterator;
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    }
};
typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

<<<<<<< HEAD
=======
/**
 * @brief Write what the function does here
 *
 * @return
 **/
size_t size() const
{
    return length;
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_iterator begin() const
{
    return const_iterator(points.begin(), colors.begin(), textureCoords.begin());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_iterator end() const
{
    return const_iterator(points.end(), colors.end(), textureCoords.end());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_iterator cbegin() const
{
    return const_iterator(points.begin(), colors.begin(), textureCoords.begin());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_iterator cend() const
{
    return const_iterator(points.end(), colors.end(), textureCoords.end());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_reverse_iterator rbegin() const
{
    return const_reverse_iterator(end());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_reverse_iterator rend() const
{
    return const_reverse_iterator(begin());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_reverse_iterator crbegin() const
{
    return const_reverse_iterator(end());
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
const_reverse_iterator crend() const
{
    return const_reverse_iterator(begin());
}

/**
 * @brief Write what the function does here
 *
 * @param m
 **/
void add(const Mesh_t &m)
{

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    if(texture())
    {

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        if(m.texture() && m.texture() != texture())
        {
            throw ImageNotSameException();
        }
    }

>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    else
    {
        textureInternal = m.texture();
    }
    length += m.length;
    points.insert(points.end(), m.points.begin(), m.points.end());
    colors.insert(colors.end(), m.colors.begin(), m.colors.end());
    textureCoords.insert(textureCoords.end(), m.textureCoords.begin(), m.textureCoords.end());
}

/**
 * @brief Write what the function does here
 *
 * @param m
 **/
void add(Mesh m)
{
    add(*m);
}

/**
 * @brief Write what the function does here
 *
 * @param m
 **/
void add(TransformedMesh m)
{
    Mesh_t m2(m);
    add(m2);
}
/*friend void writeMesh(Mesh mesh, Writer &writer, Client &client);
  friend Mesh readMesh(Reader &reader, Client &client);*/
friend Mesh interpolateColors(Mesh dest, Mesh mesh, Color cNXNYNZ, Color cNXNYPZ, Color cNXPYNZ, Color cNXPYPZ, Color cPXNYNZ, Color cPXNYPZ, Color cPXPYNZ, Color cPXPYPZ);
friend Mesh interpolateColors(Mesh mesh, Color cNXNYNZ, Color cNXNYPZ, Color cNXPYNZ, Color cNXPYPZ, Color cPXNYNZ, Color cPXNYPZ, Color cPXPYNZ, Color cPXPYPZ);
friend Mesh lightColors(Mesh dest, Mesh mesh, VectorF lightDir, float ambient, float diffuse);
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
};

/**
 * @brief Write what the function does here
 *
 * @param dest
 * @param mesh
 * @param cNXNYNZ
 * @param cNXNYPZ
 * @param cNXPYNZ
 * @param cNXPYPZ
 * @param cPXNYNZ
 * @param cPXNYPZ
 * @param cPXPYNZ
 * @param cPXPYPZ
 *
 * @return
 **/
inline Mesh interpolateColors(Mesh dest, Mesh mesh, Color cNXNYNZ, Color cNXNYPZ, Color cNXPYNZ, Color cNXPYPZ, Color cPXNYNZ, Color cPXNYPZ, Color cPXPYNZ, Color cPXPYPZ)
{
    assert(dest && mesh);

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    if(dest->texture())
    {

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        if(mesh->texture() && mesh->texture() != dest->texture())
        {
            throw ImageNotSameException();
        }
    }

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    /**
     * @brief Write what the function does here
     *
     * @return
     **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    else
    {
        dest->textureInternal = mesh->texture();
    }
    dest->length += mesh->length;
    dest->textureCoords.insert(dest->textureCoords.end(), mesh->textureCoords.begin(), mesh->textureCoords.end());
    size_t vi = 0, ci = 0;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    /**
     * @brief Write what the function does here
     *
     * @return
     **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    for(size_t i = 0; i < mesh->length; i++)
    {
        float x = mesh->points[vi++];
        float y = mesh->points[vi++];
        float z = mesh->points[vi++];
        dest->points.push_back(x);
        dest->points.push_back(y);
        dest->points.push_back(z);
        Color c;
        c.r = mesh->colors[ci++];
        c.g = mesh->colors[ci++];
        c.b = mesh->colors[ci++];
        c.a = mesh->colors[ci++];
        c = scale(c, interpolate(x, interpolate(y, interpolate(z, cNXNYNZ, cNXNYPZ), interpolate(z, cNXPYNZ, cNXPYPZ)), interpolate(y, interpolate(z, cPXNYNZ, cPXNYPZ), interpolate(z, cPXPYNZ, cPXPYPZ))));
        dest->colors.push_back(c.r);
        dest->colors.push_back(c.g);
        dest->colors.push_back(c.b);
        dest->colors.push_back(c.a);
    }
    return dest;
}

/**
 * @brief Write what the function does here
 *
 * @param mesh
 * @param cNXNYNZ
 * @param cNXNYPZ
 * @param cNXPYNZ
 * @param cNXPYPZ
 * @param cPXNYNZ
 * @param cPXNYPZ
 * @param cPXPYNZ
 * @param cPXPYPZ
 *
 * @return
 **/
inline Mesh interpolateColors(Mesh mesh, Color cNXNYNZ, Color cNXNYPZ, Color cNXPYNZ, Color cNXPYPZ, Color cPXNYNZ, Color cPXNYPZ, Color cPXPYNZ, Color cPXPYPZ)
{
    assert(mesh);
    Mesh dest = Mesh(new Mesh_t);
    dest->textureInternal = mesh->texture();
    dest->length += mesh->length;
    dest->textureCoords.insert(dest->textureCoords.end(), mesh->textureCoords.begin(), mesh->textureCoords.end());
    size_t vi = 0, ci = 0;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    /**
     * @brief Write what the function does here
     *
     * @return
     **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    for(size_t i = 0; i < mesh->length; i++)
    {
        float x = mesh->points[vi++];
        float y = mesh->points[vi++];
        float z = mesh->points[vi++];
        dest->points.push_back(x);
        dest->points.push_back(y);
        dest->points.push_back(z);
        Color c;
        c.r = mesh->colors[ci++];
        c.g = mesh->colors[ci++];
        c.b = mesh->colors[ci++];
        c.a = mesh->colors[ci++];
        c = scale(c, interpolate(x, interpolate(y, interpolate(z, cNXNYNZ, cNXNYPZ), interpolate(z, cNXPYNZ, cNXPYPZ)), interpolate(y, interpolate(z, cPXNYNZ, cPXNYPZ), interpolate(z, cPXPYNZ, cPXPYPZ))));
        dest->colors.push_back(c.r);
        dest->colors.push_back(c.g);
        dest->colors.push_back(c.b);
        dest->colors.push_back(c.a);
    }
    return dest;
}

/**
 * @brief Write what the function does here
 *
 * @param mesh
 * @param lightDir
 * @param ambient
 * @param diffuse
 *
 * @return
 **/
inline Mesh lightColors(Mesh mesh, VectorF lightDir, float ambient, float diffuse)
{
    vector<Triangle> triangles;
    triangles.reserve(mesh->size());

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    /**
     * @brief Write what the function does here
     *
     * @param mesh
     *
     * @return
     **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    for(Triangle t : *mesh)
    {
        float v = dot(t.normal(), lightDir);
        if(v < 0)
            v = 0;
        v *= diffuse;
        v += ambient;
        t.c[0] = scale(t.c[0], v);
        t.c[1] = scale(t.c[1], v);
        t.c[2] = scale(t.c[2], v);
        triangles.push_back(t);
    }
    return Mesh(new Mesh_t(mesh->texture(), triangles));
}

/**
 * @brief Write what the function does here
 *
 * @param mesh
 * @param writer
 * @param client
 **/
/*inline void writeMesh(Mesh mesh, Writer &writer, Client &client)
  {
  assert(mesh && (uint32_t)mesh->size() == mesh->size());
  writer.writeU32(mesh->size());
  mesh->texture().write(writer, client);

<<<<<<< HEAD
<<<<<<< HEAD
  for(float v : mesh->points)
  {
  writer.writeF32(v);
  }

  for(float v : mesh->textureCoords)
  {
  writer.writeF32(v);
  }

  for(float v : mesh->colors)
  {
  writer.writeF32(v);
  }
  }

=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
/**
 * @brief Write what the function does here
 *
 * @param points
 *
 * @return
 **/
for(float v : mesh->points)
{
    writer.writeF32(v);
<<<<<<< HEAD
=======
}

/**
 * @brief Write what the function does here
 *
 * @param textureCoords
 *
 * @return
 **/
for(float v : mesh->textureCoords)
{
    writer.writeF32(v);
}

/**
 * @brief Write what the function does here
 *
 * @param colors
 *
 * @return
 **/
for(float v : mesh->colors)
{
    writer.writeF32(v);
}
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
}

/**
 * @brief Write what the function does here
 *
<<<<<<< HEAD
 * @param textureCoords
 *
 * @return
 **/
for(float v : mesh->textureCoords)
{
    writer.writeF32(v);
}

/**
 * @brief Write what the function does here
 *
 * @param colors
 *
 * @return
 **/
for(float v : mesh->colors)
{
    writer.writeF32(v);
}
}

>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
/**
 * @brief Write what the function does here
 *
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
 * @param reader
 * @param client
 *
 * @return
 **/
inline Mesh readMesh(Reader &reader, Client &client)
{
    return Mesh(new Mesh_t(reader, client));
}*/

/**
 * @brief Write what the function does here
 *
 * @return
 **/
inline TransformedMesh::operator Mesh() const
{
    return Mesh(new Mesh_t(*this));
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class Renderer final
{
    private:
        Renderer(const Renderer &) = delete;
        const Renderer operator =(const Renderer &) = delete;
    public:

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        Renderer()
        {
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        ~Renderer()
        {
        }
        Renderer &operator <<(const Mesh_t &m);

        /**
         * @brief Write what the function does here
         *
         * @param m
         *
         * @return
         **/
        Renderer &operator <<(Mesh m)
        {
            operator <<(*m);
            return *this;
        }

        /**
         * @brief Write what the function does here
         *
         * @param m
         *
         * @return
         **/
        Renderer &operator <<(TransformedMesh m)
        {
            Mesh_t m2(m);
            operator <<(m2);
            return *this;
        }
};
#endif // MESH_H_INCLUDED
