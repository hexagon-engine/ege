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

#include <ege/debug/Logger.h>

namespace EGE
{

ListBox::ListBox(Widget& parent, String id)
: CompoundWidget(parent, id)
{
    setPadding({"2px", "2px"});

    m_scrollbar = addNewWidget<ScrollBar>("ListBoxSB");
    m_scrollbar->setType(ScrollBar::Type::Vertical);
    m_scrollbar->setLength(LayoutSizeD("0N"));
    m_scrollbar->align.x = LayoutAlign::Right;
    m_scrollbar->setUpdateCallback([this](double val) {

        // Y position is hardcoded to allow scrolling!
        double offset = getSize().y * val;

        log() << "LB: val=" << val << " off=" << offset;

        m_entries->setPosition(LVec2d({0, EGE_LAYOUT_PIXELS}, {-offset, EGE_LAYOUT_PIXELS}));
        setGeometryNeedUpdate();
    });

    m_entries = addNewWidget<CompoundWidget>("ListBoxList");
    m_entries->layoutDirection = LayoutElement::Direction::Vertical;
    m_entries->setSize({"0N", "0a"});
}

void ListBox::onKeyPress(sf::Event::KeyEvent& event)
{
    CompoundWidget::onKeyPress(event);
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
    log() << m_mouseOver;

    if(!m_mouseOver && !changeFocus)
        return;

    if(direction == 1)
    {
        if(changeFocus && m_entries->getFocusedWidgetIndex() < (int)m_entries->getWidgetCount() - 1)
        {
            m_entries->setFocusIndex(m_entries->getFocusedWidgetIndex() + 1);

            auto focused = m_entries->getFocusedWidget();
            double fpos = focused->getPosition().y, epos = m_entries->getPosition().y;
            double esize = m_entries->getSize().y;
            log() << fpos << ", " << epos << " -> " << fpos + epos << " (sz=" << esize << ")";
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
            log() << fpos << ", " << epos << " -> " << fpos + epos << " (sz=" << esize << ")";
            if(fpos + epos < 0)
                m_scrollbar->scrollToPosition(fpos * ((esize - 6) / esize));
        }
        else
        {
            m_scrollbar->scrollToPosition(m_scrollbar->getScrollPosition() - 16);
        }
    }
}

void ListBox::onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event)
{
    CompoundWidget::onMouseWheelScroll(event);
    if(event.delta > 0)
        scrollBy(-1, false);
    else
        scrollBy(1, false);
}

void ListBox::render(Renderer& renderer) const
{
    // Background
    renderer.renderTextBoxLikeBackground(0, 0, getSize().x - m_scrollbar->getSize().x + 1, getSize().y);
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
    renderer.renderTextBoxLikeBorder(0, 0, getSize().x, getSize().y);
}

void ListBox::updateGeometry(Renderer& renderer)
{
    Widget::updateGeometry(renderer);
    if(m_entries->getSize().y > 0)
    {
        log() << "LB: Set max val!";
        double maxVal = (m_entries->getSize().y + 4) / getSize().y;
        m_scrollbar->setMaxValue(maxVal);
    }
}

}

