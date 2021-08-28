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

#include <ege/core/Component.h>
#include <ege/event/SystemWindow.h>
#include <ege/gui/GUIScreen.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class GUIGameLoop;

// NOTE: We are using some dummy ChildType, it won't be accesible from outside anyway.
// FIXME: Don't need to provide valid Component type as ChildType.
class Window : public SFMLSystemWindow, public BasicComponent<GUIScreen>
{
public:
    Window(GUIGameLoop& owner, String id = "Window");

    EGE_ENUM_YES_NO(GUIScreenImmediateInit);

    virtual void onTick() override;

    void setBackgroundColor(ColorRGBA color) { m_backgroundColor = color; }
    ColorRGBA getBackgroundColor() const { return m_backgroundColor; }

    void setGUIScreen(SharedPtr<GUIScreen> screen, GUIScreenImmediateInit init = Window::GUIScreenImmediateInit::No);

    // T must be derived from GUIScreen
    template<class T, class... Args>
    SharedPtr<T> setNewGUIScreen(Args&&... args)
    {
        static_assert(std::is_base_of_v<GUIScreen, T>);
        auto gui = make<T>(*this, args...);
        setGUIScreen(gui);
        return gui;
    }

    SharedPtr<GUIScreen> getGUIScreen() const { return m_currentGui; }

    Renderer& getRenderer() { return m_renderer; }
    Renderer const& getRenderer() const { return m_renderer; }

    GUIGameLoop& getGUILoop() { return m_loop; }
    const GUIGameLoop& getGUILoop() const { return m_loop; }

    void render();

private:
    virtual void onCreate() override;

    SharedPtr<GUIScreen> m_currentGui;
    // to allow animations and lazy-load
    SharedPtr<GUIScreen> m_pendingGui;
    Renderer m_renderer;
    GUIGameLoop& m_loop;

    ColorRGBA m_backgroundColor;
};

}
