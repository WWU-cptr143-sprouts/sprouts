#ifndef CUBICSPLINE_H_INCLUDED
#define CUBICSPLINE_H_INCLUDED
#include <cmath>
#include "mesh.h"
#include "vector.h"
#include <vector>
using namespace std;

/**
 * @brief Write what the function does here
 *
 * @retval (variable) (description of variable)
 **/
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

    /**
     * @brief Write what the function does here
     *
     * @retval (variable) (description of variable)
     **/
    VectorF getConstant() const
    {
        return p0;
    }

    /**
     * @brief Write what the function does here
     *
     * @retval (variable) (description of variable)
     **/
    VectorF getLinear() const
    {
        return dp0;
    }

    /**
     * @brief Write what the function does here
     *
     * @retval (variable) (description of variable)
     **/
    VectorF getQuadratic() const
    {
        return -3 * p0 - 2 * dp0 - dp1 + 3 * p1;
    }

    /**
     * @brief Write what the function does here
     *
     * @retval (variable) (description of variable)
     **/
    VectorF getCubic() const
    {
        return 2 * p0 + dp0 + dp1 - 2 * p1;
    }

    /**
     * @brief Write what the function does here
     *
     * @param t
     *
     * @retval (variable) (description of variable)
     **/
    VectorF evaluate(float t) const
    {
        return t * t * t * getCubic() + t * t * getQuadratic() + t * getLinear() + getConstant();
    }
    bool intersects(const CubicSpline & rt, VectorF ignoreAxis = VectorF(0, 0, 1)) const; /// returns true if the splines intersect each other when projected onto the plane with normal == ignoreAxis

    /**
     * @brief Write what the function does here
     *
     * @param minT
     * @param maxT
     *
     * @retval (variable) (description of variable)
     **/
    CubicSpline subSection(float minT, float maxT) const
    {
        return CubicSpline(evaluate(minT), evaluate(maxT), (maxT - minT) * (minT * (3 * minT * getCubic() + 2 * getQuadratic()) + getLinear()), (maxT - minT) * (maxT * (3 * maxT * getCubic() + 2 * getQuadratic()) + getLinear()));
    }
};
#endif // CUBICSPLINE_H_INCLUDED
