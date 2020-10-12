/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "EGEPacketConverter.h"

#include <ege/debug/Logger.h>
#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/ObjectFloat.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectList.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/ObjectString.h>
#include <iostream>

#include <SFML/Network.hpp>

namespace EGE
{

namespace Internal
{
    struct _ParseResult
    {
        std::string message;
        size_t byte = 0;
    };
}

static bool parseUnsignedInt(sf::Packet& input, ObjectUnsignedInt& object)
{
    int8_t type;
    if(!(input >> type))
        return false;

    switch(type)
    {
    case 'b':
        {
            sf::Uint8 val;
            input >> val;
            object.setNumber(val);
            object.setType(ObjectUnsignedInt::Type::Byte);
        } return true;
    case 's':
        {
            sf::Uint16 val;
            input >> val;
            object.setNumber(val);
            object.setType(ObjectUnsignedInt::Type::Short);
        } return true;
    case 'i':
        {
            sf::Uint32 val;
            input >> val;
            object.setNumber(val);
            object.setType(ObjectUnsignedInt::Type::Int);
        } return true;
    case 'l':
        {
            sf::Uint64 val;
            input >> val;
            object.setNumber(val);
            object.setType(ObjectUnsignedInt::Type::Long);
        } return true;
    }
    return false;
}

static bool parseInt(sf::Packet& input, ObjectInt& object)
{
    int8_t type;
    if(!(input >> type))
        return false;

    switch(type)
    {
    case 'b':
        {
            sf::Int8 val;
            input >> val;
            object.setNumber(val);
            object.setType(ObjectInt::Type::Byte);
        } return true;
    case 's':
        {
            sf::Int16 val;
            input >> val;
            object.setNumber(val);
            object.setType(ObjectInt::Type::Short);
        } return true;
    case 'i':
        {
            sf::Int32 val;
            input >> val;
            object.setNumber(val);
            object.setType(ObjectInt::Type::Int);
        } return true;
    case 'l':
        {
            sf::Int64 val;
            input >> val;
            object.setNumber(val);
            object.setType(ObjectInt::Type::Long);
        } return true;
    }
    return false;
}

static bool parseFloat(sf::Packet& input, ObjectFloat& object)
{
    double data;
    if(!(input >> data))
        return false;

    object.setNumber(data);
    return true;
}

static bool parseString(sf::Packet& input, ObjectString& object)
{
    std::string data;
    if(!(input >> data))
        return false;

    object.setString(data);
    return true;
}

static Internal::_ParseResult parseMap(sf::Packet& input, ObjectMap& object);
static Internal::_ParseResult parseList(sf::Packet& input, ObjectList& object);

static std::shared_ptr<Object> parseSpecific(sf::Uint8 type, sf::Packet& input)
{
    // value
    switch(type)
    {
    case 'm':
        {
            std::shared_ptr<ObjectMap> obj = make<ObjectMap>();
            Internal::_ParseResult result = parseMap(input, *obj);
            if(!result.message.empty())
                return nullptr;
            return obj;
        }
        break;
    case 'l':
        {
            std::shared_ptr<ObjectList> obj = make<ObjectList>();
            Internal::_ParseResult result = parseList(input, *obj);
            if(!result.message.empty())
                return nullptr;
            return obj;
        }
        break;
    case 'u':
        {
            std::shared_ptr<ObjectUnsignedInt> obj = make<ObjectUnsignedInt>(0);
            if(!parseUnsignedInt(input, *obj))
                return nullptr;
            return obj;
        }
        break;
    case 'i':
        {
            std::shared_ptr<ObjectInt> obj = make<ObjectInt>(0);
            if(!parseInt(input, *obj))
                return nullptr;
            return obj;
        }
        break;
    case 'f':
        {
            std::shared_ptr<ObjectFloat> obj = make<ObjectFloat>(0.0);
            if(!parseFloat(input, *obj))
                return nullptr;
            return obj;
        }
        break;
    case 's':
        {
            std::shared_ptr<ObjectString> obj = make<ObjectString>("");
            if(!parseString(input, *obj))
                return nullptr;
            return obj;
        }
        break;
    }
    return nullptr;
}

static Internal::_ParseResult parseMap(sf::Packet& input, ObjectMap& object)
{
    while(true)
    {
        // key
        std::string key;
        if(!input.endOfPacket())
        {
            // TODO: maybe peek() function?
            size_t readPos = input.getReadPosition();
            const void* data = input.getData();
            unsigned char chr = ((unsigned char*)data)[readPos];
            if(chr != '0')
            {
                if(!(input >> key))
                {
                    return {"expected Key", input.getReadPosition()};
                }
            }
            else
            {
                // actually read the character.
                input >> chr;
                return {};
            }
        }

        // value type
        sf::Uint8 type;
        if(!(input >> type))
        {
            return {};
        }

        // value
        std::shared_ptr<Object> specific = parseSpecific(type, input);
        if(!specific)
            return {"expected value", input.getReadPosition()};
        object.addObject(key, specific);
    }

    ASSERT(false);
    return {"impossible error", input.getReadPosition()};
}

static Internal::_ParseResult parseList(sf::Packet& input, ObjectList& object)
{
    while(true)
    {
        // value
        if(!input.endOfPacket())
        {
            // TODO: maybe peek() function?
            size_t readPos = input.getReadPosition();
            const void* data = input.getData();
            unsigned char chr = ((unsigned char*)data)[readPos];
            if(chr != '0')
            {
                // value type
                sf::Uint8 type;
                if(!(input >> type))
                {
                    return {};
                }

                // value
                std::shared_ptr<Object> specific = parseSpecific(type, input);
                if(!specific)
                    return {"expected value", input.getReadPosition()};
                object.addObject(specific);
            }
            else
            {
                // actually read the character.
                input >> chr;
                return {};
            }
        }
    }

    ASSERT(false);
    return {"impossible error", input.getReadPosition()};
}

bool EGEPacketConverter::in(sf::Packet& input, ObjectMap& object) const
{
    // value type must be 'm'
    sf::Uint8 type;
    if(!(input >> type) || type != 'm')
    {
        return false;
    }

    Internal::_ParseResult result = parseMap(input, (ObjectMap&)object);
    if(!result.message.empty())
    {
        err(LogLevel::Verbose) << "001D EGE/egeNetwork: Packet parsing error at byte " << std::hex << result.byte << std::dec << ":" << result.message;
        return false;
    }
    return true;
}

static bool outputInt(sf::Packet& output, const ObjectInt& object)
{
    output << 'i';
    switch(object.getType())
    {
    case ObjectInt::Type::Byte:
        output << 'b' << (sf::Int8)object.asInt(); break;
    case ObjectInt::Type::Short:
        output << 's' << (sf::Int16)object.asInt(); break;
    case ObjectInt::Type::Int:
        output << 'i' << (sf::Int32)object.asInt(); break;
    case ObjectInt::Type::Long:
        output << 'l' << (sf::Int64)object.asInt(); break;
    default:
        return false;
    }
    return true;
}

static bool outputUnsignedInt(sf::Packet& output, const ObjectUnsignedInt& object)
{
    output << 'u';
    switch(object.getType())
    {
    case ObjectUnsignedInt::Type::Byte:
        output << 'b' << (sf::Uint8)object.asUnsignedInt(); break;
    case ObjectUnsignedInt::Type::Short:
        output << 's' << (sf::Uint16)object.asUnsignedInt(); break;
    case ObjectUnsignedInt::Type::Int:
        output << 'i' << (sf::Uint32)object.asUnsignedInt(); break;
    case ObjectUnsignedInt::Type::Long:
        output << 'l' << (sf::Uint64)object.asUnsignedInt(); break;
    default:
        return false;
    }
    return true;
}

static bool outputObject(sf::Packet& output, const Object& object)
{
    if(object.isMap())
    {
        output << (sf::Uint8)'m';
        bool success = true;
        for(auto pr: (ObjectMap&)object)
        {
            output << pr.first;
            success |= outputObject(output, *pr.second);
        }
        output << (sf::Uint8)'0';
        return success;
    }
    else if(object.isList())
    {
        output << (sf::Uint8)'l';
        bool success = true;
        for(auto it: (ObjectList&)object)
        {
            success |= outputObject(output, *it);
        }
        output << (sf::Uint8)'0';
        return success;
    }
    else if(object.isUnsignedInt())
    {
        return outputUnsignedInt(output, (ObjectUnsignedInt&)object);
    }
    else if(object.isInt())
    {
        return outputInt(output, (ObjectInt&)object);
    }
    else if(object.isFloat())
    {
        output << (sf::Uint8)'f' << object.asFloat();
        return true;
    }
    else if(object.isString())
    {
        output << (sf::Uint8)'s' << object.asString();
        return true;
    }
    return false;
}

bool EGEPacketConverter::out(sf::Packet& output, const ObjectMap& object) const
{
    return outputObject(output, object);
}

}
