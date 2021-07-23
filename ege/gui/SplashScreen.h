/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#pragma once

#include <ege/core/Progress.h>
#include <ege/gui/GUIScreen.h>
#include <ege/resources/Texture.h>

namespace EGE
{

class SplashScreen : public GUIScreen
{
public:
    enum class State
    {
        None, // nothing done
        Loading, // called by `startLoading`
        Displaying // called by `start`
    };

    State state() const { return m_state; }

    // int load(Progress& progress)
    // It indicates that loading succeeded.
    typedef std::function<void(SharedPtr<Progress>)> Worker;

    SplashScreen(Window& window, String id = "SplashScreen (root)")
    : GUIScreen(window, id) {}

    SharedPtr<Progress> createProgress(size_t steps) { return m_progress = make<Progress>(steps); }

    // Sets a background image.
    void setImage(String name);

    // Creates a new loading thread.
    // `worker` is running in a new thread
    // `callback` is called in GUI thread, when `worker` returns.
    // If it's called if a thread is already running, nothing happens.
    void startLoading(Worker worker, std::function<void(AsyncTask::State)> callback);

    // Displays a splash screen for a specified time.
    // It does not create any loading thread.
    // `callback` is called when the time runs out.
    void start(Time time, std::function<void()> callback);

    // TODO: use ThemeRenderer
    virtual void updateGeometry(Renderer& renderer) override;
    virtual void render(Renderer& renderer) const override;

    // Wait for underlying loading thread.
    void wait();

    // TODO: events

private:
    State m_state = State::None;
    Texture* m_texture = nullptr;
    String m_textureName;
    bool m_running = false;
    SharedPtr<Progress> m_progress;
};

}
