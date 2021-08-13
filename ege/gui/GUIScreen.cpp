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

#include "GUIScreen.h"

#include "GUIGameLoop.h"

#include <ege/debug/Logger.h>

namespace EGE
{

void GUIScreen::onResize(sf::Event::SizeEvent& event)
{
    // Resize GUIScreen to match window
    setSize({static_cast<double>(event.width), static_cast<double>(event.height)});
}

void GUIScreen::doRender(Renderer& renderer, const RenderStates& states)
{
    // Update layout
    updateLayout();

    // Ensure layout is calculated!
    // FIXME: it's a bit hacky, it should be automatically updated
    // in updateGeometry()
    if(geometryNeedUpdate())
    {
        runLayoutUpdate();
        ege_log.debug() << "Resulting layout: size(" << getSize().x << "," << getSize().y << ")";
    }

    // Actually draw child widgets
    auto newStates = states;
    applyStates(renderer, newStates);
    Widget::doRender(renderer, newStates);

    // Render dialog on top of GUI.
    if(m_dialog)
    {
        m_dialog->doRender(renderer, newStates);
    }
}

void GUIScreen::exitDialog(int code)
{
    getParent<GUIScreen>()->deferredInvoke([this, code] {
        GUIScreen* parent = getParent<GUIScreen>();
        ASSERT(parent);
        parent->onDialogExit(this, code);
        onUnload();
        removeChild(*parent->m_dialog);
        parent->m_dialog = nullptr;
    });
}

void GUIScreen::openDialog(SharedPtr<GUIScreen> dialog)
{
    // We use here a property of deferredInvokes
    // - they have guaranteed the order of calling
    if(m_dialog)
        m_dialog->exitDialog(EGE_MODAL_DIALOG_REPLACED);

    deferredInvoke([this, dialog] {
        m_dialog = dialog;
        m_dialog->onCreate();
        addChild(m_dialog);

        sf::Vector2u wndSize = getWindow().getSize();
        sf::Event::SizeEvent event{wndSize.x, wndSize.y};
        m_dialog->onResize(event);
    });
}

}
