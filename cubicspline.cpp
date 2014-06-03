#include "cubicspline.h"
#include "util.h"
#include "generate.h"

bool CubicSpline::intersects(const CubicSpline & rt, VectorF ignoreAxis) const
{
    const int splitCount = 50; // number of line segments to split spline into
    ignoreAxis = normalize(ignoreAxis);
    for(int segmentNumber = 0; segmentNumber < splitCount; segmentNumber++)
    {
        float startT = (float)(segmentNumber) / splitCount;
        float endT = (float)(segmentNumber + 1) / splitCount;
        VectorF startP = rt.evaluate(startT);
        startP -= ignoreAxis * dot(ignoreAxis, startP); // move to plane normal to ignoreAxis
        VectorF endP = rt.evaluate(endT);
        endP -= ignoreAxis * dot(ignoreAxis, endP); // move to plane normal to ignoreAxis
        VectorF delta = endP - startP;
        if(absSquared(delta) < eps * eps) // if delta is too small
            continue;
        // solve dot(evaluate(t), cross(ignoreAxis, delta)) == 0 and it intersects if dot(evaluate(t) - startP, delta) / dot(delta, delta) is in [0, 1] and t is in [0, 1]
        VectorF normal = cross(ignoreAxis, delta);
        float cubic = dot(getCubic(), normal);
        float quadratic = dot(getQuadratic(), normal);
        float linear = dot(getLinear(), normal);
        float constant = dot(getConstant(), normal);
        float intersections[3];
        int intersectionCount = solveCubic(constant, linear, quadratic, cubic, intersections);
        for(int i = 0; i < intersectionCount; i++)
        {
            float t = intersections[i];
            if(t < 0 || t > 1)
                continue;
            float v = dot(evaluate(t) - startP, delta) / dot(delta, delta);
            if(v < 0 || v > 1)
                continue;
            return true;
        }
    }
    return false;
}

Mesh CubicSpline::renderSplineList(const vector<CubicSpline> &splines, TextureDescriptor texture, Color color, float lineWidth)
{
    if(splines.size() == 0)
        return Mesh(new Mesh_t());
    const int pointsPerSpline = 20;
    vector<VectorF> points;
    points.reserve(1 + pointsPerSpline * splines.size());
    points.push_back(splines[0].evaluate(0));
    for(size_t i = 0; i < splines.size(); i++)
    {
        for(int j = 1; j <= pointsPerSpline; j++)
        {
            points.push_back(splines[i].evaluate((float)j / pointsPerSpline));
        }
    }
    return Generate::line(points, texture, color, lineWidth);
}

namespace
{
bool linesIntersect(VectorF start1, VectorF end1, VectorF start2, VectorF end2, float endSpace = eps)
{
    VectorF delta1 = end1 - start1;
    VectorF delta2 = end2 - start2;
    if(absSquared(delta1) < eps * eps || absSquared(delta2) < eps * eps)
        return false;
    VectorF normal = cross(cross(delta1, delta2), delta1);
    if(abs(dot(normal, delta1)) < eps * eps * eps * eps * eps * eps)
    {
        if(absSquared((start1 + end1) - (start2 + end2)) < eps * eps)
            return true;
        return false;
    }
    normal = normalize(normal);
    float t = findIntersectionPoint(start1, end1, normal, -dot(normal, start2));
    if(t < endSpace || t > 1 - endSpace)
        return false;
    VectorF pt = start1 + t * (end1 - start1);
    t = dot(delta2, pt - start2) / abs(delta2);
    if(t < endSpace || t > 1 - endSpace)
        return false;
    return absSquared(pt - start2 - t * (end2 - start2)) < eps * eps;
}
}

bool CubicSpline::isSelfIntersecting() const
{
    const int pointsPerSpline = 20;
    vector<VectorF> points;
    points.reserve(1 + pointsPerSpline);
    for(int i = 0; i <= pointsPerSpline; i++)
    {
        points.push_back(evaluate((float)i / pointsPerSpline));
    }
    for(size_t i = 2; i < points.size(); i++)
    {
        for(size_t j = 1; j < i; j++)
        {
            if(linesIntersect(points[i - 1], points[i], points[j - 1], points[j], (abs(i - j) > 1 ? 0 : eps)))
                return true;
        }
    }
    return false;
}

bool isPathSelfIntersecting(const vector<CubicSpline> & splines)
{
    if(splines.size() == 0)
        return false;
    const int pointsPerSpline = 20;
    vector<VectorF> points;
    points.reserve(1 + pointsPerSpline * splines.size());
    points.push_back(splines[0].evaluate(0));
    for(size_t i = 0; i < splines.size(); i++)
    {
        for(int j = 1; j <= pointsPerSpline; j++)
        {
            points.push_back(splines[i].evaluate((float)j / pointsPerSpline));
        }
    }
    for(size_t i = 2; i < points.size(); i++)
    {
        for(size_t j = 1; j < i; j++)
        {
            if(linesIntersect(points[i - 1], points[i], points[j - 1], points[j], ((abs(i - j) > 1 && (i != points.size() - 1 || j != 1)) ? 0 : eps)))
                return true;
        }
    }
    return false;
}
