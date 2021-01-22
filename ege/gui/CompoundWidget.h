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

#include "Widget.h"

#include <ege/gfx/RenderStates.h>

namespace EGE
{

class CompoundWidget : public Widget
{
public:
    explicit CompoundWidget(Widget& parent, String id = "CompoundWidget")
    : Widget(parent, id) {}

    explicit CompoundWidget(GUIGameLoop& loop, String id = "CompoundWidget (root)")
    : Widget(loop, id) {}

    // System Events -- are passed to all child widgets
    virtual void onClose() override {}
    virtual void onResize(sf::Event::SizeEvent& event) override;
    virtual void onTextEnter(sf::Event::TextEvent& event) override;
    virtual void onKeyPress(sf::Event::KeyEvent& event) override;
    virtual void onKeyRelease(sf::Event::KeyEvent& event) override;
    virtual void onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event) override;
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event) override;
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event) override;
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event) override;
    virtual void onMouseLeave() override;
    virtual void onJoystickButtonPress(sf::Event::JoystickButtonEvent& event) override;
    virtual void onJoystickButtonRelease(sf::Event::JoystickButtonEvent& event) override;
    virtual void onJoystickMove(sf::Event::JoystickMoveEvent& event) override;
    virtual void onJoystickConnect(sf::Event::JoystickConnectEvent& event) override;
    virtual void onJoystickDisconnect(sf::Event::JoystickConnectEvent& event) override;
    virtual void onTouchBegin(sf::Event::TouchEvent& event) override;
    virtual void onTouchMove(sf::Event::TouchEvent& event) override;
    virtual void onTouchEnd(sf::Event::TouchEvent& event) override;
    virtual void onSensorChange(sf::Event::SensorEvent& event) override;

    // GUI-specific Events
    virtual void onCommand(const Command& command) override
    {
        auto parent = getParentWidget();
        if(parent)
            parent->onCommand(command);
    }

    // Widget Events
    virtual void onUpdate(long long tickCounter) override;

    virtual void doRender(Renderer& renderer, const RenderStates& states = {}) override;

    template<class T, class... Args>
    SharedPtr<T> addNewWidget(Args&&... args)
    {
        auto widget = make<T>(*this, std::forward<Args>(args)...);
        addWidget(widget);
        return widget;
    }

    void addWidget(SharedPtr<Widget> widget);
    void removeWidget(Widget* widget);

    virtual void renderOverlay(Renderer&) const {}

    Widget* getWidget(Size index) const { return index < m_childWidgets.size() ? m_childWidgets[index].get() : nullptr; }
    void setFocus(Size index);
    void clearFocus();

    // slow
    void setFocus(Widget& widget);

    int getFocusedWidgetIndex() const { return m_focusedWidget; }
    Widget* getFocusedWidget() const { return m_focusedWidget >= 0 ? m_childWidgets[m_focusedWidget].get() : nullptr; }

protected:
    virtual void updateLayout() override;

private:
    Vector<SharedPtr<Widget>> m_childWidgets;
    int m_focusedWidget = -1;
};

}
