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

#include "Color.h"
#include "Object.h"
#include "ObjectBoolean.h"
#include "ObjectFloat.h"
#include "ObjectInt.h"
#include "ObjectMap.h"
#include "ObjectUnsignedInt.h"
#include "PointerUtils.h"
#include "Rect.h"
#include "Types.h"
#include "Vector.h"

#include <memory>

namespace EGE
{

namespace Serializers
{

template<class T>
SharedPtr<Object> object(T t);


// Primitives
SharedPtr<Object> object(Float t);
SharedPtr<Object> object(Double t);
SharedPtr<Object> object(LongDouble t);

SharedPtr<Object> object(Int64 t);
SharedPtr<Object> object(Int32 t);
SharedPtr<Object> object(Int16 t);

SharedPtr<Object> object(Uint64 t);
SharedPtr<Object> object(Uint32 t);
SharedPtr<Object> object(Uint16 t);

SharedPtr<Object> object(String t);

SharedPtr<Object> object(Boolean t);

template<class VT>
SharedPtr<Object> object(Vector<VT> t)
{
    SharedPtr<ObjectList> _list;
    for(VT& obj: t)
        _list->addObject(object(obj));
    return _list;
}

template<class VT>
SharedPtr<Object> object(StringMap<VT> t)
{
    SharedPtr<ObjectMap> _map;
    for(auto& obj: t)
        _map->addObject(obj.first, object(obj.second));
    return _map;
}

// EGE::Vector2
template<class T>
SharedPtr<ObjectMap> fromVector2(Vector2<T> vec)
{
    SharedPtr<ObjectMap> map = make<ObjectMap>();
    map->addObject("x", object(vec.x));
    map->addObject("y", object(vec.y));
    return map;
}
Vec2d toVector2(SharedPtr<ObjectMap> map);

// EGE::Vector3
template<class T>
SharedPtr<ObjectMap> fromVector3(Vector3<T> vec)
{
    SharedPtr<ObjectMap> map = make<ObjectMap>();
    map->addObject("x", object(vec.x));
    map->addObject("y", object(vec.y));
    map->addObject("z", object(vec.z));
    return map;
}
Vec3d toVector3(SharedPtr<ObjectMap> map);

// EGE::ColorRGBA
SharedPtr<ObjectMap> fromColorRGBA(ColorRGBA color);
ColorRGBA toColorRGBA(SharedPtr<Object> object, ColorRGBA fallback = {});

// EGE::Rect
template<class T>
SharedPtr<ObjectMap> fromRect(Rect<T> rect)
{
    SharedPtr<ObjectMap> map = make<ObjectMap>();
    map->addObject("pos", fromVector2(rect.position));
    map->addObject("size", fromVector2(rect.size));
    return map;
}
RectD toRect(SharedPtr<ObjectMap> map);

}

}
