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

#include "ObjectSerializers.h"

namespace EGE
{

namespace Serializers
{

SharedPtr<Object> object(float t)       { return make<ObjectFloat>(t); }
SharedPtr<Object> object(double t)      { return make<ObjectFloat>(t); }
SharedPtr<Object> object(long double t) { return make<ObjectFloat>(t); }

SharedPtr<Object> object(Int64 t)  { return make<ObjectInt>(t); }
SharedPtr<Object> object(Int32 t)  { return make<ObjectInt>(t, ObjectInt::Type::Int); }
SharedPtr<Object> object(Int16 t)  { return make<ObjectInt>(t, ObjectInt::Type::Short); }
SharedPtr<Object> object(Int8 t)   { return make<ObjectInt>(t, ObjectInt::Type::Byte); }

SharedPtr<Object> object(Uint64 t) { return make<ObjectUnsignedInt>(t); }
SharedPtr<Object> object(Uint32 t) { return make<ObjectUnsignedInt>(t, ObjectUnsignedInt::Type::Int); }
SharedPtr<Object> object(Uint16 t) { return make<ObjectUnsignedInt>(t, ObjectUnsignedInt::Type::Short); }
SharedPtr<Object> object(Uint8 t)  { return make<ObjectUnsignedInt>(t, ObjectUnsignedInt::Type::Byte); }

SharedPtr<Object> object(std::string t)          { return make<ObjectString>(t); }

SharedPtr<Object> object(Boolean t) { return make<ObjectBoolean>(t); }

Vec2d toVector2(const ObjectValue& object)
{
    auto list = object.to<ObjectList>();
    if(list.hasValue())
    {
        auto x = list.value()->size() > 0 ? (*list.value()).get(0).asFloat().valueOr(0) : 0.0;
        auto y = list.value()->size() > 1 ? (*list.value()).get(1).asFloat().valueOr(0) : 0.0;
        return Vec2d(x, y);
    }

    auto map = object.to<ObjectMap>();
    if(!map.hasValue())
        return {};
    auto x = map.value()->get("x").asFloat().valueOr(0);
    auto y = map.value()->get("y").asFloat().valueOr(0);
    return Vec2d(x, y);
}

Vec3d toVector3(const ObjectValue& object)
{
    auto list = object.to<ObjectList>();
    if(list.hasValue())
    {
        auto x = list.value()->size() > 0 ? (*list.value()).get(0).asFloat().valueOr(0) : 0.0;
        auto y = list.value()->size() > 1 ? (*list.value()).get(1).asFloat().valueOr(0) : 0.0;
        auto z = list.value()->size() > 2 ? (*list.value()).get(2).asFloat().valueOr(0) : 0.0;
        return Vec3d(x, y, z);
    }

    auto map = object.to<ObjectMap>();
    if(!map.hasValue())
        return {};
    auto x = map.value()->get("x").asFloat().valueOr(0);
    auto y = map.value()->get("y").asFloat().valueOr(0);
    auto z = map.value()->get("z").asFloat().valueOr(0);
    return Vec3d(x, y, z);
}

SharedPtr<ObjectMap> fromColorRGBA(ColorRGBA color)
{
    SharedPtr<ObjectMap> map = make<ObjectMap>();
    map->addObject("r", object(color.r));
    map->addObject("g", object(color.g));
    map->addObject("b", object(color.b));
    map->addObject("a", object(color.a));
    return map;
}

ColorRGBA toColorRGBA(const ObjectValue& object, ColorRGBA fallback)
{
    if(!object.exists())
        return fallback;

    auto map = Object::cast<EGE::ObjectMap>(object);
    if(map.hasValue())
    {
        auto r = map.value()->get("r").asFloat().valueOr(0);
        auto g = map.value()->get("g").asFloat().valueOr(0); 
        auto b = map.value()->get("b").asFloat().valueOr(0);
        auto a = map.value()->get("a").asFloat().valueOr(1); 
        return ColorRGBA::fromFloats(r, g, b, a);
    }

    auto string = Object::cast<EGE::ObjectString>(object);
    if(string.hasValue())
        return ColorRGBA::fromHTML(string.value()->asString());

    return fallback;
}

RectD toRect(const ObjectValue& object)
{
    if(!object.exists())
        return {};
    auto map = object.asMap();
    if(!map.hasValue())
        return {};
    auto pos = toVector2(map.value().get("pos"));
    auto size = toVector2(map.value().get("size"));
    return RectD(pos, size);
}

} // Serializers

}
