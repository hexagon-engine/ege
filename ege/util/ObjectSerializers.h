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
std::shared_ptr<EGE::Object> object(T t);


// Primitives
std::shared_ptr<EGE::Object> object(float t);
std::shared_ptr<EGE::Object> object(double t);
std::shared_ptr<EGE::Object> object(long double t);

std::shared_ptr<EGE::Object> object(long long t);
std::shared_ptr<EGE::Object> object(int t);
std::shared_ptr<EGE::Object> object(short t);

std::shared_ptr<EGE::Object> object(unsigned long long t);
std::shared_ptr<EGE::Object> object(unsigned int t);
std::shared_ptr<EGE::Object> object(unsigned short t);

// EGE::Vector2
template<class T>
std::shared_ptr<EGE::ObjectMap> fromVector2(EGE::Vector2<T> vec)
{
    std::shared_ptr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
    map->addObject("x", object(vec.x));
    map->addObject("y", object(vec.y));
    return map;
}
EGE::Vec2d toVector2(std::shared_ptr<EGE::ObjectMap> map);

// EGE::Vector3
template<class T>
std::shared_ptr<EGE::ObjectMap> fromVector3(EGE::Vector3<T> vec)
{
    std::shared_ptr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
    map->addObject("x", object(vec.x));
    map->addObject("y", object(vec.y));
    map->addObject("z", object(vec.z));
    return map;
}
EGE::Vec3d toVector3(std::shared_ptr<EGE::ObjectMap> map);

}

}
