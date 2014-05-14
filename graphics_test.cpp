#include "platform.h"
#include "generate.h"
#include "text.h"
#include "texture_atlas.h"
#include <cwchar>
#include <sstream>
#include <iostream>

TransformedMesh drawAtTopLeft(wstring str, Color color, float heightFraction = 1.0f / 25.0f)
{
    float width = Text::width(str);
    float height = Text::height(str);
    return transform(Matrix::translate(-Display::scaleX() / heightFraction, Display::scaleY() / heightFraction - height, -1.0f / heightFraction), Text::mesh(str, color));
}

Mesh drawButton(bool pressed, wstring str, float minX, float maxX, float minY, float maxY, Color buttonColor, Color textColor = Color(0))
{
    float textWidth = Text::width(str);
    float textHeight = Text::height(str);
    float leftToMiddleX = minX + 0.5 * (maxY - minY);
    float middleToRightX = maxX - 0.5 * (maxY - minY);
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
    if(pressed)
    {
        float midX = (minX + maxX) * 0.5, midY = (minY + maxY) * 0.5;
        buttonTransform = Matrix::translate(-midX, -midY, 0).concat(Matrix::rotateZ(M_PI)).concat(Matrix::translate(midX, midY, 0));
    }
    Mesh retval = (Mesh)transform(buttonTransform, diffuse);
    if(textHeight > 0 && textWidth > 0)
    {
        float textScale = 0.5 * (maxY - minY) / textHeight;
        textScale = min(textScale, (middleToRightX - leftToMiddleX) / textWidth);
        if(pressed)
            textScale *= 0.9;
        float xOffset = -0.5 * textWidth, yOffset = -0.5 * textHeight;
        xOffset = textScale * xOffset + 0.5 * (minX + maxX);
        yOffset = textScale * yOffset + 0.5 * (minY + maxY);
        retval->add(transform(Matrix::scale(textScale).concat(Matrix::translate(xOffset, yOffset, -1).concat(Matrix::scale(0.5))), Text::mesh(str, textColor)));
    }
    retval->add(transform(buttonTransform.concat(Matrix::scale(0.75)), specular));
    return retval;
}

struct TheEventHandler final : public EventHandler
{
    bool mouseDown = false;
    double lastDownTime = -1;
    virtual bool handleMouseUp(MouseUpEvent &event)
    {
        if(event.button == MouseButton_Left)
            mouseDown = false;
        return true;
    }
    virtual bool handleMouseDown(MouseDownEvent &event)
    {
        if(event.button == MouseButton_Left)
        {
            mouseDown = true;
            lastDownTime = Display::timer();
        }
        return true;
    }
    virtual bool handleMouseMove(MouseMoveEvent &event)
    {
        return true;
    }
    virtual bool handleMouseScroll(MouseScrollEvent &event)
    {
        return false; // so it will run the default action
    }
    virtual bool handleKeyUp(KeyUpEvent &event)
    {
        return false; // so it will run the default action
    }
    virtual bool handleKeyDown(KeyDownEvent &event)
    {
        return false; // so it will run the default action
    }
    virtual bool handleKeyPress(KeyPressEvent &event)
    {
        return false; // so it will run the default action
    }
    virtual bool handleQuit(QuitEvent &event)
    {
        return false; // so it will run the default action
    }
};

int main()
{
    startGraphics();
    Renderer renderer;
    Image background(L"background.png");
    TheEventHandler eventHandler;
    while(true)
    {
        Display::handleEvents(shared_ptr<EventHandler>(&eventHandler, [](const EventHandler *){/*do nothing on delete*/}));
        Display::initFrame();
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TextureDescriptor td = TextureDescriptor(background, 0, Display::width() / background.width(), 0, Display::height() / background.height());
        Mesh backgroundmesh=Generate::quadrilateral(td, VectorF(-Display::scaleX(), -Display::scaleY(), -1), Color(1), VectorF(Display::scaleX(), -Display::scaleY(), -1), Color(1), VectorF(Display::scaleX(), Display::scaleY(), -1), Color(1), VectorF(-Display::scaleX(), Display::scaleY(), -1), Color(1));
        renderer<<backgroundmesh;
        Display::initOverlay();
        wostringstream os;
        os << L"FPS : " << Display::averageFPS();
        wstring str = os.str();
        Mesh m = Text::mesh(str, Color(0.5, 0, 1));
        m->add(invert(m));
        Mesh world = Mesh(new Mesh_t()); // create a new empty mesh
        world->add(drawAtTopLeft(L"Hello world", Color(1, 0, 1)));
        world->add(transform(Matrix::rotateY(Display::timer()).concat(Matrix::translate(0, 0, -20)), m));
        renderer << world;
        Display::initOverlay();
        renderer << drawButton(eventHandler.mouseDown || (Display::timer() - eventHandler.lastDownTime < 0.1), L"Button", -0.4, 0.4, 0.9, 1, Color::HSV(fmod(Display::timer() / 4, 1), 1, 1));
        Display::flip(60);
    }
}
