/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Object.h"
#include "ObjectFloat.h"
#include "ObjectInt.h"
#include "ObjectMap.h"
#include "ObjectUnsignedInt.h"
#include "PointerUtils.h"
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

}

}
