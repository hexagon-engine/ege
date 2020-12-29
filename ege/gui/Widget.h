/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Animatable.h"
#include "Animation.h"

#include <ege/event/DefaultSystemEventHandler.h>
#include <ege/gfx/Renderable.h>
#include <ege/gfx/Renderer.h>
#include <ege/util/Vector.h>

#define WIDGET_DEBUG 0

namespace EGE
{

class GUIGameLoop;

class Widget : public Animatable, public DefaultSystemEventHandler, public Renderable
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

    explicit Widget(Widget& parent)
    : DefaultSystemEventHandler(parent.getWindow())
    , m_parent(&parent)
    , m_gameLoop(parent.m_gameLoop) {}

    // for non-parented widgets, e.g. GUIScreen
    explicit Widget(GUIGameLoop& gameLoop);

    virtual void setPosition(EGE::Vec2d position) { m_position = position; }
    EGE::Vec2d getPosition() const { return m_position; }

    GUIGameLoop& getLoop() const { return m_gameLoop; }

    virtual void setSize(EGE::Vec2d size) { m_size = size; }
    EGE::Vec2d getSize() const { return m_size; }

    Widget* getParent() { return m_parent; }

    virtual sf::FloatRect getBoundingBox();
    virtual sf::FloatRect getViewport(sf::RenderTarget& target) const;

    virtual void onUpdate(long long tickCounter);
    virtual void onLoad() {};
    virtual void onCommand(const Command&) {}
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event) override;
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event) override;
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event) override;
    virtual void onLossFocus() override;
    virtual void onGainFocus() override;

    virtual bool isMouseOver(EGE::Vec2d position);
    virtual sf::View getCustomView(sf::RenderTarget& target) const override;
    virtual bool isCustomViewNeeded() const { return true; }

    bool hasFocus() const { return m_hasFocus; }

protected:
    virtual void render(Renderer& renderer) const override;

    EGE::Vec2d m_size;
    Widget* m_parent;
    bool m_mouseOver = false;
    bool m_leftClicked = false;
    GUIGameLoop& m_gameLoop;

private:
    EGE::Vec2d m_position;
    bool m_hasFocus = false;
};

// TODO: move it to another file
class DummyWidget : public Widget
{
public:
    DummyWidget(Widget& parent)
    : Widget(parent) {}

    void setSize(EGE::Vec2d size)
    {
        m_size = size;
    }

};

}
