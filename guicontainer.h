#ifndef GUICONTAINER_H_INCLUDED
#define GUICONTAINER_H_INCLUDED
#include "guielement.h"
#include "platform.h"
#include <vector>

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class GUIContainer : public GUIElement
{
    private:
        vector<shared_ptr<GUIElement>> elements;
        size_t currentFocusIndex = 0;
        size_t lastMouseElement = NoElement;
    protected:
        static constexpr const size_t NoElement = (size_t)-1;

        /**
         * @brief Write what the function does here
         *
         virtual void onSetFocus()
         {
         }

        /**
         * @brief Write what the function does here
         *
         * @param index
         *
         * @return
         **/
        shared_ptr<GUIElement> getElement(size_t index)
        {
            return elements[index];
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        size_t getElementCount()
        {
            return elements.size();
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        size_t getCurrentElementIndex()
        {
            return currentFocusIndex;
        }
    public:
        using GUIElement::render;
        GUIContainer(float minX, float maxX, float minY, float maxY)

            /**
             * @brief Write what the function does here
             *
             * @param minX
             * @param maxX
             * @param minY
             * @param maxY
             *
             * @return
             **/
            : GUIElement(minX, maxX, minY, maxY)
            {
            }

        /**
         * @brief Write what the function does here
         *
         * @param element
         *
         * @return
         **/
        shared_ptr<GUIContainer> add(shared_ptr<GUIElement> element)
        {
            assert(element != nullptr);
            elements.push_back(element);
            element->parent = dynamic_pointer_cast<GUIContainer>(shared_from_this());
            return dynamic_pointer_cast<GUIContainer>(shared_from_this());
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        virtual shared_ptr<GUIElement> getFocusElement() override final
        {
            if(currentFocusIndex >= elements.size())
                return shared_from_this();
            return elements[currentFocusIndex];
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        virtual bool canHaveKeyboardFocus() const override final
        {
            for(shared_ptr<GUIElement> e : elements)
                if(e->canHaveKeyboardFocus())
                    return true;
            return false;
        }

        /**
         * @brief Write what the function does here
         *
         virtual void firstFocusElement() override final
         {

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        for(currentFocusIndex = 0; currentFocusIndex < elements.size(); currentFocusIndex++)
        {

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            if(elements[currentFocusIndex]->canHaveKeyboardFocus())
            {
                elements[currentFocusIndex]->firstFocusElement();
                onSetFocus();
                return;
            }
        }
        currentFocusIndex = 0;
        onSetFocus();
}

/**
 * @brief Write what the function does here
 *
 virtual void lastFocusElement() override final
 {
 currentFocusIndex = elements.size() - 1;

/**
 * @brief Write what the function does here
 *
 * @return
 **/
for(size_t i = 0; i < elements.size(); currentFocusIndex--, i++)
{

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    if(elements[currentFocusIndex]->canHaveKeyboardFocus())
    {
        elements[currentFocusIndex]->lastFocusElement();
        onSetFocus();
        return;
    }
}
currentFocusIndex = 0;
onSetFocus();
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
virtual bool prevFocusElement() override final /// returns true when reached container boundary
{
    if(elements.size() == 0)
        return true;

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    if(currentFocusIndex >= elements.size())
    {
        currentFocusIndex = 0;
        return true;
    }
    if(!elements[currentFocusIndex]->prevFocusElement())
        return false;

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    do
    {
        currentFocusIndex += elements.size() - 1;
        currentFocusIndex %= elements.size();

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        if(elements[currentFocusIndex]->canHaveKeyboardFocus())
        {
            elements[currentFocusIndex]->lastFocusElement();
            onSetFocus();
            return currentFocusIndex == elements.size() - 1;
        }
    }
    while(currentFocusIndex != elements.size() - 1);
    lastFocusElement();
    return true;
}

/**
 * @brief Write what the function does here
 *
 * @return
 **/
virtual bool nextFocusElement() override final /// returns true when reached container boundary
{
    if(elements.size() == 0)
        return true;

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    if(currentFocusIndex >= elements.size())
    {
        currentFocusIndex = 0;
        return true;
    }
    if(!elements[currentFocusIndex]->nextFocusElement())
        return false;

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    do
    {
        currentFocusIndex++;
        currentFocusIndex %= elements.size();

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        if(elements[currentFocusIndex]->canHaveKeyboardFocus())
        {
            elements[currentFocusIndex]->firstFocusElement();
            onSetFocus();
            return currentFocusIndex == 0;
        }
    }
    while(currentFocusIndex != 0);
    firstFocusElement();
    return true;
}
private:

/**
 * @brief Write what the function does here
 *
 * @param x
 * @param y
 *
 * @return
 **/
size_t getIndexFromPosition(float x, float y)
{
    VectorF v = Display::transformMouseTo3D(x, y);

    /**
     * @brief Write what the function does here
     *
     * @param 0
     *
     * @return
     **/
    for(size_t i = 0, retval = elements.size() - 1; i < elements.size(); i++, retval--)
    {
        if(elements[retval]->isPointInside(v.x, v.y))
            return retval;
    }
    return NoElement;
}
public:

/**
 * @brief Write what the function does here
 *
 * @param event
 *
 * @return
 **/
virtual bool handleMouseUp(MouseUpEvent &event) override final
{
    size_t index = getIndexFromPosition(event.x, event.y);

    /**
     * @brief Write what the function does here
     *
     * @param lastMouseElement
     *
     * @return
     **/
    if(index != lastMouseElement)
    {

        /**
         * @brief Write what the function does here
         *
         * @param NoElement
         *
         * @return
         **/
        if(lastMouseElement != NoElement)
        {
            elements[lastMouseElement]->handleMouseMoveOut(event);
        }
        lastMouseElement = index;

        /**
         * @brief Write what the function does here
         *
         * @param NoElement
         *
         * @return
         **/
        if(index != NoElement)
        {
            elements[index]->handleMouseMoveIn(event);
        }
    }

    /**
     * @brief Write what the function does here
     *
     * @param NoElement
     *
     * @return
     **/
    if(index != NoElement)
    {
        return elements[index]->handleMouseUp(event);
    }
    return GUIElement::handleMouseUp(event);
}

/**
 * @brief Write what the function does here
 *
 * @param event
 *
 * @return
 **/
virtual bool handleMouseDown(MouseDownEvent &event) override final
{
    size_t index = getIndexFromPosition(event.x, event.y);

    /**
     * @brief Write what the function does here
     *
     * @param lastMouseElement
     *
     * @return
     **/
    if(index != lastMouseElement)
    {

        /**
         * @brief Write what the function does here
         *
         * @param NoElement
         *
         * @return
         **/
        if(lastMouseElement != NoElement)
        {
            elements[lastMouseElement]->handleMouseMoveOut(event);
        }
        lastMouseElement = index;

        /**
         * @brief Write what the function does here
         *
         * @param NoElement
         *
         * @return
         **/
        if(index != NoElement)
        {
            elements[index]->handleMouseMoveIn(event);
        }
    }

    /**
     * @brief Write what the function does here
     *
     * @param NoElement
     *
     * @return
     **/
    if(index != NoElement)
    {
        return elements[index]->handleMouseDown(event);
    }
    return GUIElement::handleMouseDown(event);
}

/**
 * @brief Write what the function does here
 *
 * @param event
 *
 * @return
 **/
virtual bool handleMouseMove(MouseMoveEvent &event) override final
{
    size_t index = getIndexFromPosition(event.x, event.y);

    /**
     * @brief Write what the function does here
     *
     * @param lastMouseElement
     *
     * @return
     **/
    if(index != lastMouseElement)
    {

        /**
         * @brief Write what the function does here
         *
         * @param NoElement
         *
         * @return
         **/
        if(lastMouseElement != NoElement)
        {
            elements[lastMouseElement]->handleMouseMoveOut(event);
        }
        lastMouseElement = index;

        /**
         * @brief Write what the function does here
         *
         * @param NoElement
         *
         * @return
         **/
        if(index != NoElement)
        {
            elements[index]->handleMouseMoveIn(event);
        }
    }

    /**
     * @brief Write what the function does here
     *
     * @param NoElement
     *
     * @return
     **/
    if(index != NoElement)
    {
        return elements[index]->handleMouseMove(event);
    }
    return GUIElement::handleMouseMove(event);
}

/**
 * @brief Write what the function does here
 *
 * @param event
 *
 * @return
 **/
virtual bool handleMouseMoveOut(MouseEvent &event) override final
{

    /**
     * @brief Write what the function does here
     *
     * @param NoElement
     *
     * @return
     **/
    if(lastMouseElement != NoElement)
    {
        shared_ptr<GUIElement> e = elements[lastMouseElement];
        lastMouseElement = NoElement;
        return e->handleMouseMoveOut(event);
    }
    return GUIElement::handleMouseMoveOut(event);
}

/**
 * @brief Write what the function does here
 *
 * @param event
 *
 * @return
 **/
virtual bool handleMouseMoveIn(MouseEvent &event) override final
{
    size_t index = getIndexFromPosition(event.x, event.y);
    lastMouseElement = index;

    /**
     * @brief Write what the function does here
     *
     * @param NoElement
     *
     * @return
     **/
    if(index != NoElement)
    {
        return elements[index]->handleMouseMoveIn(event);
    }
    return GUIElement::handleMouseMoveIn(event);
}

/**
 * @brief Write what the function does here
 *
 * @param event
 *
 * @return
 **/
virtual bool handleMouseScroll(MouseScrollEvent &event) override final
{
    size_t index = getIndexFromPosition(event.x, event.y);

    /**
     * @brief Write what the function does here
     *
     * @param lastMouseElement
     *
     * @return
     **/
    if(index != lastMouseElement)
    {

        /**
         * @brief Write what the function does here
         *
         * @param NoElement
         *
         * @return
         **/
        if(lastMouseElement != NoElement)
        {
            elements[lastMouseElement]->handleMouseMoveOut(event);
        }
        lastMouseElement = index;

        /**
         * @brief Write what the function does here
         *
         * @param NoElement
         *
         * @return
         **/
        if(index != NoElement)
        {
            elements[index]->handleMouseMoveIn(event);
        }
    }

    /**
     * @brief Write what the function does here
     *
     * @param NoElement
     *
     * @return
     **/
    if(index != NoElement)
    {
        return elements[index]->handleMouseScroll(event);
    }
    return GUIElement::handleMouseScroll(event);
}

/**
 * @brief Write what the function does here
 *
 * @param event
 *
 * @return
 **/
virtual bool handleKeyUp(KeyUpEvent &event) override final
{
    shared_ptr<GUIElement> e = getFocusElement();
    if(e == shared_from_this())
        return GUIElement::handleKeyUp(event);
    return e->handleKeyUp(event);
}

/**
 * @brief Write what the function does here
 *
 * @param event
 *
 * @return
 **/
virtual bool handleKeyDown(KeyDownEvent &event) override final
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

    /**
     * @brief Write what the function does here
     *
     * @param KeyboardKey_Tab
     *
     * @return
     **/
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

    /**
     * @brief Write what the function does here
     *
     * @param KeyboardKey_Down
     *
     * @return
     **/
    else if(event.key == KeyboardKey_Down)
    {
        nextFocusElement();
        return true;
    }

    /**
     * @brief Write what the function does here
     *
     * @param KeyboardKey_Up
     *
     * @return
     **/
    else if(event.key == KeyboardKey_Up)
    {
        prevFocusElement();
        return true;
    }

    /**
     * @brief Write what the function does here
     *
     * @param KeyboardKey_Home
     *
     * @return
     **/
    else if(event.key == KeyboardKey_Home)
    {
        firstFocusElement();
        return true;
    }

    /**
     * @brief Write what the function does here
     *
     * @param KeyboardKey_End
     *
     * @return
     **/
    else if(event.key == KeyboardKey_End)
    {
        lastFocusElement();
        return true;
    }
    return false;
}

/**
 * @brief Write what the function does here
 *
 * @param event
 *
 * @return
 **/
virtual bool handleKeyPress(KeyPressEvent &event) override final
{
    shared_ptr<GUIElement> e = getFocusElement();
    if(e == shared_from_this())
        return GUIElement::handleKeyPress(event);
    return e->handleKeyPress(event);
}

/**
 * @brief Write what the function does here
 *
 * @param event
 *
 * @return
 **/
virtual bool handleQuit(QuitEvent &event) override final
{
    shared_ptr<GUIElement> e = getFocusElement();
    if(e == shared_from_this())
        return GUIElement::handleQuit(event);
    return e->handleQuit(event);
}

/**
 * @brief Write what the function does here
 *
 * @param minZ
 * @param maxZ
 * @param hasFocus
 *
 * @return
 **/
virtual Mesh render(float minZ, float maxZ, bool hasFocus) override
{
    Mesh retval = nullptr;

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
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

/**
 * @brief Write what the function does here
 *
 * @param e
 **/
void setFocus(shared_ptr<GUIElement> e)
{
    if(e == nullptr)
        return;

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    for(size_t i = 0; i < elements.size(); i++)
    {

        /**
         * @brief Write what the function does here
         *
         * @param e
         *
         * @return
         **/
        if(elements[i] == e)
        {

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            if(e->canHaveKeyboardFocus())
            {
                currentFocusIndex = i;

                /**
                 * @brief Write what the function does here
                 *
                 * @param nullptr
                 *
                 * @return
                 **/
                if(getParent() != nullptr)
                {
                    getParent()->setFocus(shared_from_this());
                }
                onSetFocus();
            }
        }
    }
}

/**
 * @brief Write what the function does here
 *
 virtual void reset() override
 {

/**
 * @brief Write what the function does here
 *
 * @param nullptr
 *
 * @return
 **/
if(getParent() == nullptr)
{
    minX = -Display::scaleX();
    maxX = Display::scaleX();
    minY = -Display::scaleY();
    maxY = Display::scaleY();
}
for(shared_ptr<GUIElement> e : elements)
    e->reset();
    firstFocusElement();
    }

/**
 * @brief Write what the function does here
 *
 * @return
 **/
virtual shared_ptr<GUIContainer> getTopLevelParent() override final
{
    shared_ptr<GUIContainer> retval = dynamic_pointer_cast<GUIContainer>(shared_from_this());
    while(retval->getParent() != nullptr)
        retval = retval->getParent();
    return retval;
}
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
inline shared_ptr<GUIContainer> GUIElement::getTopLevelParent() // this is here so that GUIContainer is defined
{
    shared_ptr<GUIContainer> retval = getParent();
    if(retval == nullptr)
        return retval;
    while(retval->getParent() != nullptr)
        retval = retval->getParent();
    return retval;
}
#endif // GUICONTAINER_H_INCLUDED
