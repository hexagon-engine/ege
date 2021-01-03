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

TextBox::TextBox(Widget& parent, String id)
: Widget(parent, id)
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

void TextBox::render(Renderer& renderer) const
{
    sf::RectangleShape rs;
    rs.setFillColor(sf::Color(255, 255, 255));
    rs.setOutlineThickness(1.f);

    // background
    renderer.renderTextBoxLikeBackground(0, 0, getSize().x, getSize().y);

    // label
    renderer.getTarget().draw(m_textDrawable);

    // selection
    float selStart = m_textDrawable.findCharacterPos(m_selectionStart).x;
    float selEnd = m_textDrawable.findCharacterPos(m_selectionEnd).x - selStart;
    renderer.renderRectangle(selStart, 3.f, selEnd, getSize().y - 6.f, sf::Color(17, 168, 219, 127));

    // caret
    if(hasFocus() && m_caretShown)
    {
        sf::RectangleShape rsCaret(sf::Vector2f(1.f, getSize().y - 6.f));
        rsCaret.setPosition(m_textDrawable.findCharacterPos(m_caretPos).x, 3.f);
        rsCaret.setFillColor(sf::Color::Black);
        renderer.getTarget().draw(rsCaret);
    }

    // border
    renderer.renderTextBoxLikeBorder(0, 0, getSize().x, getSize().y);

    Widget::render(renderer);
}

void TextBox::onMouseEnter()
{
    Widget::onMouseEnter();
    auto cursor = getLoop().getResourceManager()->getCursor(sf::Cursor::Text);
    ASSERT(cursor);
    getWindow().setMouseCursor(*cursor);
}

void TextBox::onMouseLeave()
{
    Widget::onMouseLeave();
    auto cursor = getLoop().getResourceManager()->getCursor(sf::Cursor::Arrow);
    ASSERT(cursor);
    getWindow().setMouseCursor(*cursor);
}

void TextBox::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    Widget::onMouseButtonPress(event);
    if(event.button == sf::Mouse::Left)
        m_caretAnimation->restart();

    // Find character to set caret next to.
    // TODO: Optimize it!
    doUpdateGeometry(getLoop().getRenderer());
    for(size_t s = 0; s <= m_text.getSize(); s++)
    {
        if(m_textDrawable.findCharacterPos(s).x < event.x - getPosition().x + m_textDrawable.getCharacterSize() / 3.91f)
        {
            m_caretPos = s;
            m_selectionStart = s;
            m_selectionEnd = s;
        }
    }
    setGeometryNeedUpdate();
}

void TextBox::onMouseMove(sf::Event::MouseMoveEvent& event)
{
    Widget::onMouseMove(event);

    if(m_leftClicked)
    {
        // TODO: Optimize it!
        doUpdateGeometry(getLoop().getRenderer());
        for(size_t s = 0; s <= m_text.getSize(); s++)
        {
            if(m_textDrawable.findCharacterPos(s).x < event.x - getPosition().x + m_textDrawable.getCharacterSize() / 3.91f)
            {
                m_caretPos = s;
                m_selectionEnd = s;
            }
        }
        setGeometryNeedUpdate();
    }
}

void TextBox::onTextEnter(sf::Event::TextEvent& event)
{
    if(hasFocus())
    {
        unsigned chr = event.unicode;
        if(m_selectionEnd - m_selectionStart != 0)
        {
            size_t selMin = std::min(m_selectionStart, m_selectionEnd);
            size_t selMax = std::max(m_selectionStart, m_selectionEnd);
            m_text.erase(selMin, selMax - selMin);

            // Move caret
            m_caretPos = selMin;

            bool isControl = chr < 0x20 || chr == 0x7f;

            // Replace selection with typed text !
            if(isControl)
            {
                switch(chr)
                {
                case '\b': // Backspace
                case 0x7f: // DEL
                    break;
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

            clearSelection();
            setGeometryNeedUpdate();
        }
        else
        {
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
                            setGeometryNeedUpdate();
                        }
                    } break;
                case 0x7f: // DEL
                    {
                        if(m_caretPos < m_text.getSize())
                        {
                            m_text.erase(m_caretPos);
                            setGeometryNeedUpdate();
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
                setGeometryNeedUpdate();
            }
        }
        m_caretAnimation->restart();
    }
}

void TextBox::onKeyPress(sf::Event::KeyEvent& event)
{
    switch(event.code)
    {
    case sf::Keyboard::Left:
        if(m_caretPos > 0)
        {
            if(event.shift)
            {
                if(m_selectionEnd == m_selectionStart)
                    m_selectionStart = m_caretPos;
                m_caretPos--;
                m_selectionEnd = m_caretPos;
            }
            else
            {
                if(m_selectionEnd == m_selectionStart)
                    m_caretPos--;
                else
                    clearSelection();
            }
            setGeometryNeedUpdate();
        }
        break;
    case sf::Keyboard::Right:
        if(m_caretPos + 1 <= m_text.getSize())
        {
            if(event.shift)
            {
                if(m_selectionEnd == m_selectionStart)
                    m_selectionStart = m_caretPos;
                m_caretPos++;
                m_selectionEnd = m_caretPos;
            }
            else
            {
                if(m_selectionEnd == m_selectionStart)
                    m_caretPos++;
                else
                    clearSelection();
            }
            setGeometryNeedUpdate();
        }
        break;
    default:
        break;
    }
    m_caretAnimation->restart();
}

void TextBox::generateText()
{
    auto font = getLoop().getResourceManager()->getDefaultFont();
    ASSERT(font);
    sf::Text text(m_text, *font, getSize().y / 2.f);
    text.setFillColor(sf::Color::Black);
    float overflow = std::max(0.0, (text.findCharacterPos(m_caretPos).x) - (getSize().x - 20.f));
    text.setPosition(10.f - overflow, getSize().y / 4.f);
    m_textDrawable = text;
}

void TextBox::updateGeometry(Renderer& renderer)
{
    Widget::updateGeometry(renderer);
    generateText();
}

void TextBox::clearSelection()
{
    m_selectionStart = 0;
    m_selectionEnd = 0;
    setGeometryNeedUpdate();
}

}
