#include "menu.h"
#include "gui.h"
#include "platform.h"
#include "cubicspline.h"
#include "game_state.h"
#include <cstdlib>
#include <sstream>

using namespace std;

static GameState makeTestGameState();

namespace
{

class GameCanvas : public GUICanvas
{
    vector<VectorF> line;
    bool mouseDown = false;
    GameStateStack gss;
    VectorF mousePosition = VectorF(0);
    shared_ptr<Region> mouseRegion = nullptr;
    shared_ptr<Node> mouseNode = nullptr;
    inline VectorF getMousePosition(MouseEvent &event) // sets the mousePosition variable
    {
        mousePosition = GUICanvas::getMousePosition(event);
        mousePosition.z = 0;
        mouseRegion = pointToRegion(gss.top(), mousePosition);
        mouseNode = findClosestNode(gss.top(), mousePosition);
        if(mouseNode)
            mousePosition = mouseNode->position;
        return mousePosition;
    }
    inline Mesh makeMesh() const
    {
        return CubicSpline::renderSplineList(splinesFromLines(line), TextureAtlas::ButtonMiddleDiffuse.td(), Color::HSV(fmod(Display::timer() / 10, 1), 1, 1), 0.005);
    }
public:
    GameCanvas(float minX, float maxX, float minY, float maxY, GameState gs)
        : GUICanvas(minX, maxX, minY, maxY)
    {
        gss.push(gs);
    }
    virtual bool handleMouseMove(MouseMoveEvent &event) override
    {
        if(line.size() < 1)
            line.push_back(VectorF());
        line.back() = getMousePosition(event);
        if(!mouseDown)
            return true;
        return true;
    }
    virtual bool handleMouseMoveOut(MouseEvent &event) override
    {
        return true;
    }
    virtual bool handleMouseDown(MouseDownEvent &event) override
    {
        getMousePosition(event);
        if(event.button == MouseButton_Left && line.size() >= 2 && mouseNode && mouseDown)
        {
            line.back() = mousePosition;
            vector<CubicSpline> path = splinesFromLines(line);
            line.clear();
            shared_ptr<Node> startNode = findClosestNode(gss.top(), path.front().p0);
            GameState newGameState = move(gss.top(), startNode, mouseNode, path);
            if(newGameState)
                gss.push(newGameState);
            mouseDown = false;
        }
        else if(event.button == MouseButton_Left && (mouseNode || mouseDown))
        {
            mouseDown = true;
            line.push_back(mousePosition);
            if(line.size() <= 1)
                line.push_back(mousePosition);
        }
        return true;
    }
    virtual bool handleMouseUp(MouseUpEvent &event) override
    {
        return true;
    }
protected:
    virtual Mesh generateMesh() override
    {
        Mesh retval = renderGameState(gss.top());
        if(mouseRegion)
        {
            for(Land land : mouseRegion->lands)
            {
                Color color = (land.isInverted ? Color::RGB(0, 1, 0) : Color::RGB(0, 0, 1));
                retval->add(transform(Matrix::translate(0, 0, -1).concat(Matrix::scale(0.25)), Generate::lineLoop(land.polygon, TextureAtlas::ButtonMiddleDiffuse.td(), color, 0.003)));
            }
        }
        if(mouseNode)
        {
            retval->add(transform(Matrix::scale(0.25f), renderNode(mouseNode, Color::V(1))));
        }
        retval->add(transform(Matrix::translate(0, 0, -1).concat(Matrix::scale(0.25f * 0.75f)), makeMesh()));
        return retval;
    }
    friend class GameStateLabel;
};

class GameStateLabel : public GUILabel
{
    shared_ptr<GameCanvas> gameCanvas;
public:
    GameStateLabel(float minX, float maxX, float minY, float maxY, shared_ptr<GameCanvas> gameCanvas)
        : GUILabel(L"", minX, maxX, minY, maxY), gameCanvas(gameCanvas)
    {
    }
protected:
    virtual Mesh render(float minZ, float maxZ, bool hasFocus) override
    {
        wstringstream os;
        //os << L"Region : " << gameCanvas->mouseRegion;
        text = os.str();
        return GUILabel::render(minZ, maxZ, hasFocus);
    }
};

class MyCanvas : public GUICanvas
{
    vector<VectorF> line;
    Mesh mesh;
    bool mouseDown = false;
    size_t lineSegmentCount = 0;
    inline Mesh makeMesh() const
    {
        return CubicSpline::renderSplineList(splinesFromLines(line), TextureAtlas::ButtonMiddleDiffuse.td(), Color::HSV(fmod(Display::timer() / 10, 1), 1, 1), 0.01);
    }
public:
    MyCanvas(float minX, float maxX, float minY, float maxY)
        : GUICanvas(minX, maxX, minY, maxY), mesh(Mesh(new Mesh_t))
    {
    }
    virtual bool handleMouseMove(MouseMoveEvent &event) override
    {
        if(!mouseDown)
            return true;
        line.back() = getMousePosition(event);
        return true;
    }
    virtual bool handleMouseMoveOut(MouseEvent &event) override
    {
        return true;
    }
    virtual bool handleMouseDown(MouseDownEvent &event) override
    {
        if(event.button == MouseButton_Left)
        {
            mouseDown = true;
            line.push_back(getMousePosition(event));
            if(line.size() <= 1)
                line.push_back(getMousePosition(event));
            lineSegmentCount++;
        }
        if(event.button == MouseButton_Right && line.size() >= 2)
        {
            line.back() = getMousePosition(event);
            mesh->add(makeMesh());
            line.clear();
            if(mouseDown)
            {
                line.push_back(getMousePosition(event));
            }
        }
        return true;
    }
    virtual bool handleMouseUp(MouseUpEvent &event) override
    {
        return true;
    }
    size_t getLineSegmentCount() const
    {
        return lineSegmentCount;
    }
protected:
    virtual Mesh generateMesh() override
    {
        Mesh retval = Mesh(new Mesh_t(*mesh));
        retval->add(makeMesh());
        return retval;
    }
};

class MyCanvasLineCountLabel : public GUILabel
{
    shared_ptr<MyCanvas> myCanvas;
public:
    MyCanvasLineCountLabel(float minX, float maxX, float minY, float maxY, shared_ptr<MyCanvas> myCanvas)
        : GUILabel(L"", minX, maxX, minY, maxY), myCanvas(myCanvas)
    {
    }
protected:
    virtual Mesh render(float minZ, float maxZ, bool hasFocus) override
    {
        wstringstream os;
        os << L"Line Segment Count : " << myCanvas->getLineSegmentCount();
        text = os.str();
        return GUILabel::render(minZ, maxZ, hasFocus);
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

static GameState makeInitialGameState(int nodeCount = 3)
{
    GameState gs = makeEmptyGameState();
    for(int i = 0; i < nodeCount; i++)
    {
        float angle = (float)i / nodeCount * 2 * M_PI;
        gs->addNode(make_shared<Node>(0.5 * VectorF(cos(angle), sin(angle), 0)));
    }
    recalculateRegions(gs);
    return gs;
}

static void mainGame()
{
    shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(), -Display::scaleY(), Display::scaleY());
    gui->add(make_shared<GameCanvas>(-Display::scaleX(), Display::scaleX(), 0.1 - Display::scaleY(), Display::scaleY(), makeInitialGameState()));
    gui->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->quit();
    }, L"Return to Main Menu", -0.4, 0.4, -Display::scaleY(), 0.1 - Display::scaleY()));
    runAsDialog(gui);
}

static decltype(makeEmptyGameState()->begin()) addNodeToGraph(GameState gs, VectorF position)
{
    return gs->addNode(make_shared<Node>(position));
}

static void addEdgeToGraph(GameState gs, decltype(makeEmptyGameState()->begin()) node1, decltype(makeEmptyGameState()->begin()) node2)
{
    auto edge = make_shared<Edge>(vector<CubicSpline>{CubicSpline((*node1)->position, (*node2)->position)}, *node1, *node2);
    gs->addEdge(edge, node1, node2);
    gs->addEdge(edge, node2, node1);
}

static void addEdgeToGraph(GameState gs, decltype(makeEmptyGameState()->begin()) node1, decltype(makeEmptyGameState()->begin()) node2, VectorF dp1, VectorF dp2)
{
    auto edge = make_shared<Edge>(vector<CubicSpline>{CubicSpline((*node1)->position, (*node2)->position, dp1, dp2)}, *node1, *node2);
    gs->addEdge(edge, node1, node2);
    gs->addEdge(edge, node2, node1);
}

static GameState makeTestGameState()
{
    GameState gs = makeEmptyGameState();
    auto n11 = addNodeToGraph(gs, VectorF(0.2, 0, 0));
    auto n12 = addNodeToGraph(gs, VectorF(0.3, 0, 0));
    auto n1 = addNodeToGraph(gs, VectorF(-0.9, -0.3, 0));
    auto n2 = addNodeToGraph(gs, VectorF(-0.7, 0, 0));
    auto n3 = addNodeToGraph(gs, VectorF(-0.6, 0, 0));
    auto n4 = addNodeToGraph(gs, VectorF(-0.5, 0, 0));
    auto n5 = addNodeToGraph(gs, VectorF(-0.4, 0, 0));
    auto n6 = addNodeToGraph(gs, VectorF(-0.7, 0.1, 0));
    auto n7 = addNodeToGraph(gs, VectorF(-0.9, 0.3, 0));
    auto n8 = addNodeToGraph(gs, VectorF(-0.6, 0.5, 0));
    auto n9 = addNodeToGraph(gs, VectorF(0, 0, 0));
    auto n10 = addNodeToGraph(gs, VectorF(0.1, 0, 0));
    auto n13 = addNodeToGraph(gs, VectorF(0.4, 0, 0));
    addEdgeToGraph(gs, n1, n7);
    addEdgeToGraph(gs, n1, n5);
    addEdgeToGraph(gs, n1, n2);
    addEdgeToGraph(gs, n2, n3);
    addEdgeToGraph(gs, n2, n7);
    addEdgeToGraph(gs, n3, n4, VectorF(0.01, -.1, 0), VectorF(0.01, .1, 0));
    addEdgeToGraph(gs, n3, n4, VectorF(0.01, .1, 0), VectorF(0.01, -.1, 0));
    addEdgeToGraph(gs, n4, n5);
    addEdgeToGraph(gs, n5, n7);
    //addEdgeToGraph(gs, n5, n9);
    addEdgeToGraph(gs, n9, n13, VectorF(0.01, -2, 0), VectorF(0.01, 2, 0));
    addEdgeToGraph(gs, n9, n13, VectorF(0.01, 2, 0), VectorF(0.01, -2, 0));
    addEdgeToGraph(gs, n11, n12, VectorF(0.05, -1, 0), VectorF(0.05, 1, 0));
    addEdgeToGraph(gs, n11, n12, VectorF(0.05, 1, 0), VectorF(0.05, -1, 0));
    recalculateRegions(gs);
    return gs;
}

static void testBigButton()
{
    shared_ptr<GUIContainer> gui = make_shared<GUIContainer>(-Display::scaleX(), Display::scaleX(), -Display::scaleY(), Display::scaleY());
#if 0
    shared_ptr<MyCanvas> canvas = make_shared<MyCanvas>(-0.9, 0.9, -0.9, 0.9);
    gui->add(canvas);
    gui->add(make_shared<MyCanvasLineCountLabel>(-0.4, 0.4, 0.9, 1, canvas));
#elif 1
    shared_ptr<GameCanvas> canvas = make_shared<GameCanvas>(-0.9, 0.9, -0.9, 0.9, makeTestGameState());
    gui->add(canvas);
    gui->add(make_shared<GameStateLabel>(-0.4, 0.4, 0.9, 1, canvas));
#else
    gui->add(make_shared<GUICanvas>(-0.9, 0.9, -0.9, 0.9, []()->Mesh
    {
        GameState gs = makeEmptyGameState();
        auto node1 = gs->addNode(make_shared<Node>(VectorF(-0.5, 0.5, 0)));
        auto node2 = gs->addNode(make_shared<Node>(VectorF(-0.5, -0.5, 0)));
        shared_ptr<Edge> edge1 = make_shared<Edge>(vector<CubicSpline>{CubicSpline((*node1)->position, (*node2)->position, VectorF(1, 0, 0), VectorF(1, 0, 0))}, *node1, *node2);
        gs->addEdge(edge1, node1, node2);
        gs->addEdge(edge1, node2, node1);
        return renderGameState(gs);
    }));
#endif
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
    circleArrangement->add(make_shared<GUIButton>([&gui]()
    {
        GUIRunner::get(gui)->scheduleFunction([&gui]()
        {
            mainGame();
            gui->reset();
        });
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
        GUIRunner::get(gui)->scheduleFunction([]()
        {
            endGraphics();
            exit(0);
        });
    }, L"Quit", -0.4, 0.4, -0.8, -0.7));
    gui->add(circleArrangement);
    runAsDialog(gui);
}

