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

#include "RenderingState.h"

#include "Window.h"

#include <ege/debug/Logger.h>

#include <GL/gl.h>

namespace EGE3d
{

void RenderingState::flush() const
{
    glViewport(m_viewport.position.x, m_target.getSize().y - m_viewport.position.y - m_viewport.size.y, m_viewport.size.x, m_viewport.size.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixd(m_projectionMatrix.data());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixd(m_modelviewMatrix.data());
}

void RenderingState::applyOrtho(double left, double right, double bottom, double top, double near, double far)
{
    double tx = -(right+left)/(right-left);
    double ty = -(top+bottom)/(top-bottom);
    double tz = -(far+near)/(far-near);
    applyProjectionMatrix(EGE::DoubleMatrix4x4{{
        2/(right-left), 0.0, 0.0, tx,
        0.0, 2/(top-bottom), 0.0, ty,
        0.0, 0.0, -2/(far-near), tz,
        0.0, 0.0, 0.0, 1.0
    }});
}

void RenderingState::applyTranslation(EGE::Vec3d v)
{
    applyProjectionMatrix(EGE::DoubleMatrix4x4{{
        1.0, 0.0, 0.0, v.x,
        0.0, 1.0, 0.0, v.y,
        0.0, 0.0, 1.0, v.z,
        0.0, 0.0, 0.0, 1.0
    }});
}

}
