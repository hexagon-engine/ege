/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "TextBox.h"

#include "Animation.h"
#include "AnimationEasingFunctions.h"
#include "GUIGameLoop.h"

#include <ege/debug/Logger.h>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <iomanip>
#include <iostream>

namespace EGE
{

TextBox::TextBox(Widget* parent)
: Widget(parent)
{
    m_caretAnimation = make<Animation>(this, Time(1.0, Time::Unit::Seconds), Timer::Mode::Infinite);
    m_caretAnimation->addKeyframe(0.0, 1.0);
    m_caretAnimation->addKeyframe(0.5, 0.0);
    m_caretAnimation->addKeyframe(1.0, 1.0);
    m_caretAnimation->setEasingFunction(AnimationEasingFunctions::constant1);
    addAnimation(m_caretAnimation, [this](Animation*, double val) {
                    m_caretShown = (val == 0.0);
                 });
}

void TextBox::renderOnly(sf::RenderTarget& target, const RenderStates&)
{
    sf::RectangleShape rs;
    rs.setFillColor(sf::Color(255, 255, 255));
    rs.setOutlineThickness(1.f);

    // background
    rs.setPosition(2.f, 2.f);
    rs.setOutlineColor(sf::Color(60, 60, 60));
    rs.setSize(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(4.f, 4.f));
    target.draw(rs);

    // label
    auto font = m_parent->getLoop()->getResourceManager().lock()->getDefaultFont();
    ASSERT(font);
    sf::Text text(m_text, *font, m_size.y / 2.f);
    text.setFillColor(sf::Color::Black);
    float overflow = std::max(0.0, (text.findCharacterPos(m_caretPos).x) - (m_size.x - 20.f));
    text.setPosition(10.f - overflow, m_size.y / 4.f);
    target.draw(text);

    // caret
    if(hasFocus() && m_caretShown)
    {
        sf::RectangleShape rsCaret(sf::Vector2f(1.f, m_size.y - 6.f));
        rsCaret.setPosition(text.findCharacterPos(m_caretPos).x, 3.f);
        rsCaret.setFillColor(sf::Color::Black);
        target.draw(rsCaret);
    }

    // border
    rs.setSize(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(3.f, 3.f));
    rs.setPosition(1.f, 1.f);
    rs.setFillColor(sf::Color::Transparent);
    rs.setOutlineColor(sf::Color(173, 173, 173));
    target.draw(rs);

    rs.setSize(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(2.f, 2.f));
    rs.setPosition(1.f, 1.f);
    rs.setOutlineColor(sf::Color(210, 210, 210));
    target.draw(rs);
}

void TextBox::onMouseEnter()
{
    Widget::onMouseEnter();
    auto cursor = getLoop()->getResourceManager().lock()->getCursor(sf::Cursor::Text);
    ASSERT(cursor);
    getWindow().lock()->setMouseCursor(*cursor);
}

void TextBox::onMouseLeave()
{
    Widget::onMouseLeave();
    auto cursor = getLoop()->getResourceManager().lock()->getCursor(sf::Cursor::Arrow);
    ASSERT(cursor);
    getWindow().lock()->setMouseCursor(*cursor);
}

void TextBox::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    Widget::onMouseButtonPress(event);
    if(event.button == sf::Mouse::Left)
        m_caretAnimation->restart();

    // Find character to set caret next to.
    auto font = m_parent->getLoop()->getResourceManager().lock()->getDefaultFont();
    ASSERT(font);
    sf::Text text(m_text, *font, m_size.y / 2.f);
    text.setFillColor(sf::Color::Black);
    float overflow = std::max(0.0, (text.findCharacterPos(m_caretPos).x) - (m_size.x - 20.f));
    text.setPosition(10.f - overflow, m_size.y / 4.f);

    for(size_t s = 0; s <= m_text.getSize(); s++)
    {
        if(text.findCharacterPos(s).x < event.x - getPosition().x + text.getCharacterSize() / 3.91f)
        {
            m_caretPos = s;
        }
    }
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
                    err(LogLevel::Debug) << "New line";
                } break;
            }
        }
        else
        {
            m_text.insert(m_caretPos, chr);
            m_caretPos++;
        }
    }
    m_caretAnimation->restart();
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
    m_caretAnimation->restart();
}

}
