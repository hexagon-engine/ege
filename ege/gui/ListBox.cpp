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

#include "ListBox.h"

#include "Label.h"

#include <ege/debug/Logger.h>

namespace EGE
{

ListBox::ListBox(Widget& parent, String id)
: Widget(parent, id)
{
    setPadding({"2px", "2px"});

    m_scrollbar = addNewWidget<ScrollBar>("ListBoxSB");
    m_scrollbar->setType(ScrollBar::Type::Vertical);
    m_scrollbar->setLength(LayoutSizeD("0N"));
    m_scrollbar->align.x = LayoutAlign::Right;
    m_scrollbar->setUpdateCallback([this](double val) {

        // Y position is hardcoded to allow scrolling!
        double offset = getSize().y * val;

        ege_log.debug() << "LB: val=" << val << " off=" << offset;

        m_entries->setPosition(LVec2d({0, EGE_LAYOUT_PIXELS}, {-offset, EGE_LAYOUT_PIXELS}));
        setGeometryNeedUpdate();
    });

    m_entries = addNewWidget<Widget>("ListBoxList");
    m_entries->setPadding({"2px", "0px"});
    m_entries->layoutDirection = LayoutElement::Direction::Vertical;
    m_entries->setSize({"0N", "0a"});
}

void ListBox::onKeyPress(sf::Event::KeyEvent& event)
{
    Widget::onKeyPress(event);
    if(!hasFocus() || m_entries->getFocusedWidgetIndex() == -1)
        return;

    // TODO: Mouse scrolling!

    if(event.code == sf::Keyboard::Up)
        scrollBy(-1, true);
    else if(event.code == sf::Keyboard::Down)
        scrollBy(1, true);
}

void ListBox::scrollBy(int direction, bool changeFocus)
{
    if(!m_mouseOver && !changeFocus)
        return;

    if(direction == 1)
    {
        if(changeFocus && m_entries->getFocusedWidgetIndex() < (int)m_entries->childrenCount() - 1)
        {
            m_entries->setFocusIndex(m_entries->getFocusedWidgetIndex() + 1);

            auto focused = m_entries->getFocusedWidget();
            double fpos = focused->getPosition().y, epos = m_entries->getPosition().y;
            double esize = m_entries->getSize().y;
            ege_log.debug() << fpos << ", " << epos << " -> " << fpos + epos << " (sz=" << esize << ")";
            if(fpos + epos > getSize().y - 20)
                m_scrollbar->scrollToPosition((fpos - getSize().y + 20) * ((esize - 6) / esize));
        }
        else
        {
            m_scrollbar->scrollToPosition(m_scrollbar->getScrollPosition() + 16);
        }
    }
    else if(direction == -1)
    {
        if(changeFocus && m_entries->getFocusedWidgetIndex() > 0)
        {
            m_entries->setFocusIndex(m_entries->getFocusedWidgetIndex() - 1);

            auto focused = m_entries->getFocusedWidget();
            double fpos = focused->getPosition().y, epos = m_entries->getPosition().y;
            double esize = m_entries->getSize().y;
            ege_log.debug() << fpos << ", " << epos << " -> " << fpos + epos << " (sz=" << esize << ")";
            if(fpos + epos < 0)
                m_scrollbar->scrollToPosition(fpos * ((esize - 6) / esize));
        }
        else
        {
            m_scrollbar->scrollToPosition(m_scrollbar->getScrollPosition() - 16);
        }
    }
}

void ListBox::onMouseButtonRelease(sf::Event::MouseButtonEvent& event)
{
    // Call upper to set focus
    Widget::onMouseButtonRelease(event);

    // Fire event if any entry is focused.
    if(m_mouseOver)
    {
        int i = m_entries->getFocusedWidgetIndex();
        if(i != -1 && getFocusedWidget()->getId() == "ListBoxList")
        {
            ege_log.debug() << "Firing SelectEvent for " << i << " (T[" << i << "] == " << getFocusedWidget()->getId() << ")";
            fire<SelectEvent>(*this, i, selection());
        }
    }
}

void ListBox::addEntry(String value)
{
    auto widget = m_entries->addNewWidget<Label>(value, "LBValue");
    widget->setSize({"0N", "20px"});
    setGeometryNeedUpdate();
}

void ListBox::onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event)
{
    Widget::onMouseWheelScroll(event);
    if(event.delta > 0)
        scrollBy(-1, false);
    else
        scrollBy(1, false);
}

void ListBox::render(Renderer& renderer) const
{
    // Background
    renderer.getThemeRenderer()->renderTextBoxLikeBackground(renderer, 0, 0, getSize().x - m_scrollbar->getSize().x + 1, getSize().y);
}

sf::String ListBox::selection() const
{
    auto label = dynamic_cast<Label*>(m_entries->getFocusedWidget());
    return label ? label->getString() : "";
}

void ListBox::renderOverlay(Renderer& renderer) const
{
    // Selected entry overlay
    auto selected = m_entries->getFocusedWidget();
    if(selected)
    {
        auto bounds = selected->getBoundingBox();
        auto abs = getAbsolutePosition();
        auto pos = bounds.getPosition() - sf::Vector2f((float)abs.x, (float)abs.y);
        renderer.renderRectangle(pos.x, pos.y, bounds.width, bounds.height, ColorRGBA::fromBytes(17, 168, 219, 127));
    }

    // Border2
    renderer.getThemeRenderer()->renderTextBoxLikeBorder(renderer, 0, 0, getSize().x, getSize().y);
}

void ListBox::updateGeometry(Renderer& renderer)
{
    Widget::updateGeometry(renderer);
    if(m_entries->getSize().y > 0)
    {
        ege_log.debug() << "LB: Set max val!";
        double maxVal = (m_entries->getSize().y + 4) / getSize().y;
        m_scrollbar->setMaxValue(maxVal);
    }
}

}

