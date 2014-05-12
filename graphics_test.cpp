#include "platform.h"
#include "generate.h"
#include "text.h"
#include <cwchar>
#include <sstream>
#include <iostream>

TransformedMesh drawAtTopLeft(wstring str, Color color, float heightFraction = 1.0f / 25.0f)
{
    float width = Text::width(str);
    float height = Text::height(str);
    return transform(Matrix::translate(-Display::scaleX() / heightFraction, Display::scaleY() / heightFraction - height, -1.0f / heightFraction), Text::mesh(str, color));
}

int main()
{
    startGraphics();
    Renderer renderer;
    while(true)
    {
        Display::handleEvents(nullptr);
        Display::initFrame();
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        Display::flip(60);
    }
}
