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
#include <ege/gfx/Renderer.h>
#include <ege/gui/Widget.h>
#include <ege/util/Types.h>

#define EGE_MODAL_DIALOG_REPLACED -8

namespace EGE
{

class Window;

class GUIScreen : public Widget
{
public:
    GUIScreen(const GUIScreen&) = delete;

    explicit GUIScreen(Window& window, String id = "GUIScreen (root)")
    : Widget(window, id) {}

    // for modal dialogs
    explicit GUIScreen(GUIScreen& parent, String id = "GUIScreen")
    : Widget(parent, id) {}

    // System Events
    virtual void onResize(sf::Event::SizeEvent&) override;

    virtual void onUnload() {}

    // Args: dialog, exitCode
    virtual void onDialogExit(GUIScreen*, int) {}

    virtual void doRender(Renderer& renderer, const RenderStates& states = {}) override;

    // If you call it on dialog, the onDialogExit function is called
    // and the dialog is removed on parent.
    virtual void exitDialog(int code);

    // Opened modal dialog should have 'this' as parent.
    // e.g   openDialog(make<MyDialog>(this));
    virtual void openDialog(SharedPtr<GUIScreen> dialog);

    virtual SharedPtr<GUIScreen> getDialog() { return m_dialog; }

protected:
    SharedPtr<GUIScreen> m_dialog;
};

}
