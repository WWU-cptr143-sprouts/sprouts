#include "platform.h"
#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include <memory>

using namespace std;

/*
This code basically handles any possible I/O events from the user, including keyboard and mouse
movement/keys and special circumstances like quitting the game
*/

class KeyDownEvent;
class MouseUpEvent;
class MouseDownEvent;
class MouseMoveEvent;
class MouseScrollEvent;
class KeyUpEvent;
class KeyPressEvent;
class QuitEvent;

//Create a struct to handle our events, with functions we can define down the line
struct EventHandler : public enable_shared_from_this<EventHandler>
{
    virtual bool handleMouseUp(MouseUpEvent &event) = 0;
    virtual bool handleMouseDown(MouseDownEvent &event) = 0;
    virtual bool handleMouseMove(MouseMoveEvent &event) = 0;
    virtual bool handleMouseScroll(MouseScrollEvent &event) = 0;
    virtual bool handleKeyUp(KeyUpEvent &event) = 0;
    virtual bool handleKeyDown(KeyDownEvent &event) = 0;
    virtual bool handleKeyPress(KeyPressEvent &event) = 0;
    virtual bool handleQuit(QuitEvent &event) = 0;
};

//Create an event class to handle events.
//It has different types enumerated to decide which event happened
//Also has a function to dispatch an event
class Event
{
public:
    enum Type
    {
        Type_MouseUp,
        Type_MouseDown,
        Type_MouseMove,
        Type_MouseScroll,
        Type_KeyUp,
        Type_KeyDown,
        Type_KeyPress,
        Type_Quit,
    };
    const Type type;
protected:
    Event(Type type)
        : type(type)
    {
    }
public:
    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) = 0;
};

//Class for mouse events, child of Event with extra variables a mouse needs
//Such as current position and change in position (which gives velocity)
class MouseEvent : public Event
{
public:
    const float x, y;
    const float deltaX, deltaY;
protected:
    MouseEvent(Type type, float x, float y, float deltaX, float deltaY)
        : Event(type), x(x), y(y), deltaX(deltaX), deltaY(deltaY)
    {
    }
};

//Class for keyboard events, child of Event with extra variables for
//Both the keys on the keyboard and modifiers such as shift/etc
class KeyEvent : public Event
{
public:
    const KeyboardKey key;
    const KeyboardModifiers mods;
protected:
    KeyEvent(Type type, KeyboardKey key, KeyboardModifiers mods): Event(type), key(key), mods(mods)
    {
    }
};

//Child of KeyEvent to handle explicitly when a key is pressed down. 
//Has a bool to determine if the key is held or repeated quickly
class KeyDownEvent final : public KeyEvent
{
public:
    const bool isRepetition;
    KeyDownEvent(KeyboardKey key, KeyboardModifiers mods, bool isRepetition = false) : KeyEvent(Type_KeyDown, key, mods), isRepetition(isRepetition)
    {
    }
    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleKeyDown(*this);
    }
};

//Child of KeyEvent to handle explicitly when a key is released 
class KeyUpEvent : public KeyEvent
{
public:
    KeyUpEvent(KeyboardKey key, KeyboardModifiers mods)
        : KeyEvent(Type_KeyUp, key, mods)
    {
    }
    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleKeyUp(*this);
    }
};

//Child of KeyEvent to handle when a key is pressed
//The unique thing about this is it gets what character is pressed
//It applies modifiers such as shift to get a capital letter or 
//Symbol instead of a number.
struct KeyPressEvent : public Event
{
    const wchar_t character;
    KeyPressEvent(wchar_t character)
        : Event(Type_KeyPress), character(character)
    {
    }
    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleKeyPress(*this);
    }
};

//Child of MouseEvent to handle when a mouse button is pressed
//Has a variable for which button is pressed
struct MouseButtonEvent : public MouseEvent
{
    const MouseButton button;
protected:
    MouseButtonEvent(Type type, float x, float y, float deltaX, float deltaY, MouseButton button)
        : MouseEvent(type, x, y, deltaX, deltaY), button(button)
    {
    }
};

//Child of the MouseButtonEvent class to determine exactly when the mouse button is released
struct MouseUpEvent : public MouseButtonEvent
{
    MouseUpEvent(float x, float y, float deltaX, float deltaY, MouseButton button) : MouseButtonEvent(Type_MouseUp, x, y, deltaX, deltaY, button)
    {
    }

    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleMouseUp(*this);
    }
};

//Child of the MouseButtonEvent class to determine exactly when the mouse button is first pressed
struct MouseDownEvent : public MouseButtonEvent
{
    MouseDownEvent(float x, float y, float deltaX, float deltaY, MouseButton button)
        : MouseButtonEvent(Type_MouseDown, x, y, deltaX, deltaY, button)
    {
    }

    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleMouseDown(*this);
    }
};

//Child of MouseEvent to handle when the mouse moves
struct MouseMoveEvent : public MouseEvent
{
    MouseMoveEvent(float x, float y, float deltaX, float deltaY)
        : MouseEvent(Type_MouseMove, x, y, deltaX, deltaY)
    {
    }

    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleMouseMove(*this);
    }
};

//Child of MouseEvent to handle the scroll wheel 
struct MouseScrollEvent : public MouseEvent
{
    const int scrollX, scrollY;
    MouseScrollEvent(float x, float y, float deltaX, float deltaY, int scrollX, int scrollY)
        : MouseEvent(Type_MouseScroll, x, y, deltaX, deltaY), scrollX(scrollX), scrollY(scrollY)
    {
    }

    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleMouseScroll(*this);
    }
};

//Struct to handle quitting the game
struct QuitEvent : public Event
{
    QuitEvent()
        : Event(Type_Quit)
    {
    }
    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleQuit(*this);
    }
};

//This class uses the others to actually respond to any event necessary 
//The first-> calls make sure it's the first such event so events don't get called
//Repeatedly I believe
class CombinedEventHandler final : public EventHandler
{
private:
    shared_ptr<EventHandler> first, second;
public:
    CombinedEventHandler(shared_ptr<EventHandler> first, shared_ptr<EventHandler> second)
        : first(first), second(second)
    {
    }
    virtual bool handleMouseUp(MouseUpEvent &event) override
    {
        if(first->handleMouseUp(event))
        {
            return true;
        }

        return second->handleMouseUp(event);
    }
    virtual bool handleMouseDown(MouseDownEvent &event) override
    {
        if(first->handleMouseDown(event))
        {
            return true;
        }

        return second->handleMouseDown(event);
    }
    virtual bool handleMouseMove(MouseMoveEvent &event) override
    {
        if(first->handleMouseMove(event))
        {
            return true;
        }

        return second->handleMouseMove(event);
    }
    virtual bool handleMouseScroll(MouseScrollEvent &event)override
    {
        if(first->handleMouseScroll(event))
        {
            return true;
        }

        return second->handleMouseScroll(event);
    }
    virtual bool handleKeyUp(KeyUpEvent &event)override
    {
        if(first->handleKeyUp(event))
        {
            return true;
        }

        return second->handleKeyUp(event);
    }
    virtual bool handleKeyDown(KeyDownEvent &event)override
    {
        if(first->handleKeyDown(event))
        {
            return true;
        }

        return second->handleKeyDown(event);
    }
    virtual bool handleKeyPress(KeyPressEvent &event)override
    {
        if(first->handleKeyPress(event))
        {
            return true;
        }

        return second->handleKeyPress(event);
    }
    virtual bool handleQuit(QuitEvent &event)override
    {
        if(first->handleQuit(event))
        {
            return true;
        }

        return second->handleQuit(event);
    }
};

#endif // EVENT_H_INCLUDED
