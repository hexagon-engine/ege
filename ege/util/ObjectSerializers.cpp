/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectSerializers.h"

namespace EGE
{

namespace Serializers
{

std::shared_ptr<Object> object(float t)                { return make<ObjectFloat>(t); }
std::shared_ptr<Object> object(double t)               { return make<ObjectFloat>(t); }
std::shared_ptr<Object> object(long double t)          { return make<ObjectFloat>(t); }

std::shared_ptr<Object> object(long long t)            { return make<ObjectInt>(t); }
std::shared_ptr<Object> object(int t)                  { return make<ObjectInt>(t, ObjectInt::Type::Int); }
std::shared_ptr<Object> object(short t)                { return make<ObjectInt>(t, ObjectInt::Type::Short); }

std::shared_ptr<Object> object(unsigned long long t)   { return make<ObjectUnsignedInt>(t); }
std::shared_ptr<Object> object(unsigned int t)         { return make<ObjectUnsignedInt>(t, ObjectUnsignedInt::Type::Int); }
std::shared_ptr<Object> object(unsigned short t)       { return make<ObjectUnsignedInt>(t, ObjectUnsignedInt::Type::Short); }

std::shared_ptr<Object> object(std::string t)          { return make<ObjectString>(t); }

Vec2d toVector2(std::shared_ptr<ObjectMap> map)
{
    if(!map)
        return {};
    auto x = map->getObject("x").as<Float>().valueOr(0);
    auto y = map->getObject("y").as<Float>().valueOr(0);
    return Vec2d(x, y);
}

Vec3d toVector3(std::shared_ptr<ObjectMap> map)
{
    if(!map)
        return {};
    auto x = map->getObject("x").as<Float>().valueOr(0);
    auto y = map->getObject("y").as<Float>().valueOr(0);
    auto z = map->getObject("z").as<Float>().valueOr(0);
    return Vec3d(x, y, z);
}

} // Serializers

}
