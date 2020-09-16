/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "TextBox.h"

#include "Animation.h"
#include "AnimationEasingFunctions.h"
#include "GUIGameLoop.h"

#include <ege/main/Config.h>
#include <iomanip>
#include <iostream>

namespace EGE
{

TextBox::TextBox(Widget* parent)
: Widget(parent)
{
    auto anim = make<Animation>(this, Time(1.0, Time::Unit::Seconds), Timer::Mode::Infinite);
    anim->addKeyframe(0.0, 1.0);
    anim->addKeyframe(0.5, 0.0);
    anim->addKeyframe(1.0, 1.0);
    anim->setEasingFunction(AnimationEasingFunctions::constant1);
    addAnimation(anim, [this](Animation*, double val) {
                    m_caretShown = (val > 0.0);
                 });
}

void TextBox::renderOnly(sf::RenderTarget& target)
{
    sf::RectangleShape rs;
    rs.setFillColor(sf::Color(255, 255, 255));
    rs.setOutlineThickness(1.f);

    // background
    rs.setPosition(2.f, 2.f);
    rs.setOutlineColor(sf::Color(60, 60, 60));
    rs.setSize(m_size - sf::Vector2f(4.f, 4.f));
    target.draw(rs);

    // label
    auto font = m_parent->getLoop()->getResourceManager().lock()->getDefaultFont();
    ASSERT(font);
    sf::Text text(m_text, *font, m_size.y / 2.f);
    text.setFillColor(sf::Color::Black);
    float overflow = std::max(0.f, (text.findCharacterPos(m_caretPos).x) - (m_size.x - 20.f));
    text.setPosition(10.f - overflow, m_size.y / 4.f);
    target.draw(text);

    // caret
    if(hasFocus() && m_caretShown)
    {
        sf::RectangleShape rsCaret(sf::Vector2f(1.f, m_size.y - 10.f));
        rsCaret.setPosition(text.findCharacterPos(m_caretPos).x, 5.f);
        rsCaret.setFillColor(sf::Color::Black);
        target.draw(rsCaret);
    }

    // border
    rs.setSize(m_size - sf::Vector2f(3.f, 3.f));
    rs.setPosition(1.f, 1.f);
    rs.setFillColor(sf::Color::Transparent);
    rs.setOutlineColor(sf::Color(173, 173, 173));
    target.draw(rs);

    rs.setSize(m_size - sf::Vector2f(2.f, 2.f));
    rs.setPosition(1.f, 1.f);
    rs.setOutlineColor(sf::Color(210, 210, 210));
    target.draw(rs);
}

void TextBox::onTextEnter(sf::Event::TextEvent& event)
{
    if(hasFocus())
    {
        unsigned chr = event.unicode;
        if(chr < 0x20 || chr == 0x7f)
        {
            switch(chr)
            {
            case '\b': // Backspace
                {
                    if(!m_text.isEmpty() && m_caretPos > 0)
                    {
                        m_text.erase(m_caretPos - 1);
                        m_caretPos--;
                    }
                } break;
            case 0x7f: // DEL
                {
                    if(m_caretPos < m_text.getSize())
                    {
                        m_text.erase(m_caretPos);
                    }
                } break;
            case '\r':
                {
                    std::cerr << "New line" << std::endl;
                } break;
            }
        }
        else
        {
            m_text.insert(m_caretPos, chr);
            m_caretPos++;
        }
    }
}

void TextBox::onKeyPress(sf::Event::KeyEvent& event)
{
    switch(event.code)
    {
    case sf::Keyboard::Left:
        if(m_caretPos > 0)
            m_caretPos--;
        break;
    case sf::Keyboard::Right:
        if(m_caretPos + 1 <= m_text.getSize())
            m_caretPos++;
        break;
    default:
        break;
    }
}

}
