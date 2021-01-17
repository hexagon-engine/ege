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

#include "PartStub.h"

#include "CirclePart.h"
#include "RectanglePart.h"
#include "SceneObject2D.h"
#include "TexturedPart.h"

namespace EGE
{

PartCreatorMap::PartCreatorMap()
{
    // Add default parts
    add("Rectangle", EGE_PART_CREATOR_2D(EGE::RectanglePart));
    add("Circle", EGE_PART_CREATOR_2D(EGE::CirclePart));
    add("Textured", EGE_PART_CREATOR_2D(EGE::TexturedPart));
}

PartCreatorMap PartStub::PartCreators;

SharedPtr<Part> PartStub::makeInstance(SceneObject& sobject)
{
    log() << "Creating instance of part for SO " << sobject.getName();
    auto partCreator = PartStub::PartCreators.get(m_type);
    if(!partCreator)
    {
        err() << "No such part with type: " << m_type;
        return nullptr;
    }

    auto part = (*partCreator)(sobject);
    if(!part)
    {
        err() << "Failed to create part!";
        return nullptr;
    }

    if(!part->deserialize(m_map))
        return nullptr;
    return part;
}

SharedPtr<ObjectMap> PartStub::serialize() const
{
    return m_map;
}

bool PartStub::deserialize(SharedPtr<ObjectMap> data)
{
    m_map = Object::cast<ObjectMap>(data->copy()).value();
    m_type = data->getObject("type").as<String>().valueOr("");
    if(m_type.empty())
        return false;
    return true;
}

}

