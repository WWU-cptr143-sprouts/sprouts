#ifndef CUBICSPLINE_H_INCLUDED
#define CUBICSPLINE_H_INCLUDED

#include <cmath>
#include "mesh.h"
#include "vector.h"
#include "matrix.h"
#include <vector>

/**************
/*Description:
/*Input:
/*Output:
**************/

using namespace std;

struct CubicSpline final
{
    VectorF p0, p1, dp0, dp1; // p0 : starting point, p1 : ending point, dp0 derivative at t = 0, dp1 derivative at t = 1
    CubicSpline()
        : p0(0), p1(0), dp0(0), dp1(0)
    {
    }
    CubicSpline(VectorF p0, VectorF p1)
        : p0(p0), p1(p1), dp0(p1 - p0), dp1(p1 - p0)
    {
    }
    CubicSpline(VectorF p0, VectorF p1, VectorF dp0, VectorF dp1)
        : p0(p0), p1(p1), dp0(dp0), dp1(dp1)
    {
    }
    static Mesh renderSplineList(const vector<CubicSpline> &splines, TextureDescriptor texture, Color color, float lineWidth);
    VectorF getConstant() const
    {
        return p0;
    }
    VectorF getLinear() const
    {
        return dp0;
    }
    VectorF getQuadratic() const
    {
        return -3 * p0 - 2 * dp0 - dp1 + 3 * p1;
    }
    VectorF getCubic() const
    {
        return 2 * p0 + dp0 + dp1 - 2 * p1;
    }
    VectorF evaluate(float t) const
    {
        return t * t * t * getCubic() + t * t * getQuadratic() + t * getLinear() + getConstant();
    }
    bool intersects(const CubicSpline & rt, VectorF ignoreAxis = VectorF(0, 0, 1)) const; /// returns true if the splines intersect each other when projected onto the plane with normal == ignoreAxis
    CubicSpline subSection(float minT, float maxT) const
    {
        return CubicSpline(evaluate(minT), evaluate(maxT), (maxT - minT) * (minT * (3 * minT * getCubic() + 2 * getQuadratic()) + getLinear()), (maxT - minT) * (maxT * (3 * maxT * getCubic() + 2 * getQuadratic()) + getLinear()));
    }
    CubicSpline reversed() const
    {
        return CubicSpline(p1, p0, -dp1, -dp0);
    }
    bool isSelfIntersecting() const;
};

inline vector<CubicSpline> splinesFromLines(const vector<VectorF> & line)
{
    vector<CubicSpline> retval;
    if(line.size() < 2)
        return retval;
    retval.reserve(line.size() - 1);
    vector<VectorF> derivatives;
    derivatives.reserve(line.size());
    derivatives.push_back(VectorF(0));
    for(size_t i = 2; i < line.size(); i++)
    {
        derivatives.push_back((line[i] - line[i - 2]) / sqrt(2));
    }
    derivatives.push_back(VectorF(0));
    for(size_t i = 1; i < line.size(); i++)
    {
        retval.push_back(CubicSpline(line[i - 1], line[i], derivatives[i - 1], derivatives[i]));
    }
    return retval;
}

bool isPathSelfIntersecting(const vector<CubicSpline> & path);
bool linesIntersect(VectorF start1, VectorF end1, VectorF start2, VectorF end2, float endSpace = eps);

inline CubicSpline transform(const Matrix & tform, CubicSpline spline)
{
    return CubicSpline(tform.apply(spline.p0), tform.apply(spline.p1), tform.applyNoTranslate(spline.dp0), tform.applyNoTranslate(spline.dp1));
}

inline vector<CubicSpline> transform(const Matrix & tform, vector<CubicSpline> splines)
{
    for(CubicSpline & spline : splines)
    {
        spline = transform(tform, spline);
    }
    return std::move(splines);
}

#endif // CUBICSPLINE_H_INCLUDED
