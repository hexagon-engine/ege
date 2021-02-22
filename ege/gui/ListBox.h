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

#include "CompoundWidget.h"
#include "ScrollBar.h"

#include <ege/util/Types.h>

namespace EGE
{

class ListBox : public CompoundWidget
{
public:
    class SelectEvent : public Event
    {
    public:
        EGE_EVENT("EGE::SelectEvent")

        SelectEvent(ListBox& _listbox, int _index, sf::String _value)
        : listbox(_listbox), index(_index), value(_value) {}

        ListBox& listbox;
        int index;
        sf::String value;
    };

    ListBox(Widget& parent, String id = "ListBox");

    virtual void onKeyPress(sf::Event::KeyEvent& event) override;
    virtual void onMouseButtonRelease(sf::Event::MouseButtonEvent& event) override;
    virtual void onMouseWheelScroll(sf::Event::MouseWheelScrollEvent& event) override;

    virtual void render(Renderer& renderer) const override;
    virtual void renderOverlay(Renderer& renderer) const override;

    void addEntry(String value);
    sf::String selection() const;

    virtual void updateGeometry(Renderer& renderer) override;

    void scrollBy(int direction, bool changeFocus);

private:
    void scrollToFocused();

    SharedPtr<ScrollBar> m_scrollbar;
    SharedPtr<CompoundWidget> m_entries;
    int m_selectedEntry = -1;
};

}
