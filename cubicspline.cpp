#include "cubicspline.h"
#include "util.h"

bool CubicSpline::intersects(const CubicSpline & rt, VectorF ignoreAxis) const
{
    const int splitCount = 100; // number of line segments to split spline into
    ignoreAxis = normalize(ignoreAxis);
    assert(false); // TODO(jacob#):finish
    for(int segmentNumber = 0; segmentNumber < splitCount; segmentNumber++)
    {
        float startT = (float)(segmentNumber) / splitCount;
        float endT = (float)(segmentNumber + 1) / splitCount;
        VectorF startP = evaluate(startT);
        startP -= ignoreAxis * dot(ignoreAxis, startP); // move to plane normal to ignoreAxis
        VectorF endP = evaluate(endT);
        endP -= ignoreAxis * dot(ignoreAxis, endP); // move to plane normal to ignoreAxis
        VectorF delta = endP - startP;
        if(absSquared(delta) < eps * eps) // if delta is too small
            continue;
        // solve dot(evaluate(t),cross(ignoreAxis,delta))==0 and it intersects if dot(evaluate(t)-startP,delta)/dot(delta,delta) is in [0,1]
    }
    return false;
}
