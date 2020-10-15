/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectSerializers.h"

namespace EGE
{

namespace Serializers
{

std::shared_ptr<EGE::Object> object(float t)                { return make<EGE::ObjectFloat>(t); }
std::shared_ptr<EGE::Object> object(double t)               { return make<EGE::ObjectFloat>(t); }
std::shared_ptr<EGE::Object> object(long double t)          { return make<EGE::ObjectFloat>(t); }

std::shared_ptr<EGE::Object> object(long long t)            { return make<EGE::ObjectInt>(t); }
std::shared_ptr<EGE::Object> object(int t)                  { return make<EGE::ObjectInt>(t); }
std::shared_ptr<EGE::Object> object(short t)                { return make<EGE::ObjectInt>(t); }

std::shared_ptr<EGE::Object> object(unsigned long long t)   { return make<EGE::ObjectUnsignedInt>(t); }
std::shared_ptr<EGE::Object> object(unsigned int t)         { return make<EGE::ObjectUnsignedInt>(t); }
std::shared_ptr<EGE::Object> object(unsigned short t)       { return make<EGE::ObjectUnsignedInt>(t); }

EGE::Vec2d toVector2(std::shared_ptr<EGE::ObjectMap> map)
{
    auto _x = map->getObject("x");
    auto _y = map->getObject("y");

    if(_x.expired() || _y.expired())
    {
        return EGE::Vec2d(0, 0); //TODO: EGE::Optional
    }

    if(!_x.lock()->isFloat() || !_y.lock()->isFloat())
    {
        return EGE::Vec2d(0, 0); //TODO: EGE::Optional
    }

    auto x = _x.lock()->asFloat();
    auto y = _y.lock()->asFloat();
    return EGE::Vec2d(x, y);
}

EGE::Vec3d toVector3(std::shared_ptr<EGE::ObjectMap> map)
{
    auto _x = map->getObject("x");
    auto _y = map->getObject("y");
    auto _z = map->getObject("z");

    if(_x.expired() || _y.expired() || _z.expired())
    {
        return EGE::Vec3d(0, 0, 0); //TODO: EGE::Optional
    }

    if(!_x.lock()->isFloat() || !_y.lock()->isFloat() || !_z.lock()->isFloat())
    {
        return EGE::Vec3d(0, 0, 0); //TODO: EGE::Optional
    }

    auto x = _x.lock()->asFloat();
    auto y = _y.lock()->asFloat();
    auto z = _z.lock()->asFloat();
    return EGE::Vec3d(x, y, z);
}

}

}
