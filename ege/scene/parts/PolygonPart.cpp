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

#include "PolygonPart.h"

#include <ege/util/ObjectSerializers.h>

namespace EGE
{

void PolygonPart::render(Renderer& renderer) const
{
    renderer.renderPrimitives(m_vertexes, sf::TriangleFan);
}

void PolygonPart::updateGeometry(Renderer&)
{
    // TODO: Support non-convex shapes

    // Average
    Vec2d avg;
    for(auto& vertex: vertexes)
        avg += vertex;

    avg /= static_cast<double>(vertexes.size());

    auto sfColor = sf::Color(fillColor.r * 255, fillColor.g * 255, fillColor.b * 255, fillColor.a * 255);
    m_vertexes.push_back(Vertex::make({static_cast<float>(avg.x), static_cast<float>(avg.y), 0}, sfColor));

    for(auto& vertex: vertexes)
        m_vertexes.push_back(Vertex::make({static_cast<float>(vertex.x), static_cast<float>(vertex.y), 0}, sfColor));

    m_vertexes.push_back(Vertex::make({static_cast<float>(vertexes.front().x), static_cast<float>(vertexes.front().y), 0}, sfColor));
}

bool PolygonPart::deserialize(SharedPtr<ObjectMap> data)
{
    if(!Part::deserialize(data))
        return false;

    auto _vertexes = data->getObject("vertexes").to<ObjectList>().valueOr({});
    for(auto& vertex : *_vertexes)
    {
        auto _vertex = Object::cast<ObjectMap>(vertex);
        if(!_vertex.hasValue())
            return false;

        vertexes.push_back(Serializers::toVector2(_vertex.value()));
    }

    fillColor = Serializers::toColorRGBA(data->getObject("fillColor").object());
    setGeometryNeedUpdate();
    return true;
}

}
