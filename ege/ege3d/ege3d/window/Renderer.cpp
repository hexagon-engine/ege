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

#include "Renderer.h"
#include "GLError.h"

#include <ege/debug/Logger.h>
#include <ege/util/Color.h>
#include <ege/util/Vector.h>
#include <ege/util/VectorOperations.h>
#include <ege/util/Types.h>

#include <GL/gl.h>

#define RENDERER_DEBUG 0

namespace EGE3d
{

void Renderer::renderVertexesRaw(Vertex const* array, size_t count, GLenum mode, size_t first)
{
    ASSERT(first < count);
    static bool enabled = false;
    if(!enabled)
    {
        enabled = true;
        EGE3D_GLCHECK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
        EGE3D_GLCHECK(glEnableClientState(GL_COLOR_ARRAY));
        EGE3D_GLCHECK(glEnableClientState(GL_NORMAL_ARRAY));
        EGE3D_GLCHECK(glEnableClientState(GL_VERTEX_ARRAY));
    }
    EGE3D_GLCHECK(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &array[0].texCoords));
    EGE3D_GLCHECK(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &array[0].color));
    EGE3D_GLCHECK(glNormalPointer(GL_FLOAT, sizeof(Vertex), &array[0].normal));
    EGE3D_GLCHECK(glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &array[0].position));
    EGE3D_GLCHECK(glDrawArrays(mode, first, count));
}

void Renderer::renderRectangle(EGE::RectF rect, EGE::ColorRGBA const& fillColor)
{
    Vertex vertexes[4] = {
        {{0.f, 0.f}, fillColor, {}, rect.leftTopPoint()},
        {{0.f, 1.f}, fillColor, {}, rect.rightTopPoint()},
        {{1.f, 0.f}, fillColor, {}, rect.leftBottomPoint()},
        {{1.f, 1.f}, fillColor, {}, rect.rightBottomPoint()},
    };
    renderVertexes(vertexes, GL_TRIANGLE_STRIP);
}

void Renderer::renderCircle(EGE::Vec2f center, float radius, EGE::ColorRGBA const& fillColor, size_t points)
{
    EGE::Vector<Vertex> vertexes;
    vertexes.resize(points + 2);

    // TODO: Implement proper texturing
    vertexes[0] = {{}, fillColor, {}, center};
    for(size_t s = 1; s <= points + 1; s++)
    {
        float angle = 360 * s / points;
        EGE::Vec2f position = center + EGE::VectorOperations::fromPolar<float>({angle, radius});
        vertexes[s] = {position / radius, fillColor, {}, position};
    }

    renderVertexes(vertexes, GL_TRIANGLE_FAN);
}

void Renderer::renderTexturedRectangle(EGE::RectF rect, Texture const& texture, EGE::RectF textureRect)
{
    TextureBinder binder(texture);
    if(textureRect == EGE::RectF())
        textureRect = {texture.size()};
    Vertex vertexes[4] = {
        {textureRect.leftTopPoint() / rect.size.x, EGE::Colors::white, {}, rect.leftTopPoint()},
        {textureRect.rightTopPoint() / rect.size.y, EGE::Colors::white, {}, rect.rightTopPoint()},
        {textureRect.leftBottomPoint() / rect.size.x, EGE::Colors::white, {}, rect.leftBottomPoint()},
        {textureRect.rightBottomPoint() / rect.size.y, EGE::Colors::white, {}, rect.rightBottomPoint()},
    };
    renderVertexes(vertexes, GL_TRIANGLE_STRIP);
}

void Renderer::glViewport(EGE::RectI rect)
{
    ::glViewport(rect.position.x, target().getSize().y - rect.position.y - rect.size.y, rect.size.x, rect.size.y);
}

void Renderer::glMatrixMode(MatrixMode mode)
{
    GLenum matrixMode = 0;
    switch(mode)
    {
        case MatrixMode::Modelview: matrixMode = GL_MODELVIEW; break;
        case MatrixMode::Projection: matrixMode = GL_PROJECTION; break;
        default: CRASH();
    }
    ::glMatrixMode(matrixMode);
}

void Renderer::glLoadIdentity()
{
    ::glLoadIdentity();
}

bool Renderer::glIsError() const
{
    auto error = glGetError();
    return error != GL_NO_ERROR;
}

void Renderer::ensureIsCurrent() const
{
    if(!target().isCurrent())
        target().setCurrent();
}

}
