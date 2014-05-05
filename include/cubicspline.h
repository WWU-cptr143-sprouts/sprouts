#ifndef CUBICSPLINE_H_INCLUDED
#define CUBICSPLINE_H_INCLUDED

#include <cmath>

using namespace std;

struct CubicSpline2D final
{
    float x0, y0, x1, y1, dx0, dy0, dx1, dy1;
    CubicSpline2D()
        : x0(0), y0(0), x1(0), y1(0), dx0(0), dy0(0), dx1(0), dy1(0)
    {
    }
    CubicSpline2D(float x0, float y0, float x1, float y1)
        : x0(x0), y0(y0), x1(x1), y1(y1), dx0(x1 - x0), dy0(y1 - y0), dx1(x1 - x0), dy1(y1 - y0)
    {
    }
    CubicSpline2D(float x0, float y0, float x1, float y1, float dx0, float dy0, float dx1, float dy1)
        : x0(x0), y0(y0), x1(x1), y1(y1), dx0(dx0), dy0(dy0), dx1(dx1), dy1(dy1)
    {
    }
    float getConstantX() const
    {
        return x0;
    }
    float getLinearX() const
    {
        return dx0;
    }
    float getQuadraticX() const
    {
        return -3 * x0 - 2 * dx0 - dx1 + 3 * x1;
    }
    float getCubicX() const
    {
        return 2 * x0 + dx0 + dx1 - 2 * x1;
    }
    float getConstantY() const
    {
        return y0;
    }
    float getLinearY() const
    {
        return dy0;
    }
    float getQuadraticY() const
    {
        return -3 * y0 - 2 * dy0 - dy1 + 3 * y1;
    }
    float getCubicY() const
    {
        return 2 * y0 + dy0 + dy1 - 2 * y1;
    }
    float evaluateX(float t) const
    {
        return t * t * t * getCubicX() + t * t * getQuadraticX() + t * getLinearX() + getConstantX();
    }
    float evaluateY(float t) const
    {
        return t * t * t * getCubicY() + t * t * getQuadraticY() + t * getLinearY() + getConstantY();
    }
};

#endif // CUBICSPLINE_H_INCLUDED
