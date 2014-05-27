#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include "guielement.h"
#include "guicontainer.h"
#include "guibutton.h"
#include "guilabel.h"
#include "guicanvas.h"
#include "guicircle_arrangement.h"
using namespace std;

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class GUIRunner final : public enable_shared_from_this<GUIRunner>
{
    shared_ptr<GUIContainer> gui;
    vector<function<void()>> functionList;
    static unordered_map<shared_ptr<GUIContainer>, weak_ptr<GUIRunner>> * runners;
    static void makeRunners();
    GUIRunner(shared_ptr<GUIContainer> gui)
        : gui(gui)
    {
        makeRunners();
    }
    GUIRunner(const GUIRunner &) = delete;
    const GUIRunner & operator =(const GUIRunner &) = delete;
    bool runRetval = false;
    bool needQuit = false;
    public:

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    ~GUIRunner()
    {

        if(gui != nullptr && runners != nullptr)
        {
            runners->erase(gui);
        }
    }

    /**
     * @brief Write what the function does here
     *
     * @param gui
     *
     * @return
     **/
    static shared_ptr<GUIRunner> make(shared_ptr<GUIContainer> gui)
    {
        makeRunners();
        gui = gui->getTopLevelParent();
        shared_ptr<GUIRunner> retval = shared_ptr<GUIRunner>(new GUIRunner(gui));
        (*runners)[gui] = retval;
        return retval;
    }

    /**
     * @brief Write what the function does here
     *
     * @param gui
     *
     * @return
     **/
    static shared_ptr<GUIRunner> get(shared_ptr<GUIContainer> gui)
    {
        makeRunners();
        gui = gui->getTopLevelParent();
        auto iter = runners->find(gui);
        if(iter == runners->end())
            return shared_ptr<GUIRunner>(nullptr);
        return std::get<1>(*iter).lock();
    }
    bool run();

    /**
     * @brief Write what the function does here
     *
     * @param fn
     **/
    void scheduleFunction(function<void()> fn)
    {
        functionList.push_back(fn);
    }

    /**
     * @brief Write what the function does here
     *
     * @param false
     **/
    void quit(bool retval = false)
    {
        needQuit = true;
        runRetval = retval;
    }

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    shared_ptr<GUIContainer> getGUI()
    {
        return gui;
    }
};

/**
 * @brief Write what the function does here
 *
 * @param gui
 *
 * @return
 **/
inline bool runAsDialog(shared_ptr<GUIContainer> gui)
{
    shared_ptr<GUIRunner> runner = GUIRunner::make(gui);
    return runner->run();
}
#endif // GUI_H_INCLUDED
