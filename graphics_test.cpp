#include "platform.h"
#include "gui.h"
#include "text.h"
#include <cwchar>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <random>

using namespace std;

namespace
{
class FPSLabel : public GUILabel
{
public:
    FPSLabel(float minX, float maxX, float minY, float maxY)
        : GUILabel(L"", minX, maxX, minY, maxY)
    {
    }
protected:
    virtual Mesh render(float minZ, float maxZ, bool hasFocus) override
    {
        wostringstream os;
        os << L"FPS : " << setprecision(4) << setw(5) << setfill(L'0') << left << Display::averageFPS();
        text = os.str();
        return GUILabel::render(minZ, maxZ, hasFocus);
    }
};

bool runSubmenu()
{
    shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(), -Display::scaleY(), Display::scaleY());
    wstring str = L"This is\nsoo neat.";
    Mesh mesh = Text::mesh(str, Color::V(1));
    mesh->add(invert(mesh));
    float width = Text::width(str);
    float height = Text::height(str);
    mesh = (Mesh)transform(Matrix::translate(-width / 2, -height / 2, 0).concat(Matrix::scale(2 / width)).concat(Matrix::translate(0, 0, -1)), mesh);
    shared_ptr<GUICircleArrangement> circleArrangement = make_shared<GUICircleArrangement>(-1, 1, -1, 1, -M_PI / 3, M_PI / 3, VectorF(-0.7, 0, 0), 0.8, mesh, VectorF(0, 0.5, 0));
    circleArrangement->add(make_shared<GUIButton>([]()
    {
    }, L"Button 1", -0.4, 0.4, -0.8, -0.7, Color::RGB(1, 0, 0), Color::V(0), Color::RGB(1,
            0, 1)));
    circleArrangement->add(make_shared<GUIButton>([]()
    {
    }, L"Button 2", -0.4, 0.4, -0.8, -0.7, Color::RGB(1, 0, 0), Color::V(0), Color::RGB(1,
            0, 1)));
    circleArrangement->add(make_shared<GUIButton>([]()
    {
    }, L"Button 3", -0.4, 0.4, -0.8, -0.7, Color::RGB(1, 0, 0), Color::V(0), Color::RGB(1,
            0, 1)));
    circleArrangement->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->quit();
    }, L"Quit", -0.4, 0.4, -0.8, -0.7, Color::RGB(1, 0, 0), Color::V(0), Color::RGB(1,
            0, 1)));
    gui->add(circleArrangement);
    return runAsDialog(gui);
}
}

int main()
{
    startGraphics();
    Display::initFrame();
    vector<function<void()>> needRunFunctions;
    shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(),
                                   -Display::scaleY(), Display::scaleY());
    shared_ptr<GUILabel> lastButtonLabel = make_shared<GUILabel>(L"", -0.5, 0.5, -0.35, -0.05);
    gui->add(make_shared<GUIButton>([lastButtonLabel]()
    {
        lastButtonLabel->text = L"button 1 pressed √π ≤ ½·e²";
        lastButtonLabel->textColor = Color::RGB(1, 0, 0);
    }, L"Button 1", -0.4, 0.4, 0.9, 1));
    gui->add(make_shared<GUIButton>([lastButtonLabel]()
    {
        lastButtonLabel->text = L"π\n⌠\n│ sin(x) dx = 2\n⌡\n0";
        lastButtonLabel->textColor = Color::RGB(1, 1, 0);
    }, L"Button 1½", -0.4, 0.4, 0.8, 0.9));
    gui->add(make_shared<GUIButton>([lastButtonLabel]()
    {
        lastButtonLabel->text = L"button 2 pressed";
        lastButtonLabel->textColor = Color::RGB(1, 1, 0);
    }, L"Button 2", -0.4, 0.4, 0.7, 0.8));
    shared_ptr<GUILabel> fpsLabel = make_shared<FPSLabel>(-0.5, 0.5, -0.05, 0.05);
    gui->add(make_shared<GUIContainer>(-0.5, 0.5, -0.4, 0.1)->add(fpsLabel)->add(lastButtonLabel));
    gui->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->scheduleFunction([&gui]()
        {
            Display::setSize(640, 480);
            Display::initFrame();
            gui->reset();
        });
    }, L"Set 640x480 graphics", -0.4, 0.4, 0.3, 0.4));
    gui->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->scheduleFunction([&gui]()
        {
            Display::setSize(1024, 768);
            Display::initFrame();
            gui->reset();
        });
    }, L"Set 1024x768 graphics", -0.4, 0.4, -0.4, -0.3, Color::RGB(1, 0, 0), Color::V(0), Color::RGB(1,
            0, 1)));
    gui->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->scheduleFunction([&gui]()
        {
            Display::fullScreen(!Display::fullScreen());
            Display::initFrame();
            gui->reset();
        });
    }, L"Toggle fullscreen", -0.4, 0.4, -0.6, -0.5, Color::RGB(1, 0, 0), Color::V(0), Color::RGB(1,
            0, 1)));
    gui->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->quit();
    }, L"Quit", -0.4, 0.4, -0.7, -0.6, Color::RGB(1, 0, 0), Color::V(0), Color::RGB(1,
            0, 1)));
    gui->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->scheduleFunction([&gui]()
        {
            runSubmenu();
            gui->reset();
        });
    }, L"Run dialog", -0.4, 0.4, -0.9, -0.8, Color::RGB(1, 1, 0), Color::V(0), Color::RGB(1,
            1, 1)));
    gui->add(make_shared<GUICanvas>(-1, -0.5, -0.25, 0.25, []() -> Mesh
    {
        wstring str = L"Hello";
        Mesh mesh = Text::mesh(str, Color::V(1));
        mesh->add(invert(mesh));
        float width = Text::width(str);
        float height = Text::height(str);
        mesh = (Mesh)transform(Matrix::translate(-width / 2, -height / 2, 0)
        .concat(Matrix::scale(2 / width)), mesh);
        Mesh retval = Mesh(new Mesh_t());
        minstd_rand0 randomEngine;
        uniform_real_distribution<double> r(0, 2 * M_PI);
        uniform_real_distribution<double> r2(0, 1);

        for(int i = 0; i < 5; i++)
        {
            retval->add(scaleColors(Color::HSV(r2(randomEngine), 1, 1), transform(Matrix::rotateX(r(randomEngine)).concat(Matrix::rotateY(r(
                randomEngine))).concat(Matrix::rotateZ(r(randomEngine))), mesh)));
        }
        return (Mesh)transform(Matrix::rotateY(Display::timer() * M_PI / 3).concat(Matrix::translate(0, 0, -1)), retval);
    }));

    runAsDialog(gui);

    endGraphics();
    return 0;
}
