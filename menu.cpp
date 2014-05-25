#include "menu.h"
#include "gui.h"
#include "platform.h"
#include "cubicspline.h"
#include "game_state.h"
#include <cstdlib>

namespace
{

class MyCanvas : public GUICanvas
{
    Mesh theMesh;
    VectorF lastPosition;
    bool mouseDown = false;
    VectorF getMousePosition(MouseEvent &event)
    {
        VectorF retval = Display::transformMouseTo3D(event.x, event.y, 1);
        float height = maxY - minY;
        float width = maxX - minX;
        float scale = min(height, width);
        retval.x -= (minX + maxX) / 2;
        retval.x /= scale;
        retval.y -= (minY + maxY) / 2;
        retval.y /= scale;
        return retval;
    }
public:
    MyCanvas(float minX, float maxX, float minY, float maxY)
        : GUICanvas(minX, maxX, minY, maxY), theMesh(Mesh(new Mesh_t()))
    {
    }
    virtual bool handleMouseMove(MouseMoveEvent &event) override
    {
        if(!mouseDown)
            return true;
        VectorF currentPosition = getMousePosition(event);
        theMesh->add(Generate::line({lastPosition, currentPosition}, TextureAtlas::ButtonMiddleDiffuse.td(), Color::V(1), 0.1));
        lastPosition = currentPosition;
        return true;
    }
    virtual bool handleMouseMoveOut(MouseEvent &event) override
    {
        mouseDown = false;
        return true;
    }
    virtual bool handleMouseDown(MouseDownEvent &event) override
    {
        if(event.button == MouseButton_Left)
        {
            mouseDown = true;
            lastPosition = getMousePosition(event);
        }
        return true;
    }
    virtual bool handleMouseUp(MouseUpEvent &event) override
    {
        if(event.button == MouseButton_Left)
        {
            mouseDown = false;
        }
        return true;
    }
protected:
    virtual Mesh generateMesh() const override
    {
        return theMesh;
    }
};
}

static const wchar_t *const creditsText =
    L"Credits:\n"
    "TODO: create text";

static shared_ptr<GUICircleArrangement> createCircleArrangement()
{
    Mesh fmesh = (Mesh)transform(Matrix::scale(0.8), Generate::quadrilateral(TextureAtlas::MenuGearTop.td(), VectorF(-1, -1, -1), Color(1), VectorF(1, -1, -1), Color(1), VectorF(1, 1, -1), Color(1),
                                 VectorF(-1, 1, -1), Color(1)));
    Mesh rmesh = Generate::quadrilateral(TextureAtlas::MenuGearBottom.td(), VectorF(-1, -1, -1), Color(1), VectorF(1, -1, -1), Color(1), VectorF(1, 1, -1), Color(1), VectorF(-1, 1, -1), Color(1));
    return make_shared<GUICircleArrangement>(-1, 1, -1, 1, -M_PI / 3, M_PI / 3, VectorF(-0.7, 0, 0), 0.8, fmesh, VectorF(0), rmesh, VectorF(-0.35, 0.35, 0), VectorF(0, 0.5, 0));
}

static void creditsScreen()
{
    shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(), -Display::scaleY(), Display::scaleY());
    gui->add(make_shared<GUILabel>(creditsText, -1, 1, -0.8, 1, Color::V(0)));
    gui->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->quit();
    }, L"Return to Main Menu", -0.4, 0.4, -0.95, -0.85));
    runAsDialog(gui);
}

static void mainGame()
{
    shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(), -Display::scaleY(), Display::scaleY());
    gui->add(make_shared<GUILabel>(creditsText, -1, 1, -0.8, 1, Color::V(0)));
    gui->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->quit();
    }, L"Return to Main Menu", -0.4, 0.4, -0.95, -0.85));
    runAsDialog(gui);
}

static void testBigButton()
{
    shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(), -Display::scaleY(), Display::scaleY());
    gui->add(make_shared<GUICanvas>(-0.9, 0.9, -0.9, 0.9, []()->Mesh
    {
        GameState gs = makeEmptyGameState();
        auto node1 = gs->addNode(make_shared<Node>(VectorF(-0.5, 0.5, 0)));
        auto node2 = gs->addNode(make_shared<Node>(VectorF(-0.5, -0.5, 0)));
        gs->addEdge(make_shared<Edge>(vector<CubicSpline>{CubicSpline((*node1)->position, (*node2)->position, VectorF(1, 0, 0), VectorF(1, 0, 0))}, nullptr, nullptr), node1, node2);
        return renderGameState(gs);
    }));
    gui->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->quit();
    }, L"Return", -0.4, 0.4, -1, -0.9));
    runAsDialog(gui);
}

static Mesh shadeMesh(Mesh mesh)
{
    vector<Triangle> triangles;
    triangles.reserve(mesh->size());
    VectorF lightVector(1, 1, 1);
    lightVector = normalize(lightVector);
    VectorF specularityLightVector = normalize(lightVector + VectorF(0, 0, 1));
    for(Triangle tri : *mesh)
    {
        VectorF normal = tri.normal();
        float v = dot(normal, lightVector);
        if(v < 0)
            v = 0;
        Color c = Color::HSB(0, 1, min(1.0, 0.3 * v + 0.15 + 0.5 * pow(max<float>(0.0, dot(normal, specularityLightVector)), 16)));
        tri.c[0] = scale(c, tri.c[0]);
        tri.c[1] = scale(c, tri.c[1]);
        tri.c[2] = scale(c, tri.c[2]);
        triangles.push_back(tri);
    }
    return Mesh(new Mesh_t(mesh->texture(), triangles));
}

static void testSplineRendering()
{
    shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(), -Display::scaleY(), Display::scaleY());
    gui->add(make_shared<GUICanvas>(-0.9, 0.9, -0.9, 0.9, []()->Mesh
    {
#if 0
        Mesh mesh = Generate::unitBox(TextureAtlas::Point.td(), TextureAtlas::Point.td(), TextureAtlas::Point.td(), TextureAtlas::Point.td(), TextureAtlas::Point.td(), TextureAtlas::Point.td());
        mesh = (Mesh)transform(Matrix::translate(VectorF(-0.5)).concat(Matrix::rotateX(Display::timer())).concat(Matrix::rotateY(Display::timer() * M_PI)), mesh);
        mesh->add(invert(mesh));
        mesh = shadeMesh(mesh);
        mesh = (Mesh)transform(Matrix::translate(0, 0, -2), mesh);
        return mesh;
        #warning change #if 1 to #if 0 when CubicSpline::renderSplineList is implemented
#else
        vector<CubicSpline> testSplineList;
        VectorF dir = Matrix::rotateZ(Display::timer()).apply(VectorF(0, 10, 0));
        testSplineList.push_back(CubicSpline(VectorF(-3, -3, 0), VectorF(0, 0, 0), dir, dir));
        testSplineList.push_back(CubicSpline(VectorF(0, 0, 0), VectorF(3, 3, 0), dir, dir));
        Mesh mesh = CubicSpline::renderSplineList(testSplineList, TextureAtlas::MenuGearTop.td(), Color::V(1), 0.4);
        //mesh->add(invert(mesh));
        return (Mesh)transform(Matrix::translate(0, 0, -10), mesh);
#endif
    }));
    gui->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->quit();
    }, L"Return to Video Settings", -0.4, 0.4, -1, -0.9));
    runAsDialog(gui);
}

static void videoSettingsMenu()
{
    for(;;)
    {
        shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(), -Display::scaleY(), Display::scaleY());
        shared_ptr<GUICircleArrangement> circleArrangement = createCircleArrangement();
        circleArrangement->add(make_shared<GUIButton>([&gui]()
        {
            GUIRunner::get(gui)->scheduleFunction([&gui]()
            {
                Display::fullScreen(!Display::fullScreen());
                GUIRunner::get(gui)->quit(true);
            });
        }, L"Toggle Fullscreen", -0.4, 0.4, -0.8, -0.7));

        if(!Display::fullScreen())
        {
            circleArrangement->add(make_shared<GUIButton>([&gui]()
            {
                GUIRunner::get(gui)->scheduleFunction([&gui]()
                {
                    Display::setSize(1024, 768);
                    GUIRunner::get(gui)->quit(true);
                });
            }, L"Set to 1024x768", -0.4, 0.4, -0.8, -0.7));
            circleArrangement->add(make_shared<GUIButton>([&gui]()
            {
                GUIRunner::get(gui)->scheduleFunction([&gui]()
                {
                    Display::setSize(768, 1024);
                    GUIRunner::get(gui)->quit(true);
                });
            }, L"Set to 768x1024", -0.4, 0.4, -0.8, -0.7));
        }

        circleArrangement->add(make_shared<GUIButton>([&gui]()
        {
            GUIRunner::get(gui)->scheduleFunction([&gui]()
            {
                testSplineRendering();
                gui->reset();
            });
        }, L"Test Spline Rendering", -0.4, 0.4, -0.8, -0.7));
        circleArrangement->add(make_shared<GUIButton>([&gui]()
        {
            GUIRunner::get(gui)->quit(false);
        }, L"Return to Settings", -0.4, 0.4, -0.8, -0.7));
        gui->add(circleArrangement);

        if(!runAsDialog(gui))
        {
            break;
        }
    }
}

static void settingsMenu()
{
    shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(), -Display::scaleY(), Display::scaleY());
    shared_ptr<GUICircleArrangement> circleArrangement = createCircleArrangement();
    circleArrangement->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->scheduleFunction([&gui]()
        {
            videoSettingsMenu();
            gui->reset();
        });
    }, L"Video Settings", -0.4, 0.4, -0.8, -0.7));
    circleArrangement->add(make_shared<GUIButton>([]()
    {
    }, L"Audio Settings", -0.4, 0.4, -0.8, -0.7));
    circleArrangement->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->quit();
    }, L"Return to Main Menu", -0.4, 0.4, -0.8, -0.7));
    gui->add(circleArrangement);
    runAsDialog(gui);
}

void mainMenu()
{
    shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(), -Display::scaleY(), Display::scaleY());
    shared_ptr<GUICircleArrangement> circleArrangement = createCircleArrangement();
    circleArrangement->add(make_shared<GUIButton>([]()
    {
    }, L"Start New Game", -0.4, 0.4, -0.8, -0.7));
    circleArrangement->add(make_shared<GUIButton>([]()
    {
    }, L"Resume Game", -0.4, 0.4, -0.8, -0.7));
    circleArrangement->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->scheduleFunction([&gui]()
        {
            settingsMenu();
            gui->reset();
        });
    }, L"Settings", -0.4, 0.4, -0.8, -0.7));
    circleArrangement->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->scheduleFunction([&gui]()
        {
            creditsScreen();
            gui->reset();
        });
    }, L"Credits", -0.4, 0.4, -0.8, -0.7));
    circleArrangement->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->scheduleFunction([&gui]()
        {
            testBigButton();
            gui->reset();
        });
    }, L"Test", -0.4, 0.4, -0.8, -0.7));
    circleArrangement->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->scheduleFunction([]()
        {
            endGraphics();
            exit(0);
        });
    }, L"Quit", -0.4, 0.4, -0.8, -0.7));
    gui->add(circleArrangement);
    runAsDialog(gui);
}

