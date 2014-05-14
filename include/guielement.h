#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include "event.h"

class GUIElement : public EventHandler
{
public:
    GUIElement(float minX, float maxX, float minY, float maxY);
    virtual ~GUIElement();
    Mesh render() const
    {
        return render(0.1, 10);
    }
protected:
    float minX, maxX, minY, maxY;
    virtual Mesh render(float minZ, float maxZ) const = 0;
};

#endif // GUIELEMENT_H
