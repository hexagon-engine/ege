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

#include "RenderStates.h"
#include "Renderer.h"

#include <ege/util/Vector.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class Renderable
{
public:
    void renderWithStates(Renderer& renderer, const RenderStates& states) const;

    // Render with setting view and states.
    virtual void doRender(Renderer& renderer, const RenderStates& states = {});

    bool geometryNeedUpdate() const { return m_geometryNeedUpdate; }

    // FIXME: allow multiple shaders
    void setShader(SharedPtr<sf::Shader> shader) { m_shader = shader; }

protected:
    virtual void render(Renderer& renderer) const = 0;

    virtual void setGeometryNeedUpdate(bool val = true) { m_geometryNeedUpdate = val; }

    virtual void updateGeometry(Renderer&) {}

    virtual bool isCustomViewNeeded() const { return false; }
    virtual sf::View getCustomView(sf::RenderTarget& target) const { return target.getDefaultView(); }

    virtual bool isCustomTransformNeeded() const { return false; }
    virtual sf::Transform getCustomTransform(sf::RenderTarget&) const { return {}; }

    virtual void applyStates(Renderer& renderer);
    virtual void applyStates(Renderer& renderer, RenderStates& states);
    virtual void doUpdateGeometry(Renderer& renderer);

private:
    bool m_geometryNeedUpdate = true;
    SharedPtr<sf::Shader> m_shader;
};

}
