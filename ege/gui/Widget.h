/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Animatable.h"
#include "Animation.h"

#include <ege/event/DefaultSystemEventHandler.h>
#include <ege/gfx/RenderStates.h>
#include <ege/util/Vector.h>

#define WIDGET_DEBUG 0

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

    virtual void setPosition(EGE::Vec2d position)
    {
        m_position = position;
    }
    EGE::Vec2d getPosition() const
    {
        return m_position;
    }
    GUIGameLoop* getLoop() const
    {
        return m_gameLoop;
    }
    virtual void setSize(EGE::Vec2d size)
    {
        m_size = size;
    }
    EGE::Vec2d getSize() const
    {
        return m_size;
    }
    Widget* getParent() { return m_parent; }

    virtual sf::FloatRect getBoundingBox();
    virtual sf::FloatRect getViewport(sf::RenderTarget& target);
    virtual void render(sf::RenderTarget& target, const RenderStates& states = {});

    // render without setting view
    virtual void renderOnly(sf::RenderTarget& target, const RenderStates& states = {});

    virtual void onUpdate(long long tickCounter);
    virtual void onLoad() {};
    virtual void onCommand(const Command&) {}
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event) override;
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event) override;
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event) override;
    virtual void onLossFocus() override;
    virtual void onGainFocus() override;

    virtual bool isMouseOver(EGE::Vec2d position);
    virtual void setViewForWidget(sf::RenderTarget& target);

    virtual sf::View getView(sf::RenderTarget& target);

    bool hasFocus()
    {
        return m_hasFocus;
    }

protected:
    EGE::Vec2d m_size;
    Widget* m_parent;
    bool m_mouseOver = false;
    bool m_leftClicked = false;
    GUIGameLoop* m_gameLoop = nullptr;

private:
    EGE::Vec2d m_position;
    bool m_hasFocus = false;
};

// TODO: move it to another file
class DummyWidget : public Widget
{
public:
    DummyWidget(Widget* parent)
    : Widget(parent) {}

    void setSize(EGE::Vec2d size)
    {
        m_size = size;
    }

};

}
