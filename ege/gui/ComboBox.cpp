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

#include "ComboBox.h"

#include "Button.h"

namespace EGE
{

ComboBox::ComboBox(Widget& parent, String id)
: CompoundWidget(parent, id)
{
    layoutDirection = LayoutElement::Direction::Vertical;
    auto layout = addNewWidget<CompoundWidget>("CBSel");
    layout->setSize({"1N", "24px"});
    layout->setPadding({1, 1});
    {
        m_cbSel = layout->addNewWidget<Label>();

        auto expand = layout->addNewWidget<Button>("CBExpandBtn");
        expand->align.x = LayoutAlign::Right;
        expand->setSize({"20px", "1N"});
        expand->events<Button::ClickEvent>().add([this](Button::ClickEvent&) {
            m_listBox->hide(!m_listBox->isHidden());
            return EventResult::Success;
        });
    }
    m_listBox = addNewWidget<ListBox>("CBList");
    m_listBox->hide();
    m_listBox->events<ListBox::SelectEvent>().add([this](ListBox::SelectEvent& event) {
        ege_log.info() << "ListBox::SelectEvent ()";
        m_listBox->hide();
        m_cbSel->setString(m_listBox->selection());

        // Pass the event.
        fire(event);

        return EventResult::Success;
    });
}

void ComboBox::render(Renderer& renderer) const
{
    renderer.renderTextBoxLikeBackground(0, 0, getSize().x, 24);
    renderer.renderTextBoxLikeBorder(0, 0, getSize().x, 24);
}

}

