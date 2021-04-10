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

#include "CirclePart.h"

#include <ege/debug/Logger.h>
#include <ege/util/ObjectSerializers.h>

namespace EGE
{

void CirclePart::render(Renderer& renderer) const
{
    renderer.renderCircle(position.x, position.y, radius, fillColor, outlineColor);
}

bool CirclePart::deserialize(SharedPtr<ObjectMap> data)
{
    if(!Part::deserialize(data))
        return false;
    position = Serializers::toVector2(data->getObject("pos").to<ObjectMap>().valueOr({}));
    radius = data->getObject("radius").asFloat().valueOr(0);
    fillColor = Serializers::toColorRGBA(data->getObject("fillColor").to<ObjectMap>().valueOr({}));
    outlineColor = Serializers::toColorRGBA(data->getObject("outlineColor").to<ObjectMap>().valueOr({}));
    setGeometryNeedUpdate();
    log() << radius;
    return true;
}

}
