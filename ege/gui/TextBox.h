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

#include "ege/debug/InspectorNode.h"
#include <ege/core/Event.h>
#include <ege/gfx/RenderStates.h>
#include <ege/gui/Widget.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

// Called when pressing 'enter' and widget is focused.
class SubmitEvent : public Event
{
public:
    EGE_EVENT("EGE::SubmitEvent");

    SubmitEvent(Widget& _widget, String _value)
    : widget(_widget), value(_value) {}

    Widget& widget;
    String value;
};

class TextBox : public Widget
{
public:
    explicit TextBox(Widget& parent, String id = "TextBox");

    void setText(sf::String text)
    {
        m_text = text;
        if(m_caretPos > m_text.getSize())
            m_caretPos = m_text.getSize();
        setGeometryNeedUpdate();
    }

    sf::String getText()
    {
        return m_text;
    }

    void setBorder(bool border = true) { m_border = border; }

    virtual void onMouseEnter() override;
    virtual void onMouseLeave() override;
    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event) override;
    virtual void onMouseMove(sf::Event::MouseMoveEvent& event) override;
    virtual void onTextEnter(sf::Event::TextEvent& event) override;
    virtual void onKeyPress(sf::Event::KeyEvent& event) override;

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
    bool m_border = true;
    SharedPtr<NumberAnimation> m_caretAnimation;
};

}
