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

#include "Widget.h"

#include "GUIGameLoop.h"
#include "ege/core/BasicComponent.h"

#include <ege/debug/Logger.h>
#include <ege/main/Config.h>
#include <ege/util/GlobalConfig.h>
#include <ege/util/PointerUtils.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

Widget::Widget(Widget& parent, String id)
: Animatable(parent, "Widget: " + id)
, DefaultSystemEventHandler(parent.m_window)
, LayoutElement(&parent, id)
, m_window(parent.m_window) {}

Widget::Widget(Window& window, String id)
: Animatable(window, id)
, DefaultSystemEventHandler(window)
, LayoutElement(nullptr, id)
, m_window(window) {}

sf::FloatRect Widget::getBoundingBox() const
{
    return sf::FloatRect(getAbsolutePosition().x, getAbsolutePosition().y, getSize().x, getSize().y);
}
sf::FloatRect Widget::getViewport(sf::RenderTarget& target) const
{
    sf::Vector2u windowSize;
    windowSize = target.getSize();

    EGE::Vec2d widgetPosition = getAbsolutePosition();

    sf::FloatRect currentRect(widgetPosition.x / windowSize.x, widgetPosition.y / windowSize.y,
                  getSize().x / windowSize.x, getSize().y / windowSize.y);

    auto parent = tryGetParent<Widget>();
    if(parent)
    {
        sf::FloatRect intersection;
        currentRect.intersects((sf::FloatRect)parent->getViewport(target), intersection);
        return intersection;
    }
    else
        return currentRect;
}

SharedPtr<ResourceManager> Widget::getResourceManager() const
{
    return getLoop().getResourceManager();
}

GUIGameLoop& Widget::getLoop() const
{
    return m_window.getGUILoop();
}

void Widget::render(Renderer& renderer) const
{
    // draw some debug shape
    if(EGE_GCONFIG_IS_SET(GUI_LayoutBox))
    {
        // TODO: Use Renderer's methods
        sf::RectangleShape rs(sf::Vector2f(getSize().x, getSize().y) - sf::Vector2f(2.f, 2.f));
        rs.setPosition(sf::Vector2f(1.f, 1.f));
        rs.setOutlineColor(sf::Color::Cyan);

        if(hasFocus())
        {
            rs.setOutlineThickness(2.f);
            rs.setSize(sf::Vector2f(getSize().x, getSize().y) - sf::Vector2f(4.f, 4.f));
            rs.setPosition(sf::Vector2f(2.f, 2.f));
            rs.setOutlineColor(sf::Color::Yellow);
        }
        else
            rs.setOutlineThickness(1.f);

        rs.setFillColor(sf::Color::Transparent);
        if(m_mouseOver)
            rs.setOutlineColor(sf::Color::Red);
        if(m_leftClicked)
            rs.setOutlineColor(sf::Color::Green);
        renderer.getTarget().draw(rs);

        //renderer.renderText(0, 0, *getLoop().getResourceManager()->getDefaultFont(), std::to_string(getPosition().x) + "," + std::to_string(getPosition().y), 10, sf::Color::Black);
    }
}

void Widget::onResize(sf::Event::SizeEvent&)
{
    setGeometryNeedUpdate();
}

void Widget::onMouseMove(sf::Event::MouseMoveEvent& event)
{
    bool mouseOver = isMouseOver(EGE::Vec2d(event.x, event.y));
    if(mouseOver)
    {
        if(!m_mouseOver)
            onMouseEnter();
    }
    else
    {
        m_leftClicked = false;
        if(m_mouseOver)
            onMouseLeave();
    }
    m_mouseOver = mouseOver;

    Vec2d position = Vec2d(event.x, event.y);
    forEachChildTyped<Widget>([&](auto& widget) {
        sf::Event event2;
        event2.type = sf::Event::MouseMoved;
        event2.mouseMove = { (int)position.x, (int)position.y };
        widget.template fire<SystemEvent>(event2);
    });
}

void Widget::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    ege_log.debug() << "Widget::onMouseButtonPress for " << isnName();
    auto position = Vec2i(event.x, event.y);
    if(event.button == sf::Mouse::Left)
    {
        ege_log.debug() << "!! leftClicked " << isnName();
        m_leftClicked = true;
    }

    forEachChildTyped<Widget>([&](auto& widget) {
        ege_log.debug() << "  Widget " << isnName() << " pos(" << widget.getAbsolutePosition().x << "," << widget.getAbsolutePosition().y << ")"
        << " size(" << widget.getSize().x << "," << widget.getSize().y << ")";
        ege_log.debug() << "mousepos: " << position.x << "," << position.y;
        if(widget.isMouseOver(position) && event.button == sf::Mouse::Left)
        {
            ege_log.debug() << "- isMouseOver!";
            // Change focused widget.
            setFocus(widget);

            sf::Event event2;
            event2.type = sf::Event::MouseButtonPressed;
            event2.mouseButton = { event.button, position.x, position.y };

            // Handle self
            widget.onMouseButtonPress(event);

            // Pass down to children
            widget.template fire<SystemEvent>(event2);
        }
    });
}

void Widget::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    if(event.button == sf::Mouse::Left)
    {
        m_leftClicked = false;
    }
}

EventResult Widget::fireEvent(Event& event)
{
    auto result = Animatable::fireEvent(event);
    if(dynamic_cast<SystemEvent*>(&event))
        return EventResult((bool)handle(static_cast<SystemEvent&>(event)) || (bool)result);
    return result;
}

bool Widget::isMouseOver(EGE::Vec2d position) const
{
    return getBoundingBox().contains(sf::Vector2f(position.x, position.y));
}

sf::View Widget::getCustomView(sf::RenderTarget& target) const
{
    sf::FloatRect viewport = getViewport(target);

    // We need to calculate offset if viewport position is not corresponding to actual position.
    // It may happen when we intersect viewports.
    auto [x, y] = getAbsolutePosition();
    auto [vx, vy] = viewport.getPosition();
    sf::Vector2f offsetPosition = sf::Vector2f(vx * target.getSize().x - x, vy * target.getSize().y - y);

    sf::View view(sf::FloatRect(offsetPosition,
                                sf::Vector2f(viewport.getSize().x * target.getSize().x, viewport.getSize().y * target.getSize().y)));
    view.setViewport(viewport);
    return view;
}

void Widget::doRender(Renderer& renderer, const RenderStates& states)
{
    //if constexpr(WIDGET_DEBUG) ege_log.info() << "Widget::doRender(" << renderer.getTarget().getSize().x << "," << renderer.getTarget().getSize().y << ")";
    // TODO: draw only visible widgets

    auto newStates = states;
    applyStates(renderer, newStates);
    Renderable::doRender(renderer, newStates);

    // Render child widgets
    forEachChildTyped<Widget>([&](auto& widget) {
        if(widget.isHidden())
            return;

        applyStates(renderer);
        /*if constexpr(WIDGET_DEBUG) ege_log.info() << "-- View: (" << renderer.getTarget().getView().getSize().x << ","
                                                  << renderer.getTarget().getView().getSize().y << ")";*/
        widget.doRender(renderer, newStates);
    });

    // Render self (overlay)
    applyStates(renderer);
    renderOverlay(renderer);
}

void Widget::addChild(SharedPtr<Widget> widget)
{
    BasicComponent::addChild(widget);
    DUMP(GUI_DEBUG, "addWidget");
    DUMP(GUI_DEBUG, widget.get());
    ASSERT(widget);
    widget->onCreate();

    // allow widgets know about window's size when creating
    sf::Vector2u wndSize = getWindow().getSize();
    sf::Event::SizeEvent event{wndSize.x, wndSize.y};
    widget->onResize(event);

    setGeometryNeedUpdate();
}

void Widget::removeWidget(Widget& widget)
{
    if(widget.hasFocus())
        clearFocus();
    removeChild(widget);
    setGeometryNeedUpdate();
}

void Widget::updateLayout()
{
    forEachChild([](auto& widget)->void {
        if(widget.geometryNeedUpdate())
            widget.updateLayout();
    });
}

void Widget::setFocusIndex(size_t index)
{
    ASSERT(index < childrenCount());
    clearFocus();

    m_focusedWidget = index;
    auto widget = getFocusedWidget();
    ASSERT(widget);
    widget->setFocus();
    widget->onGainFocus();
}

void Widget::clearFocus()
{
    if(m_focusedWidget != -1)
    {
        auto widget = getFocusedWidget();
        ASSERT(widget);
        widget->setFocus(false);
        widget->onLossFocus();
        m_focusedWidget = -1;
    }
}

void Widget::setFocus(Widget& widget)
{
    clearFocus();

    size_t index = 0;
    forEachChild([&](auto& other) {
        if(&other == &widget)
        {
            widget.setFocus(true);
            m_focusedWidget = index;
            return;
        }
        index++;
    });
}

void Widget::setFocus(bool value)
{
    m_hasFocus = value;
    if(!value)
    {
        forEachChild([&](auto& widget) {
            widget.setFocus(value);
        });
    }
}

bool Widget::shouldFireEventForChild(Widget const& widget, Event const& event) const
{
    if(widget.isHidden())
        return false;

    auto systemEvent = dynamic_cast<SystemEvent const*>(&event);
    if(systemEvent)
    {
        auto type = systemEvent->getEvent().type;
        if(type == sf::Event::MouseMoved || type == sf::Event::MouseButtonPressed)
        {
            // These are handled separately
            // TODO: Add mouse scroll to this list
            return false;
        }
    }
    return true;
}

}
