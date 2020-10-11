/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "JSONConverter.h"

#include "Object.h"
#include "ObjectFloat.h"
#include "ObjectList.h"
#include "ObjectMap.h"
#include "ObjectString.h"
#include "ObjectInt.h"
#include "PointerUtils.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <memory>

namespace EGE
{

std::string logIndex(size_t i)
{
    return " (at i=" + std::to_string(i) +  ")";
}

size_t ignoreWhitespace(JSONConverter::InputStreamType& input)
{
    size_t counter = 0;
    while(isspace(input.peek()))
    {
        input.ignore(1);
        counter++;
    }
    return counter;
}

bool parseValue(JSONConverter::InputStreamType& input, std::shared_ptr<Object>& object);

bool consumeStringWithEscapes(JSONConverter::InputStreamType& input, std::string& object)
{
    // Starting '"'
    char c = input.peek();
    if(c != '"')
    {
        std::cerr << "json: expected '\"'" << logIndex(input.tellg()) << std::endl;
        return false;
    }
    input.ignore(1);

    bool escape = false;
    while(true)
    {
        c = input.peek();
        if(escape)
        {
            switch(c)
            {
                case '\\': object += "\\"; break;
                case '/': object += "/"; break;
                case 'n': object += "\n"; break;
                case 't': object += "\t"; break;
                case '"': object += "\""; break;
                case '\n': break;
                default:
                {
                    std::cerr << "json: invalid escape character: " << c << logIndex(input.tellg()) << std::endl;
                    return false;
                }
            }
            escape = false;
            input.ignore(1);
            continue;
        }

        if(c == '"' && !escape)
        {
            input.ignore(1);
            return true;
        }
        else if(c == '\\' && !escape)
        {
            escape = true;
            input.ignore(1);
            continue;
        }
        else
        {
            object += c;
            input.ignore(1);
        }
    }
}

bool parseString(JSONConverter::InputStreamType& input, std::string& object)
{
    if(!consumeStringWithEscapes(input, object))
    {
        std::cerr << "json: invalid string" << logIndex(input.tellg()) << std::endl;
        return false;
    }

    if(input.eof())
    {
        std::cerr << "json: unexpected EOF" << logIndex(input.tellg()) << std::endl;
        return false;
    }
    return true;
}

bool parsePair(JSONConverter::InputStreamType& input, ObjectMap& object)
{
    // name
    std::string name;
    if(!parseString(input, name))
    {
        std::cerr << "json: expected name" << logIndex(input.tellg()) << std::endl;
        return false;
    }

    // potential whitespace
    ignoreWhitespace(input);

    // colon
    char c = input.peek();
    if(c != ':')
    {
        std::cerr << "json: expected ':'" << logIndex(input.tellg()) << std::endl;
        return false;
    }
    input.ignore(1);

    // potential whitespace
    ignoreWhitespace(input);

    // value
    std::shared_ptr<Object> value;
    bool result = parseValue(input, value);
    if(!result || !value)
    {
        std::cerr << "json: expected value for pair" << logIndex(input.tellg()) << std::endl;
        return false;
    }
    object.addObject(name, value);

    // potential whitespace
    ignoreWhitespace(input);

    // comma
    c = input.peek();
    if(c != ',')
    {
        if(c == '}')
        {
            ignoreWhitespace(input);
            return true;
        }
        std::cerr << "json: expected ',' or '}'" << logIndex(input.tellg()) << std::endl;
        return false;
    }
    input.ignore(1);

    // potential whitespace
    ignoreWhitespace(input);

    return true;
}

bool consumeNumber(JSONConverter::InputStreamType& input, std::string& object)
{
    char c = 0;
    c = input.peek();
    while(isdigit(c) || c == '-' || c == '+' || c == 'e' || c == 'E' || c == '.')
    {
        if(input.eof())
        {
            std::cerr << "json: unexpected EOF while parsing number" << logIndex(input.tellg()) << std::endl;
            return false;
        }

        object += c;
        input.ignore(1);

        c = input.peek();
    }
    return true;
}

bool parseFloat(JSONConverter::InputStreamType& input, double& object)
{
    std::string str;
    if(!consumeNumber(input, str))
    {
        std::cerr << "json: expected number" << logIndex(input.tellg()) << std::endl;
        return false;
    }

    try
    {
        object = std::stof(str);
        return true;
    }
    catch(...)
    {
        std::cerr << "json: invalid number" << logIndex(input.tellg()) << std::endl;
        return false;
    }
}

bool parseList(JSONConverter::InputStreamType& input, ObjectList& object)
{
    // starting '['
    char c = input.peek();
    if(c != '[')
    {
        std::cerr << "json: expected '['" << logIndex(input.tellg()) << std::endl;
        return false;
    }
    input.ignore(1);

    while(true)
    {
        // potential whitespace
        ignoreWhitespace(input);

        // potential ']'
        if(input.peek() == ']')
        {
            input.ignore(1);
            return true;
        }

        // value
        std::shared_ptr<Object> subObject;
        if(!parseValue(input, subObject))
        {
            std::cerr << "json: expected value for list" << logIndex(input.tellg()) << std::endl;
            return false;
        }
        if(!subObject)
        {
            std::cerr << "json: invalid subObject" << logIndex(input.tellg()) << std::endl;
            return false;
        }
        object.addObject(subObject);

        // potential whitespace
        ignoreWhitespace(input);

        // comma
        c = input.peek();
        if(c != ',')
        {
            if(c == ']')
            {
                input.ignore(1);
                return true;
            }
            std::cerr << "json: expected ',' or ']'" << logIndex(input.tellg()) << std::endl;
            return false;
        }
        input.ignore(1);

        // potential whitespace
        ignoreWhitespace(input);
    }

    return true;
}

bool parseMap(JSONConverter::InputStreamType& input, ObjectMap& object)
{
    // find first '{'
    char c = input.peek();
    if(c != '{')
    {
        std::cerr << "json: expected '{'" << logIndex(input.tellg()) << std::endl;
        return false;
    }
    input.ignore(1);

    ignoreWhitespace(input);

    // parse object as long as we encounter next '}'
    while(true)
    {
        c = input.peek();
        if(c == '}')
        {
            input.ignore(1);
            return true;
        }
        else if(c == '"')
        {
            bool result = parsePair(input, object);
            if(!result)
            {
                std::cerr << "json: expected key-value pair for map" << logIndex(input.tellg()) << std::endl;
                return false;
            }
        }
        else
        {
            std::cerr << "json: expected '}' or ','" << logIndex(input.tellg()) << std::endl;
            return false;
        }
    }
}

bool parseValue(JSONConverter::InputStreamType& input, std::shared_ptr<Object>& object)
{
    char c = input.peek();
    if(c == '"')
    {
        std::shared_ptr<ObjectString> object2 = make<ObjectString>("");
        std::string value;
        if(!parseString(input, value))
        {
            std::cerr << "json: expected string" << logIndex(input.tellg()) << std::endl;
            return false;
        }
        object2->setString(value);
        object = object2;
        return true;
    }
    else if(isdigit(c) || c == '-' || c == '+')
    {
        std::shared_ptr<ObjectFloat> object2 = make<ObjectFloat>(0.0);
        double value;
        if(!parseFloat(input, value))
        {
            std::cerr << "json: expected number" << logIndex(input.tellg()) << std::endl;
            return false;
        }
        object2->setNumber(value);
        object = object2;
        return true;
    }
    else if(c == '[')
    {
        std::shared_ptr<ObjectList> object2 = make<ObjectList>();
        if(!parseList(input, *object2))
        {
            std::cerr << "json: expected list" << logIndex(input.tellg()) << std::endl;
            return false;
        }
        object = object2;
        return true;
    }
    else if(c == '{')
    {
        std::shared_ptr<ObjectMap> object2 = make<ObjectMap>();
        if(!parseMap(input, *object2))
        {
            std::cerr << "json: expected map" << logIndex(input.tellg()) << std::endl;
            return false;
        }
        object = object2;
        return true;
    }
    std::cerr << "json: expected '\"', number, '[' or '{'" << logIndex(input.tellg()) << std::endl;
    return false;
}

bool JSONConverter::in(JSONConverter::InputStreamType& input, ObjectMap& object) const
{
    ignoreWhitespace(input);
    bool b = parseMap(input, object);
    ignoreWhitespace(input);
    if(!input.eof())
    {
        std::cerr << "json: expected EOF" << logIndex(input.tellg()) << std::endl;
        b = false;
    }
    if(!b)
        input.setstate(std::ios_base::failbit);
    return b;
}

bool JSONConverter::out(JSONConverter::OutputStreamType& output, const ObjectMap& object) const
{
    output << object.toString();
    return true;
}

}

