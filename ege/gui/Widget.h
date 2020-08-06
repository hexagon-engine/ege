/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <SFML/System.hpp>
#include <ege/syswindow/DefaultSystemEventHandler.h>

#define WIDGET_DEBUG 1

namespace EGE
{

class GUIGameLoop;

class Widget : public DefaultSystemEventHandler
{
public:
    Widget(Widget* parent)
    : DefaultSystemEventHandler(parent->getWindow())
    , m_parent(parent) {}

    // for non-parented widgets, e.g. GUIScreen
    Widget(GUIGameLoop* gameLoop);

    void setPosition(sf::Vector2f position)
    {
        m_position = position;
    }

    virtual sf::FloatRect getBoundingBox();
    virtual void render(sf::RenderTarget& target);

    virtual void onUpdate(long long tickCounter) {}
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event) override;
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event) override;
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event) override;

    virtual bool isMouseOver(sf::Vector2f position);

protected:
    sf::Vector2f m_size;
    Widget* m_parent;
    bool m_mouseOver = false;
    bool m_leftClicked = false;

private:
    sf::Vector2f m_position;
};

// TODO: move it to another file
class DummyWidget : public Widget
{
public:
    DummyWidget(Widget* parent)
    : Widget(parent) {}

    void setSize(sf::Vector2f size)
    {
        m_size = size;
    }

};

}
