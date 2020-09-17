/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Animatable.h"
#include "Animation.h"

#include <SFML/System.hpp>
#include <ege/event/DefaultSystemEventHandler.h>

#define WIDGET_DEBUG 1

namespace EGE
{

class GUIGameLoop;

class Widget : public Animatable, public DefaultSystemEventHandler
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

    Widget(Widget* parent)
    : DefaultSystemEventHandler(parent ? parent->getWindow() : std::weak_ptr<SFMLSystemWindow>())
    , m_parent(parent)
    , m_gameLoop(parent ? parent->m_gameLoop : nullptr) {}

    // for non-parented widgets, e.g. GUIScreen
    Widget(GUIGameLoop* gameLoop);

    virtual void setPosition(sf::Vector2f position)
    {
        m_position = position;
    }
    sf::Vector2f getPosition() const
    {
        return m_position;
    }
    GUIGameLoop* getLoop() const
    {
        return m_gameLoop;
    }
    virtual void setSize(sf::Vector2f size)
    {
        m_size = size;
    }
    sf::Vector2f getSize() const
    {
        return m_size;
    }

    virtual sf::FloatRect getBoundingBox();
    virtual sf::FloatRect getViewport(sf::RenderTarget& target);
    virtual void render(sf::RenderTarget& target);

    // render without setting view
    virtual void renderOnly(sf::RenderTarget& target);

    virtual void onUpdate(long long tickCounter);
    virtual void onLoad() {};
    virtual void onCommand(const Command&) {}
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event) override;
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event) override;
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event) override;
    virtual void onLossFocus() override;
    virtual void onGainFocus() override;

    virtual bool isMouseOver(sf::Vector2f position);
    virtual void setViewForWidget(sf::RenderTarget& target);

    bool hasFocus()
    {
        return m_hasFocus;
    }

protected:
    sf::Vector2f m_size;
    Widget* m_parent;
    bool m_mouseOver = false;
    bool m_leftClicked = false;
    GUIGameLoop* m_gameLoop = nullptr;

private:
    sf::Vector2f m_position;
    bool m_hasFocus = false;
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
