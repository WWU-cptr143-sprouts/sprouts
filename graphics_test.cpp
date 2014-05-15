#include "platform.h"
#include "guibutton.h"
#include "guilabel.h"
#include <cwchar>
#include <sstream>
#include <iostream>
#include <iomanip>

int main()
{
    startGraphics();
    Renderer renderer;
    Image background(L"background.png");
    Display::initFrame();
    vector<function<void()>> needRunFunctions;
    shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(), -Display::scaleY(), Display::scaleY());
    shared_ptr<GUILabel> lastButtonLabel = make_shared<GUILabel>(L"", -0.5, 0.5, -0.15, -0.05);
    gui->add(make_shared<GUIButton>([lastButtonLabel](){lastButtonLabel->text = L"button 1 pressed"; lastButtonLabel->textColor = Color::RGB(1, 0, 0);}, L"Button 1", -0.4, 0.4, 0.9, 1));
    gui->add(make_shared<GUIButton>([lastButtonLabel](){lastButtonLabel->text = L"button 2 pressed"; lastButtonLabel->textColor = Color::RGB(1, 1, 0);}, L"Button 2", -0.4, 0.4, 0.7, 0.8));
    shared_ptr<GUILabel> fpsLabel = make_shared<GUILabel>(L"", -0.5, 0.5, -0.05, 0.05);
    gui->add(make_shared<GUIContainer>(-0.5, 0.5, -0.4, 0.1)->add(fpsLabel)->add(lastButtonLabel));
    gui->add(make_shared<GUIButton>([&needRunFunctions, &gui](){needRunFunctions.push_back([&gui](){endGraphics(); setPreferredMode(640, 480); startGraphics(); Display::initFrame(); gui->reset();});}, L"Set 640x480 graphics", -0.4, 0.4, 0.3, 0.4));
    gui->add(make_shared<GUIButton>([&needRunFunctions, &gui](){needRunFunctions.push_back([&gui](){endGraphics(); setPreferredMode(1024, 768); startGraphics(); Display::initFrame(); gui->reset();});}, L"Set 1024x768 graphics", -0.4, 0.4, -0.4, -0.3));
    while(true)
    {
        Display::initFrame();
        gui->minX = -Display::scaleX();
        gui->maxX = Display::scaleX();
        gui->minY = -Display::scaleY();
        gui->maxY = Display::scaleY();
        Display::handleEvents(gui);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TextureDescriptor td = TextureDescriptor(background, 0, Display::width() / background.width(), 0, Display::height() / background.height());
        Mesh backgroundmesh=Generate::quadrilateral(td, VectorF(-Display::scaleX(), -Display::scaleY(), -1), Color(1), VectorF(Display::scaleX(), -Display::scaleY(), -1), Color(1), VectorF(Display::scaleX(), Display::scaleY(), -1), Color(1), VectorF(-Display::scaleX(), Display::scaleY(), -1), Color(1));
        renderer<<backgroundmesh;
        Display::initOverlay();
        wostringstream os;
        os << L"FPS : " << setprecision(4) << setw(5) << setfill(L'0') << left << Display::averageFPS();
        fpsLabel->text = os.str();
        renderer << gui->render();
        Display::flip(60);
        vector<function<void()>> fns = std::move(needRunFunctions);
        needRunFunctions.clear();
        for(function<void()> fn : fns)
        {
            fn();
        }
    }
}
