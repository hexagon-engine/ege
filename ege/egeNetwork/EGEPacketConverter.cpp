/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "EGEPacketConverter.h"

#include <ege/main/Config.h>
#include <ege/util/ObjectInt.h>
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

static bool parseInt(sf::Packet& input, ObjectInt& object)
{
    long long data;
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

static std::shared_ptr<Object> parseSpecific(sf::Uint8 type, sf::Packet& input)
{
    // value
    switch(type)
    {
    case 'm':
        {
            std::shared_ptr<ObjectMap> obj = std::make_shared<ObjectMap>();
            Internal::_ParseResult result = parseMap(input, *obj);
            if(!result.message.empty())
                return nullptr;
            return obj;
        }
        break;
    case 'i':
        {
            std::shared_ptr<ObjectInt> obj = std::make_shared<ObjectInt>(0);
            if(!parseInt(input, *obj))
                return nullptr;
            return obj;
        }
        break;
    case 's':
        {
            std::shared_ptr<ObjectString> obj = std::make_shared<ObjectString>("");
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

bool EGEPacketConverter::in(sf::Packet& input, ObjectMap& object) const
{
    // value type must be 'm'
    sf::Uint8 type;
    if(!(input >> type) || type != 'm')
    {
        return false;
    }

    ASSERT(instanceof(&object, ObjectMap));

    Internal::_ParseResult result = parseMap(input, (ObjectMap&)object);
    if(!result.message.empty())
    {
        std::cerr << "001D EGE/egeNetwork: Packet parsing error at byte " << std::hex << result.byte << std::dec << ":" << result.message << std::endl;
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
    else if(object.isInt())
    {
        output << (sf::Uint8)'i' << object.asInt();
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
