/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/gfx/RenderStates.h>
#include <ege/gui/Widget.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class TextBox : public Widget
{
public:
    explicit TextBox(Widget& parent);

    void setText(sf::String text)
    {
        m_text = text;
    }

    sf::String getText()
    {
        return m_text;
    }

    virtual void onMouseEnter();
    virtual void onMouseLeave();
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event);
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event);
    virtual void onTextEnter(sf::Event::TextEvent& event);
    virtual void onKeyPress(sf::Event::KeyEvent& event);

protected:
    virtual void render(Renderer& renderer) const override;
    virtual void updateGeometry(Renderer& renderer) override;

private:
    virtual void generateText();
    virtual void clearSelection();

    sf::String m_text;
    sf::Text m_textDrawable;
    size_t m_caretPos = 0;
    size_t m_selectionStart = 0;
    size_t m_selectionEnd = 0;
    bool m_caretShown = true;
    std::shared_ptr<Animation> m_caretAnimation;
};

}
