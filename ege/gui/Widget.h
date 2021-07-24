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

#include "Animatable.h"
#include "Animation.h"
#include "LayoutElement.h"

#include <ege/debug/Logger.h>
#include <ege/event/DefaultSystemEventHandler.h>
#include <ege/gfx/Renderable.h>
#include <ege/gfx/Renderer.h>
#include <ege/resources/ResourceManager.h>
#include <ege/util/Vector.h>

#define WIDGET_DEBUG 0

namespace EGE
{

class GUIGameLoop;
class Window;

class Widget : public Animatable, public DefaultSystemEventHandler, public LayoutElement
{
public:
    class Command
    {
    public:
        std::string getId() const
        {
            return m_id;
        }

    protected:
        Command(std::string id)
        : m_id(id) {}

    private:
        std::string m_id;
    };

    // NOTE: This is very similar to copy constructor.
    explicit Widget(Widget& parent, String id = "Widget");

    // for non-parented widgets, e.g. GUIScreen
    // NOTE: For layouting, they must have fixed (px or A) size set!
    explicit Widget(Window& gameLoop, String id = "Widget (root)");

    GUIGameLoop& getLoop() const;
    Window& getWindow() const { return m_window; }
    SharedPtr<ResourceManager> getResourceManager() const;

    virtual sf::FloatRect getBoundingBox();
    virtual sf::FloatRect getViewport(sf::RenderTarget& target) const;

    virtual void onUpdate(long long tickCounter);

    virtual void onCreate() {}
    virtual void onCommand(const Command&) {}

    virtual void onResize(sf::Event::SizeEvent& event) override;
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event) override;
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event) override;
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event) override;

    virtual bool isMouseOver(Vec2d position);
    virtual sf::View getCustomView(sf::RenderTarget& target) const override;
    virtual bool isCustomViewNeeded() const override { return true; }

    bool hasFocus() const { return m_hasFocus; }

    Widget* getParentWidget() const { return m_parentWidget; }

    virtual void setPosition(LVec2d position) override { LayoutElement::setPosition(position); setGeometryNeedUpdate(); }
    virtual void setSize(LVec2d size) override  { LayoutElement::setSize(size); setGeometryNeedUpdate(); }
    virtual void setPadding(LVec2d padding) override  { LayoutElement::setPadding(padding); setGeometryNeedUpdate(); }

    // Called before calculating layout, when rendering.
    // In this method, the widget is required to set up
    // all data needed to calculate layout (e.g set content
    // size).
    virtual void updateLayout();
    virtual void setFocus(bool value = true) { m_hasFocus = value; }

    void hide(bool hide = true) { m_hide = hide; setGeometryNeedUpdate(); m_mouseOver = false; m_leftClicked = false; }
    bool isHidden() const { return m_hide; }

protected:
    virtual void render(Renderer& renderer) const override;

    bool m_mouseOver = false;
    bool m_leftClicked = false;
    Window& m_window;
    Widget* m_parentWidget;
    bool m_hide = false;

private:
    bool m_hasFocus = false;
};

// TODO: move it to another file
class DummyWidget : public Widget
{
public:
    DummyWidget(Widget& parent)
    : Widget(parent) {}
};

}
