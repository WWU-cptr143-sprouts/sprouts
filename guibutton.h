#ifndef GUIBUTTON_H_INCLUDED
#define GUIBUTTON_H_INCLUDED

#include "guielement.h"
#include "platform.h"
#include "text.h"
#include "texture_atlas.h"
#include "generate.h"
#include <functional>
#include <stdexcept>

using namespace std;

/*Create a struct we can use to place buttons, of type GUIElement of course
It has private variables for colors, default sizes/etc for the button itself
and then some functions to do things like see where it is, see if a point
is inside of it, and handle events.
*/
struct GUIButton : public GUIElement
{
    wstring title; 
    Color textColor;
    Color selectedTextColor;
    Color buttonColor;
    Color selectedButtonColor;
    GUIButton(function<void()> pressEvent, wstring title, float minX, float maxX, float minY,
              float maxY, Color buttonColor = Color::V(0.5), Color textColor = Color::V(0),
              Color selectedButtonColor = Color::RGB(185 / 255.0, 12 / 255.0, 12 / 255.0), Color selectedTextColor = Color::V(0))
        : GUIElement(minX, maxX, minY, maxY), title(title), textColor(textColor),
          selectedTextColor(selectedTextColor), buttonColor(buttonColor),
          selectedButtonColor(selectedButtonColor), buttonPressEvent(pressEvent)
    {
    }
protected:
    GUIButton(wstring title, float minX, float maxX, float minY, float maxY,
              Color buttonColor = Color::V(0.75), Color textColor = Color::V(0),
              Color selectedButtonColor = Color::V(0.75), Color selectedTextColor = Color::RGB(0, 0, 1))
        : GUIElement(minX, maxX, minY, maxY), title(title), textColor(textColor),
          selectedTextColor(selectedTextColor), buttonColor(buttonColor),
          selectedButtonColor(selectedButtonColor), buttonPressEvent([]()
    {
        //Basically this function should be overwritten to actually function properly, so if this is called
        //and runs through to here that's a problem -- throw an error
        throw logic_error("button press event function called instead of overridden member");
    })
    {
    }
    //The following are some private variables the public functions use to determine things 
    //like is pointinside etc, pretty self-explanatory
private:
    float getLeftToMiddleX() const
    {
        return minX + 0.5 * (maxY - minY);
    }
    float getMiddleToRightX() const
    {
        return maxX - 0.5 * (maxY - minY);
    }
    
    //This is where things get interesting, here the button handles things like mouse events
    //And determines if a point is inside it
public:
    //This takes in an x,y coordinate and determines if it's inside the button. 
    //It uses the private functions and some relatively simple logic to do so.
    virtual bool isPointInside(float x, float y) const override
    {
        //I believe this checks to see if it's in the element at all, which
        //Is a larger area than this particular button
        if(!GUIElement::isPointInside(x, y))
        {
            return false;
        }

        float leftToMiddleX = getLeftToMiddleX();
        float middleToRightX = getMiddleToRightX();

        if(x > leftToMiddleX)
        {
            x -= middleToRightX;

            if(x < 0)
            {
                return true;
            }
        }
        else
        {
            x -= leftToMiddleX;
        }

        //Get the vertical midpoint of the button
        float halfHeight = 0.5 * (maxY - minY);
        y -= 0.5 * (minY + maxY);

        //Basically if x^2 + y^2 < halfHeight^2 then it has to be inside the button
        if(x * x + y * y < halfHeight * halfHeight)
        {
            return true;
        }

        return false;
    }
    //Check to see if this element can have the keyboard's focus
    //I'm guessing this is for highlighting/etc and determining if
    //You'll skip this element when using the keyboard/etc
    virtual bool canHaveKeyboardFocus() const override
    {
        return true;
    }
    //Allows the button to change variables as needed when the mouse is released
    virtual bool handleMouseUp(MouseUpEvent &event)
    {
        if(event.button == MouseButton_Left)
        {
            isMousePressed = false;
            return true;
        }

        return GUIElement::handleMouseUp(event);
    }
    //Run anything needed when the button is clicked, calls other button functions
    virtual bool handleMouseDown(MouseDownEvent &event) override
    {
        if(event.button == MouseButton_Left)
        {
            isMousePressed = true;
            lastDownTime = Display::timer();
            setFocus();
            handleButtonPress();
            return true;
        }

        return GUIElement::handleMouseDown(event);
    }
    //If the button is sensitive to highlighting with mouse you can change things here
    virtual bool handleMouseMoveOut(MouseEvent &event) override
    {
        isMousePressed = false; //Just to be safe really
        return true;
    }
    //If the button has a key assigned then this handles that, send to the same function as a mouse click
    virtual bool handleKeyDown(KeyDownEvent &event) override
    {
        if(!event.isRepetition && (event.key == KeyboardKey_Return || event.key == KeyboardKey_Space))
        {
            lastDownTime = Display::timer();
            handleButtonPress();
            return true;
        }

        return GUIElement::handleKeyDown(event);
    }
protected:
    //Simple function that allows children to see if the button is pressed
    bool isPressed() const
    {
        return isMousePressed || (Display::timer() - lastDownTime < 0.1);
    }
    //a virtual void you can add stuff to in child classes, just redirects right now
    virtual void handleButtonPress()
    {
        buttonPressEvent();
    }
    //Actually render the button, changes color if the button has focus
    virtual Mesh render(float minZ, float maxZ, bool hasFocus) override
    {
        //Choose the colors of the button and text
        Color buttonColor = this->buttonColor;
        Color textColor = this->textColor;

        //If we have the focus, then highlight the button
        if(hasFocus)
        {
            buttonColor = this->selectedButtonColor;
            textColor = this->selectedTextColor;
        }

        //Set some variables for displaying
        bool pressed = isPressed();
        float textWidth = Text::width(title);
        float textHeight = Text::height(title);
        float leftToMiddleX = getLeftToMiddleX();
        float middleToRightX = getMiddleToRightX();
        //Create the button by layering colored meshes on top of each other
        Mesh diffuse = Generate::quadrilateral(TextureAtlas::ButtonLeftDiffuse.td(),
                                               VectorF(minX, minY, -1), buttonColor,
                                               VectorF(leftToMiddleX, minY, -1), buttonColor,
                                               VectorF(leftToMiddleX, maxY, -1), buttonColor,
                                               VectorF(minX, maxY, -1), buttonColor);
        diffuse->add(Generate::quadrilateral(TextureAtlas::ButtonMiddleDiffuse.td(),
                                             VectorF(leftToMiddleX, minY, -1), buttonColor,
                                             VectorF(middleToRightX, minY, -1), buttonColor,
                                             VectorF(middleToRightX, maxY, -1), buttonColor,
                                             VectorF(leftToMiddleX, maxY, -1), buttonColor));
        diffuse->add(Generate::quadrilateral(TextureAtlas::ButtonRightDiffuse.td(),
                                             VectorF(middleToRightX, minY, -1), buttonColor,
                                             VectorF(maxX, minY, -1), buttonColor,
                                             VectorF(maxX, maxY, -1), buttonColor,
                                             VectorF(middleToRightX, maxY, -1), buttonColor));
        Mesh specular = Generate::quadrilateral(TextureAtlas::ButtonLeftSpecular.td(),
                                                VectorF(minX, minY, -1), Color(1),
                                                VectorF(leftToMiddleX, minY, -1), Color(1),
                                                VectorF(leftToMiddleX, maxY, -1), Color(1),
                                                VectorF(minX, maxY, -1), Color(1));
        specular->add(Generate::quadrilateral(TextureAtlas::ButtonMiddleSpecular.td(),
                                              VectorF(leftToMiddleX, minY, -1), Color(1),
                                              VectorF(middleToRightX, minY, -1), Color(1),
                                              VectorF(middleToRightX, maxY, -1), Color(1),
                                              VectorF(leftToMiddleX, maxY, -1), Color(1)));
        specular->add(Generate::quadrilateral(TextureAtlas::ButtonRightSpecular.td(),
                                              VectorF(middleToRightX, minY, -1), Color(1),
                                              VectorF(maxX, minY, -1), Color(1),
                                              VectorF(maxX, maxY, -1), Color(1),
                                              VectorF(middleToRightX, maxY, -1), Color(1)));
        Matrix buttonTransform = Matrix::identity();
        //If the button is pressed, move it just a little bit to show that it's pressed
        if(pressed)
        {
            float midX = (minX + maxX) * 0.5, midY = (minY + maxY) * 0.5;
            buttonTransform = Matrix::translate(-midX, -midY,
                                                0).concat(Matrix::rotateZ(M_PI)).concat(Matrix::translate(midX, midY, 0));
        }

        //Return the mesh of the transformed button/created
        Mesh retval = (Mesh)transform(buttonTransform.concat(Matrix::scale(interpolate(2.0 / 3, minZ,
                                      maxZ))), diffuse);

        //If we have text, scaled it properly and change things depending on if it's pressed or not
        if(textHeight > 0 && textWidth > 0)
        {
            float textScale = 0.5 * (maxY - minY) / textHeight;
            textScale = min(textScale, (middleToRightX - leftToMiddleX) / textWidth);

            if(pressed)
            {
                textScale *= 0.9;
            }

            float xOffset = -0.5 * textWidth, yOffset = -0.5 * textHeight;
            xOffset = textScale * xOffset + 0.5 * (minX + maxX);
            yOffset = textScale * yOffset + 0.5 * (minY + maxY);
            retval->add(transform(Matrix::scale(textScale).concat(Matrix::translate(xOffset, yOffset,
                                  -1).concat(Matrix::scale(minZ))), Text::mesh(title, textColor)));
        }
        //Add the specular mesh to our return value
        retval->add(transform(buttonTransform.concat(Matrix::scale(interpolate(1.0 / 3, minZ, maxZ))),
                              specular));
        return retval;
    }
    //Reset the state of the button, where it hasn't been pressed yet
    virtual void reset()
    {
        isMousePressed = false;
        lastDownTime = -1;
    }
    //Some more basic private variables to use for the button state
private:
    bool isMousePressed = false;
    double lastDownTime = -1;
    function<void()> buttonPressEvent;
};

#endif // GUIBUTTON_H_INCLUDED
