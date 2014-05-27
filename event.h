#include "platform.h"
#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED
#include <memory>
using namespace std;
class KeyDownEvent;
class MouseUpEvent;
class MouseDownEvent;
class MouseMoveEvent;
class MouseScrollEvent;
class KeyUpEvent;
class KeyPressEvent;
class QuitEvent;

/**
 * @brief Write what the function does here
 *
 * @return
 **/
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

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class Event
{
    public:

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
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

            /**
             * @brief Write what the function does here
             *
             * @param type
             *
             * @return
             **/
            : type(type)
            {
            }
    public:
        virtual bool dispatch(shared_ptr<EventHandler> eventHandler) = 0;
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class MouseEvent : public Event
{
    public:
        const float x, y;
        const float deltaX, deltaY;
    protected:
        MouseEvent(Type type, float x, float y, float deltaX, float deltaY)

            /**
             * @brief Write what the function does here
             *
             * @param type
             * @param x
             * @param y
             * @param deltaX
             * @param deltaY
             *
             * @return
             **/
            : Event(type), x(x), y(y), deltaX(deltaX), deltaY(deltaY)
            {
            }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class KeyEvent : public Event
{
    public:
        const KeyboardKey key;
        const KeyboardModifiers mods;
    protected:

        /**
         * @brief Write what the function does here
         *
         * @param type
         * @param key
         * @param mods
         * @param type
         * @param key
         * @param mods
         *
         * @return
         **/
        KeyEvent(Type type, KeyboardKey key, KeyboardModifiers mods): Event(type), key(key), mods(mods)
    {
    }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class KeyDownEvent final : public KeyEvent
{
    public:
        const bool isRepetition;

        /**
         * @brief Write what the function does here
         *
         * @param key
         * @param mods
         * @param false
         * @param Type_KeyDown
         * @param key
         * @param mods
         * @param isRepetition
         *
         * @return
         **/
        KeyDownEvent(KeyboardKey key, KeyboardModifiers mods, bool isRepetition = false) : KeyEvent(Type_KeyDown, key, mods), isRepetition(isRepetition)
    {
    }

        /**
         * @brief Write what the function does here
         *
         * @param eventHandler
         *
         * @return
         **/
        virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
        {
            return eventHandler->handleKeyDown(*this);
        }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class KeyUpEvent : public KeyEvent
{
    public:
        KeyUpEvent(KeyboardKey key, KeyboardModifiers mods)

            /**
             * @brief Write what the function does here
             *
             * @param Type_KeyUp
             * @param key
             * @param mods
             *
             * @return
             **/
            : KeyEvent(Type_KeyUp, key, mods)
            {
            }

        /**
         * @brief Write what the function does here
         *
         * @param eventHandler
         *
         * @return
         **/
        virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
        {
            return eventHandler->handleKeyUp(*this);
        }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct KeyPressEvent : public Event
{
    const wchar_t character;
    KeyPressEvent(wchar_t character)

        /**
         * @brief Write what the function does here
         *
         * @param Type_KeyPress
         * @param character
         *
         * @return
         **/
        : Event(Type_KeyPress), character(character)
        {
        }

    /**
     * @brief Write what the function does here
     *
     * @param eventHandler
     *
     * @return
     **/
    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleKeyPress(*this);
    }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct MouseButtonEvent : public MouseEvent
{
    const MouseButton button;
    protected:
    MouseButtonEvent(Type type, float x, float y, float deltaX, float deltaY, MouseButton button)

        /**
         * @brief Write what the function does here
         *
         * @param type
         * @param x
         * @param y
         * @param deltaX
         * @param deltaY
         * @param button
         *
         * @return
         **/
        : MouseEvent(type, x, y, deltaX, deltaY), button(button)
        {
        }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct MouseUpEvent : public MouseButtonEvent
{

    /**
     * @brief Write what the function does here
     *
     * @param x
     * @param y
     * @param deltaX
     * @param deltaY
     * @param button
     * @param Type_MouseUp
     * @param x
     * @param y
     * @param deltaX
     * @param deltaY
     * @param button
     *
     * @return
     **/
    MouseUpEvent(float x, float y, float deltaX, float deltaY, MouseButton button) : MouseButtonEvent(Type_MouseUp, x, y, deltaX, deltaY, button)
    {
    }

    /**
     * @brief Write what the function does here
     *
     * @param eventHandler
     *
     * @return
     **/
    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleMouseUp(*this);
    }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct MouseDownEvent : public MouseButtonEvent
{
    MouseDownEvent(float x, float y, float deltaX, float deltaY, MouseButton button)

        /**
         * @brief Write what the function does here
         *
         * @param Type_MouseDown
         * @param x
         * @param y
         * @param deltaX
         * @param deltaY
         * @param button
         *
         * @return
         **/
        : MouseButtonEvent(Type_MouseDown, x, y, deltaX, deltaY, button)
        {
        }

    /**
     * @brief Write what the function does here
     *
     * @param eventHandler
     *
     * @return
     **/
    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleMouseDown(*this);
    }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct MouseMoveEvent : public MouseEvent
{
    MouseMoveEvent(float x, float y, float deltaX, float deltaY)

        /**
         * @brief Write what the function does here
         *
         * @param Type_MouseMove
         * @param x
         * @param y
         * @param deltaX
         * @param deltaY
         *
         * @return
         **/
        : MouseEvent(Type_MouseMove, x, y, deltaX, deltaY)
        {
        }

    /**
     * @brief Write what the function does here
     *
     * @param eventHandler
     *
     * @return
     **/
    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleMouseMove(*this);
    }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct MouseScrollEvent : public MouseEvent
{
    const int scrollX, scrollY;
    MouseScrollEvent(float x, float y, float deltaX, float deltaY, int scrollX, int scrollY)

        /**
         * @brief Write what the function does here
         *
         * @param Type_MouseScroll
         * @param x
         * @param y
         * @param deltaX
         * @param deltaY
         * @param scrollX
         * @param scrollY
         *
         * @return
         **/
        : MouseEvent(Type_MouseScroll, x, y, deltaX, deltaY), scrollX(scrollX), scrollY(scrollY)
        {
        }

    /**
     * @brief Write what the function does here
     *
     * @param eventHandler
     *
     * @return
     **/
    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleMouseScroll(*this);
    }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct QuitEvent : public Event
{
    QuitEvent()

        /**
         * @brief Write what the function does here
         *
         * @param Type_Quit
         *
         * @return
         **/
        : Event(Type_Quit)
        {
        }

    /**
     * @brief Write what the function does here
     *
     * @param eventHandler
     *
     * @return
     **/
    virtual bool dispatch(shared_ptr<EventHandler> eventHandler) override
    {
        return eventHandler->handleQuit(*this);
    }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class CombinedEventHandler final : public EventHandler
{
    private:
        shared_ptr<EventHandler> first, second;
    public:
        CombinedEventHandler(shared_ptr<EventHandler> first, shared_ptr<EventHandler> second)

            /**
             * @brief Write what the function does here
             *
             * @param first
             * @param second
             *
             * @return
             **/
            : first(first), second(second)
            {
            }

        /**
         * @brief Write what the function does here
         *
         * @param event
         *
         * @return
         **/
        virtual bool handleMouseUp(MouseUpEvent &event) override
        {

            /**
             * @brief Write what the function does here
             *
             * @param event
             *
             * @return
             **/
            if(first->handleMouseUp(event))
            {
                return true;
            }
            return second->handleMouseUp(event);
        }

        /**
         * @brief Write what the function does here
         *
         * @param event
         *
         * @return
         **/
        virtual bool handleMouseDown(MouseDownEvent &event) override
        {

            /**
             * @brief Write what the function does here
             *
             * @param event
             *
             * @return
             **/
            if(first->handleMouseDown(event))
            {
                return true;
            }
            return second->handleMouseDown(event);
        }

        /**
         * @brief Write what the function does here
         *
         * @param event
         *
         * @return
         **/
        virtual bool handleMouseMove(MouseMoveEvent &event) override
        {

            /**
             * @brief Write what the function does here
             *
             * @param event
             *
             * @return
             **/
            if(first->handleMouseMove(event))
            {
                return true;
            }
            return second->handleMouseMove(event);
        }

        /**
         * @brief Write what the function does here
         *
         * @param event
         *
         * @return
         **/
        virtual bool handleMouseScroll(MouseScrollEvent &event)override
        {

            /**
             * @brief Write what the function does here
             *
             * @param event
             *
             * @return
             **/
            if(first->handleMouseScroll(event))
            {
                return true;
            }
            return second->handleMouseScroll(event);
        }

        /**
         * @brief Write what the function does here
         *
         * @param event
         *
         * @return
         **/
        virtual bool handleKeyUp(KeyUpEvent &event)override
        {

            /**
             * @brief Write what the function does here
             *
             * @param event
             *
             * @return
             **/
            if(first->handleKeyUp(event))
            {
                return true;
            }
            return second->handleKeyUp(event);
        }

        /**
         * @brief Write what the function does here
         *
         * @param event
         *
         * @return
         **/
        virtual bool handleKeyDown(KeyDownEvent &event)override
        {

            /**
             * @brief Write what the function does here
             *
             * @param event
             *
             * @return
             **/
            if(first->handleKeyDown(event))
            {
                return true;
            }
            return second->handleKeyDown(event);
        }

        /**
         * @brief Write what the function does here
         *
         * @param event
         *
         * @return
         **/
        virtual bool handleKeyPress(KeyPressEvent &event)override
        {

            /**
             * @brief Write what the function does here
             *
             * @param event
             *
             * @return
             **/
            if(first->handleKeyPress(event))
            {
                return true;
            }
            return second->handleKeyPress(event);
        }

        /**
         * @brief Write what the function does here
         *
         * @param event
         *
         * @return
         **/
        virtual bool handleQuit(QuitEvent &event)override
        {

            /**
             * @brief Write what the function does here
             *
             * @param event
             *
             * @return
             **/
            if(first->handleQuit(event))
            {
                return true;
            }
            return second->handleQuit(event);
        }
};
#endif // EVENT_H_INCLUDED
