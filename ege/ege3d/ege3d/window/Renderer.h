/*
    EGE3d - 3D rendering engine for Hexagon

    Copyright (c) 2020 Hexagon Engine

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once

#include <ege3d/window/Window.h>
#include <ege/util/Rect.h>

#include "RenderingState.h"

namespace EGE3d
{

class Renderer
{
public:
    // TODO: Make some rendertarget stuff
    explicit Renderer(RenderingState const& state) : m_state(state) { m_state.flush(); }

    Window& target() const { return m_state.target(); }

    // OpenGL Wrappers
    void glViewport(EGE::RectI rect);

    enum class MatrixMode
    {
        Projection,
        Modelview
    };

    void glMatrixMode(MatrixMode mode);
    void glLoadIdentity();

    bool glIsError() const;

    RenderingState& state() { return m_state; }

private:
    void ensureIsCurrent() const;

    RenderingState m_state;
};

}
