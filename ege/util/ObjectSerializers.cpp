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
    auto _x = map->getObject("x");
    auto _y = map->getObject("y");

    if(_x.expired() || _y.expired())
    {
        return Vec2d(0, 0); //TODO: Optional
    }

    if(!_x.lock()->isFloat() || !_y.lock()->isFloat())
    {
        return Vec2d(0, 0); //TODO: Optional
    }

    auto x = _x.lock()->asFloat();
    auto y = _y.lock()->asFloat();
    return Vec2d(x, y);
}

Vec3d toVector3(std::shared_ptr<ObjectMap> map)
{
    auto _x = map->getObject("x");
    auto _y = map->getObject("y");
    auto _z = map->getObject("z");

    if(_x.expired() || _y.expired() || _z.expired())
    {
        return Vec3d(0, 0, 0); //TODO: Optional
    }

    if(!_x.lock()->isFloat() || !_y.lock()->isFloat() || !_z.lock()->isFloat())
    {
        return Vec3d(0, 0, 0); //TODO: Optional
    }

    auto x = _x.lock()->asFloat();
    auto y = _y.lock()->asFloat();
    auto z = _z.lock()->asFloat();
    return Vec3d(x, y, z);
}

}

}
