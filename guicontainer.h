#ifndef GUICONTAINER_H_INCLUDED
#define GUICONTAINER_H_INCLUDED

#include "guielement.h"
#include <vector>

class GUIContainer final : public GUIElement
{
private:
    vector<shared_ptr<GUIElement>> elements;
    size_t currentFocusIndex = 0;
    static constexpr const size_t NoElement = (size_t)-1;
    size_t lastMouseElement = NoElement;
public:
    using GUIElement::render;
    GUIContainer(float minX, float maxX, float minY, float maxY)
        : GUIElement(minX, maxX, minY, maxY)
    {
    }
    shared_ptr<GUIContainer> add(shared_ptr<GUIElement> element)
    {
        assert(element != nullptr);
        elements.push_back(element);
        element->parent = dynamic_pointer_cast<GUIContainer>(shared_from_this());
        return dynamic_pointer_cast<GUIContainer>(shared_from_this());
    }

    virtual shared_ptr<GUIElement> getFocusElement() override
    {
        if(currentFocusIndex >= elements.size())
            return shared_from_this();
        return elements[currentFocusIndex];
    }

    virtual bool canHaveKeyboardFocus() const override
    {
        for(shared_ptr<GUIElement> e : elements)
            if(e->canHaveKeyboardFocus())
                return true;
        return false;
    }

    virtual void firstFocusElement() override
    {
        for(currentFocusIndex = 0; currentFocusIndex < elements.size(); currentFocusIndex++)
        {
            if(elements[currentFocusIndex]->canHaveKeyboardFocus())
                return;
        }
        currentFocusIndex = 0;
    }

    virtual void lastFocusElement() override
    {
        currentFocusIndex = elements.size() - 1;
        for(size_t i = 0; i < elements.size(); currentFocusIndex--, i++)
        {
            if(elements[currentFocusIndex]->canHaveKeyboardFocus())
                return;
        }
        currentFocusIndex = 0;
    }

    virtual bool prevFocusElement() override /// returns true when reached container boundary
    {
        if(elements.size() == 0)
            return true;
        if(currentFocusIndex >= elements.size())
        {
            currentFocusIndex = 0;
            return true;
        }
        if(!elements[currentFocusIndex]->prevFocusElement())
            return false;
        do
        {
            currentFocusIndex += elements.size() - 1;
            currentFocusIndex %= elements.size();
            if(elements[currentFocusIndex]->canHaveKeyboardFocus())
            {
                elements[currentFocusIndex]->lastFocusElement();
                return currentFocusIndex != elements.size() - 1;
            }
        }
        while(currentFocusIndex != elements.size() - 1);
        lastFocusElement();
        return true;
    }

    virtual bool nextFocusElement() override /// returns true when reached container boundary
    {
        if(elements.size() == 0)
            return true;
        if(currentFocusIndex >= elements.size())
        {
            currentFocusIndex = 0;
            return true;
        }
        if(!elements[currentFocusIndex]->nextFocusElement())
            return false;
        do
        {
            currentFocusIndex++;
            currentFocusIndex %= elements.size();
            if(elements[currentFocusIndex]->canHaveKeyboardFocus())
            {
                elements[currentFocusIndex]->firstFocusElement();
                return currentFocusIndex != 0;
            }
        }
        while(currentFocusIndex != 0);
        firstFocusElement();
        return true;
    }
private:
    size_t getIndexFromPosition(float x, float y)
    {
        VectorF v = Display::transformMouseTo3D(x, y);
        for(size_t i = 0, retval = elements.size() - 1; i < elements.size(); i++, retval--)
        {
            if(elements[retval]->isPointInside(v.x, v.y))
                return retval;
        }
        return NoElement;
    }
public:
    virtual bool handleMouseUp(MouseUpEvent &event) override
    {
        size_t index = getIndexFromPosition(event.x, event.y);
        if(index != lastMouseElement)
        {
            if(lastMouseElement != NoElement)
            {
                elements[lastMouseElement]->handleMouseMoveOut(event);
            }
            lastMouseElement = index;
            if(index != NoElement)
            {
                elements[index]->handleMouseMoveIn(event);
            }
        }
        if(index != NoElement)
        {
            return elements[index]->handleMouseUp(event);
        }
        return GUIElement::handleMouseUp(event);
    }
    virtual bool handleMouseDown(MouseDownEvent &event) override
    {
        size_t index = getIndexFromPosition(event.x, event.y);
        if(index != lastMouseElement)
        {
            if(lastMouseElement != NoElement)
            {
                elements[lastMouseElement]->handleMouseMoveOut(event);
            }
            lastMouseElement = index;
            if(index != NoElement)
            {
                elements[index]->handleMouseMoveIn(event);
            }
        }
        if(index != NoElement)
        {
            return elements[index]->handleMouseDown(event);
        }
        return GUIElement::handleMouseDown(event);
    }
    virtual bool handleMouseMove(MouseMoveEvent &event) override
    {
        size_t index = getIndexFromPosition(event.x, event.y);
        if(index != lastMouseElement)
        {
            if(lastMouseElement != NoElement)
            {
                elements[lastMouseElement]->handleMouseMoveOut(event);
            }
            lastMouseElement = index;
            if(index != NoElement)
            {
                elements[index]->handleMouseMoveIn(event);
            }
        }
        if(index != NoElement)
        {
            return elements[index]->handleMouseMove(event);
        }
        return GUIElement::handleMouseMove(event);
    }
    virtual bool handleMouseMoveOut(MouseEvent &event) override
    {
        if(lastMouseElement != NoElement)
        {
            shared_ptr<GUIElement> e = elements[lastMouseElement];
            lastMouseElement = NoElement;
            return e->handleMouseMoveOut(event);
        }
        return GUIElement::handleMouseMoveOut(event);
    }
    virtual bool handleMouseMoveIn(MouseEvent &event) override
    {
        size_t index = getIndexFromPosition(event.x, event.y);
        lastMouseElement = index;
        if(index != NoElement)
        {
            return elements[index]->handleMouseMoveIn(event);
        }
        return GUIElement::handleMouseMoveIn(event);
    }
    virtual bool handleMouseScroll(MouseScrollEvent &event) override
    {
        size_t index = getIndexFromPosition(event.x, event.y);
        if(index != lastMouseElement)
        {
            if(lastMouseElement != NoElement)
            {
                elements[lastMouseElement]->handleMouseMoveOut(event);
            }
            lastMouseElement = index;
            if(index != NoElement)
            {
                elements[index]->handleMouseMoveIn(event);
            }
        }
        if(index != NoElement)
        {
            return elements[index]->handleMouseScroll(event);
        }
        return GUIElement::handleMouseScroll(event);
    }
    virtual bool handleKeyUp(KeyUpEvent &event) override
    {
        shared_ptr<GUIElement> e = getFocusElement();
        if(e == shared_from_this())
            return GUIElement::handleKeyUp(event);
        return e->handleKeyUp(event);
    }
    virtual bool handleKeyDown(KeyDownEvent &event) override
    {
        shared_ptr<GUIElement> e = getFocusElement();
        bool retval;
        if(e == shared_from_this())
            retval = GUIElement::handleKeyDown(event);
        else
            retval = e->handleKeyDown(event);
        if(retval)
            return true;
        if(getParent() != nullptr || (event.mods & (KeyboardModifiers_Ctrl | KeyboardModifiers_Alt | KeyboardModifiers_Meta | KeyboardModifiers_Mode)) != 0)
            return false;
        if(event.key == KeyboardKey_Tab)
        {
            if(event.mods & KeyboardModifiers_Shift)
                prevFocusElement();
            else
                nextFocusElement();
            return true;
        }
        else if(event.mods & KeyboardModifiers_Shift)
            return false;
        else if(event.key == KeyboardKey_Home)
        {
            firstFocusElement();
            return true;
        }
        else if(event.key == KeyboardKey_End)
        {
            lastFocusElement();
            return true;
        }
        return false;
    }
    virtual bool handleKeyPress(KeyPressEvent &event) override
    {
        shared_ptr<GUIElement> e = getFocusElement();
        if(e == shared_from_this())
            return GUIElement::handleKeyPress(event);
        return e->handleKeyPress(event);
    }
    virtual bool handleQuit(QuitEvent &event) override
    {
        shared_ptr<GUIElement> e = getFocusElement();
        if(e == shared_from_this())
            return GUIElement::handleQuit(event);
        return e->handleQuit(event);
    }
    virtual Mesh render(float minZ, float maxZ, bool hasFocus) const
    {
        Mesh retval = nullptr;
        for(size_t i = 0; i < elements.size(); i++)
        {
            Mesh mesh = elements[i]->render(interpolate((float)(i + 1) / elements.size(), maxZ, minZ), interpolate((float)i / elements.size(), maxZ, minZ), hasFocus && i == currentFocusIndex);
            if(retval != nullptr)
                retval->add(mesh);
            else
                retval = mesh;
        }
        if(retval == nullptr)
            return Mesh(new Mesh_t());
        return retval;
    }
    void setFocus(shared_ptr<GUIElement> e)
    {
        if(e == nullptr)
            return;
        for(size_t i = 0; i < elements.size(); i++)
        {
            if(elements[i] == e)
            {
                if(e->canHaveKeyboardFocus())
                {
                    currentFocusIndex = i;
                    if(getParent() != nullptr)
                    {
                        getParent()->setFocus(shared_from_this());
                    }
                }
            }
        }
    }
    virtual void reset()
    {
        for(shared_ptr<GUIElement> e : elements)
            e->reset();
        firstFocusElement();
    }
};

#endif // GUICONTAINER_H_INCLUDED
