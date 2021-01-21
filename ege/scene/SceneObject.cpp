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

#include "SceneObject.h"

#include "Scene.h"

#include <ege/debug/Dump.h>
#include <ege/util/ObjectString.h>

namespace EGE
{

SceneObject::~SceneObject()
{
    log(LogLevel::Debug) << "SceneObject::~SceneObject() " << this;
}

void SceneObject::doRender(Renderer& renderer, const RenderStates& states)
{
    Renderable::doRender(renderer, states);

    if(m_renderer)
        m_renderer->doRender(renderer);

    // Render all parts :)
    for(auto it: getParts())
        it.second->doRender(renderer, states);
}

void SceneObject::onUpdate(long long)
{
    if(!m_parentId.empty() && !m_parent)
    {
        setParent(getOwner().getObjectByName(m_parentId));
        m_parentId = "";
        m_changedSinceLoad = false;
    }

    Animatable::onUpdate();
}

std::shared_ptr<ObjectMap> SceneObject::serialize() const
{
    std::shared_ptr<ObjectMap> data = make<ObjectMap>();
    data->addString("name", m_name);
    data->addObject("m", serializeMain());
    data->addObject("x", serializeExtended());
    return data;
}

bool SceneObject::deserialize(std::shared_ptr<ObjectMap> object)
{
    ASSERT(object);
    auto _name = object->getObject("name");
    auto _m = object->getObject("m");
    auto _x = object->getObject("x");

    m_name = _name.as<EGE::String>().valueOr(m_name);

    bool s = true;

    if(_m.is<ObjectMap::ValueType>())
        s &= deserializeMain(_m.to<ObjectMap>().value());
    else
    {
        err(LogLevel::Error) << "No main data key in SceneObject data!";
        return false;
    }

    if(_x.is<ObjectMap::ValueType>())
        s &= deserializeExtended(_x.to<ObjectMap>().value());
    else
        err(LogLevel::Verbose) << "No extended data key in SceneObject data!";
        // They are not required

    m_changedSinceLoad = false;
    return s;
}

std::shared_ptr<ObjectMap> SceneObject::serializeMain() const
{
    auto data = make<ObjectMap>();
    if(m_parent)
        data->addString("parent", m_parent->getName());

    return data;
}

bool SceneObject::deserializeMain(std::shared_ptr<ObjectMap> data)
{
    m_parentId = data->getObject("parent").as<String>().valueOr("");
    return true;
}

std::shared_ptr<ObjectMap> SceneObject::serializeExtended() const
{
    return nullptr;
}

bool SceneObject::deserializeExtended(std::shared_ptr<ObjectMap>)
{
    // nothing
    return true;
}

void SceneObject::setParent(SceneObject* object)
{
    log(LogLevel::Debug) << "SceneObject::setParent(" << object << ")";
    if(m_parent)
        m_parent->m_children.erase(this);

    setMainChanged();
    m_parent = object;
    if(!object)
        return;

    m_parent->m_children.insert(this);
    isnSetParent(object);
}

Part* SceneObject::getPart(String name)
{
    auto it = m_parts.find(name);
    if(it != m_parts.end())
        return it->second.get();
    return nullptr;
}

void SceneObject::init()
{
    getType().fillObjectWithData(*this);
    onInit();
}

}
