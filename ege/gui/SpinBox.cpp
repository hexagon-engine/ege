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

#include "SpinBox.h"

#include "Button.h"

#include <ege/debug/Logger.h>

namespace EGE
{

SpinBox::SpinBox(Widget& parent, String id)
: CompoundWidget(parent, id)
{
    layoutDirection = LayoutElement::Direction::Horizontal;

    m_textBox = addNewWidget<TextBox>("SBoxVal");
    m_textBox->setBorder(false);
    m_textBox->setText("0");

    m_textBox->events<SubmitEvent>().add([this](SubmitEvent& event) {
        try
        {
            m_value = std::stoi(event.value);
            m_textBox->setText(std::to_string(m_value));
        }
        catch(...)
        {
            m_value = 0;
            m_textBox->setText(std::to_string(m_value));
        }
        return EventResult::Success;
    });

    auto buttons = addNewWidget<CompoundWidget>("SBoxBtns");
    buttons->setSize({"20px", "1N"});
    buttons->setPadding({0, 1});
    buttons->layoutDirection = LayoutElement::Direction::Vertical;
    {
        auto buttonUp = buttons->addNewWidget<Button>("SBoxUp");
        buttonUp->align.x = LayoutAlign::Right;
        buttonUp->setLabel("");
        buttonUp->events<Button::ClickEvent>().add([this](Button::ClickEvent&) {
            ege_log.debug() << "Up";
            m_textBox->fire<SubmitEvent>(*m_textBox, m_textBox->getText());
            m_value += 1;
            m_textBox->setText(std::to_string(m_value));
            return EventResult::Success;
        });

        auto buttonDown = buttons->addNewWidget<Button>("SBoxDown");
        buttonDown->align.x = LayoutAlign::Right;
        buttonDown->setLabel("");
        buttonDown->events<Button::ClickEvent>().add([this](Button::ClickEvent&) {
            ege_log.debug() << "Down";
            m_textBox->fire<SubmitEvent>(*m_textBox, m_textBox->getText());
            m_value -= 1;
            m_textBox->setText(std::to_string(m_value));
            return EventResult::Success;
        });
    }
}

void SpinBox::render(Renderer& renderer) const
{
    renderer.renderTextBoxLikeBorder(0, 0, getSize().x, getSize().y);
}

}

