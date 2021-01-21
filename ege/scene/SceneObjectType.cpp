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

#include "SceneObjectType.h"

#include "Scene.h"
#include "Scene2D.h"

namespace EGE
{

bool SceneObjectType::deserialize(SharedPtr<ObjectMap> data)
{
    log(LogLevel::Info) << "Loading SceneObjectType: " << getId();

    // Part list
    auto d_parts = data->getObject("parts").to<ObjectList>();
    if(d_parts.hasValue())
    {
        for(auto& dd_part: *d_parts.value())
        {
            auto dd_part_map = Object::cast<ObjectMap>(dd_part);
            if(!dd_part_map.hasValue())
            {
                err() << "Part is not a map";
                return false;
            }

            auto part_type = dd_part_map.value()->getObject("type").asString();
            if(!part_type.hasValue())
            {
                err() << "Part type not set";
                return false;
            }

            auto part_name = dd_part_map.value()->getObject("name").asString();
            if(!part_name.hasValue())
            {
                err() << "Part name not set";
                return false;
            }

            PartStub partstub;
            String part_type_s = part_type.value();

            if(!partstub.deserialize(dd_part_map.value()))
            {
                err() << "Invalid PartStub";
                return false;
            }

            log() << "Adding Part: " << part_name.value();
            m_parts[part_name.value()] = partstub;
        }
    }
    log() << "Total part count: " << m_parts.size();
    return true;
}

SharedPtr<SceneObject> SceneObjectType2D::createEmptyObject(Scene& scene) const
{
    ASSERT(dynamic_cast<Scene2D*>(&scene));
    auto sceneObject = createEmptyObject((Scene2D&)scene);
    return sceneObject;
}

void SceneObjectType2D::fillObjectWithData(SceneObject& object) const
{
    // Parts
    log() << "Adding parts to " << object.getName();
    for(auto it: m_parts)
    {
        log() << "  * " << it.first;
        object.addPart(it.first, it.second.makeInstance(object));
    }
}

SharedPtr<SceneObject2D> SceneObjectType2D::createEmptyObject(Scene2D& scene) const
{
    return make<SceneObject2D>(scene, *this);
}

}
