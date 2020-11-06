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
#include "Vector.h"

#include <memory>

namespace EGE
{

namespace Serializers
{

template<class T>
std::shared_ptr<Object> object(T t);


// Primitives
std::shared_ptr<Object> object(float t);
std::shared_ptr<Object> object(double t);
std::shared_ptr<Object> object(long double t);

std::shared_ptr<Object> object(long long t);
std::shared_ptr<Object> object(int t);
std::shared_ptr<Object> object(short t);

std::shared_ptr<Object> object(unsigned long long t);
std::shared_ptr<Object> object(unsigned int t);
std::shared_ptr<Object> object(unsigned short t);

std::shared_ptr<Object> object(std::string t);

template<class VT>
std::shared_ptr<Object> object(std::vector<VT> t)
{
    std::shared_ptr<ObjectList> _list;
    for(VT& obj: t)
        _list->addObject(object(obj));
    return _list;
}

template<class VT>
std::shared_ptr<Object> object(std::map<std::string, VT> t)
{
    std::shared_ptr<ObjectMap> _map;
    for(auto& obj: t)
        _map->addObject(obj.first, object(obj.second));
    return _map;
}

// EGE::Vector2
template<class T>
std::shared_ptr<ObjectMap> fromVector2(Vector2<T> vec)
{
    std::shared_ptr<ObjectMap> map = make<ObjectMap>();
    map->addObject("x", object(vec.x));
    map->addObject("y", object(vec.y));
    return map;
}
Vec2d toVector2(std::shared_ptr<ObjectMap> map);

// EGE::Vector3
template<class T>
std::shared_ptr<ObjectMap> fromVector3(Vector3<T> vec)
{
    std::shared_ptr<ObjectMap> map = make<ObjectMap>();
    map->addObject("x", object(vec.x));
    map->addObject("y", object(vec.y));
    map->addObject("z", object(vec.z));
    return map;
}
Vec3d toVector3(std::shared_ptr<ObjectMap> map);

}

}
