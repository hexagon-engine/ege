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

#include "SplashScreen.h"

#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void SplashScreen::setImage(String name)
{
    m_textureName = name;
    m_texture = nullptr;
    setGeometryNeedUpdate();
}

void SplashScreen::startLoading(Worker worker, std::function<void(AsyncTask::State)> callback)
{
    ASSERT_WITH_MESSAGE(m_progress, "A SplashScreen must have assigned a Progress. Use `createProgress()` to do it.");
    if(m_state != State::None)
    {
        ege_log.warning() << "SplashScreen: Cannot start loading, one already is in progress";
        return;
    }

    m_state = State::Loading;
    auto task = make<AsyncTask>([this, worker](AsyncTask&) {
        worker(m_progress);

        if(m_progress->error())
        {
            ege_log.warning() << "SplashScreen: Worker set error()!";
            return 1;
        }
        ege_log.info() << "SplashScreen: Loading finished successfully.";
        return 0;
    }, [this, callback] (AsyncTask::State state) {
        ege_log.verbose() << "SplashScreen: Callback with finished=" << state.finished << ", rc=" << state.returnCode;
        callback(state);
        m_state = State::None;
    });
    m_progress->setTask(task);
    addAsyncTask(task, "splashScreen");
}

void SplashScreen::start(Time time, std::function<void()> callback)
{
    if(m_state != State::None)
    {
        ege_log.warning() << "SplashScreen: Cannot start loading, one already is in progress";
        return;
    }

    m_state = State::Displaying;
    addTimer("splashScreen", make<Timer>(*this, Timer::Mode::Limited, time, [this, callback](std::string,Timer*) {
        callback();
        m_state = State::None;
    }));
}

void SplashScreen::updateGeometry(Renderer&)
{
    if(!m_texture && !m_textureName.empty())
    {
        m_texture = getLoop().getResourceManager()->getTexture(m_textureName).get();
        m_textureName = ""; // Do not try to load twice if texture not found
    }
}

void SplashScreen::render(Renderer& renderer) const
{
    if(m_texture)
    {
        renderer.renderTexturedRectangle(0, 0, renderer.getTarget().getSize().x, renderer.getTarget().getSize().y, m_texture->getTexture());
    }
    // TODO: display percentage if possible
}

void SplashScreen::wait()
{
    m_progress->task()->wait();
}

}

