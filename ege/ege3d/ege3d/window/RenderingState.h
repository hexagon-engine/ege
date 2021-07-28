/*
    EGE3d - 3D rendering engine for Hexagon

    Copyright (c) 2021 Hexagon Engine

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

#include <ege/util/Types.h>
#include <ege/util/Matrix.h>
#include <ege/util/Rect.h>

namespace EGE3d
{

class Window;

class RenderingState
{
public:
    // TODO: do some rendertarget stuff
    RenderingState(Window& target)
    : m_target(target) {}

    void setViewport(EGE::RectI const& viewport) { m_viewport = viewport; }
    void setProjectionMatrix(EGE::DoubleMatrix4x4 const& matrix) { m_projectionMatrix = matrix; }
    void setModelviewMatrix(EGE::DoubleMatrix4x4 const& matrix) { m_modelviewMatrix = matrix; }

    void applyProjectionMatrix(EGE::DoubleMatrix4x4 const& matrix) { m_projectionMatrix *= matrix; }
    void applyModelviewMatrix(EGE::DoubleMatrix4x4 const& matrix) { m_modelviewMatrix *= matrix; }

    // Multiply projection matrix by ortho matrix.
    void applyOrtho(double left, double right, double bottom, double top, double near, double far);

    // Multiply modelview matrix by translation matrix.
    void applyTranslation(EGE::Vec3d);

    void flush() const;

    Window& target() const { return m_target; }

private:
    EGE::RectI m_viewport;
    EGE::DoubleMatrix4x4 m_projectionMatrix = EGE::DoubleMatrix4x4::identity();
    EGE::DoubleMatrix4x4 m_modelviewMatrix = EGE::DoubleMatrix4x4::identity();
    Window& m_target;
};

}
