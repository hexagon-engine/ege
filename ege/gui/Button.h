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

#include <ege/gfx/RenderStates.h>
#include <ege/gui/Widget.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class Button : public Widget
{
public:
    class Command : public Widget::Command
    {
    public:
        Command(Button* button)
        : Widget::Command("EGE::Button::Command"), m_button(button)
        {}

        Button* getButton() const
        {
            return m_button;
        }
    private:
        Button* m_button;
    };

    explicit Button(Widget& parent)
    : Widget(parent) {}

    virtual void setLabel(sf::String label)
    {
        m_label = label;
    }

    sf::String getLabel() const
    {
        return m_label;
    }

    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event);
    virtual void onTouchEnd(sf::Event::TouchEvent& event) override;

    void setCallback(std::function<void()> callback)
    {
        m_callback = callback;
    }

protected:
    virtual void render(Renderer& renderer) const override;

    virtual void onClick(EGE::Vec2d position);
    std::function<void()> m_callback;

private:
    // position may be needed for some animations
    void handleClick(EGE::Vec2d position);

    EGE::Vec2d m_lastClickPos;
    sf::String m_label;
};

}
