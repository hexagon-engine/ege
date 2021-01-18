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

#include "Part.h"

#include <ege/util/Serializable.h>
#include <functional>

#define EGE_PART_CREATOR_2D(_type) [](EGE::SceneObject& sobject)->EGE::SharedPtr<EGE::Part> \
{ \
    auto sobject2d = dynamic_cast<EGE::SceneObject2D*>(&sobject); \
    if(!sobject2d) \
        return nullptr; \
    return make<_type>(*sobject2d); \
}

namespace EGE
{

class PartCreatorMap
{
public:
    PartCreatorMap();

    typedef std::function<SharedPtr<Part>(SceneObject&)> PartCreator;
    typedef Map<String, PartCreator> ValueType;

    void add(String type, const PartCreator& creator)
        { map.insert(make_pair(type, creator)); }

    const PartCreator* get(String type)
        {
            auto it = map.find(type);
            if(it == map.end())
                return nullptr;
            return &it->second;
        }
private:
    ValueType map;
};

class PartStub : public Serializable
{
public:
    // Use this object to add your own parts!
    static PartCreatorMap PartCreators;

    SharedPtr<Part> makeInstance(SceneObject&);

    virtual SharedPtr<ObjectMap> serialize() const override;
    virtual bool deserialize(SharedPtr<ObjectMap>) override;

private:
    SharedPtr<ObjectMap> m_map;
    String m_type;
};

}
