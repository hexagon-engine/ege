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

#include <ege/main/Config.h>
#include <iostream>
#include <memory>
#include <sstream>

namespace EGE
{

size_t ignoreWhitespace(std::istringstream& input)
{
    size_t counter = 0;
    while(isspace(input.peek()))
    {
        input.ignore(1);
        counter++;
    }
    return counter;
}

bool parseValue(std::istringstream& input, std::shared_ptr<Object>& object);

bool parseString(std::istringstream& input, std::string& object)
{
    char c = input.peek();
    if(c != '"')
        return false;
    input.ignore(1);

    std::getline(input, object, '"');

    if(input.eof())
        return false;
    return true;
}

bool parsePair(std::istringstream& input, ObjectMap& object)
{
    // name
    std::string name;
    if(!parseString(input, name))
    {
        return false;
    }
    DUMP(1, name);

    // potential whitespace
    ignoreWhitespace(input);

    // colon
    char c = input.peek();
    if(c != ':')
        return false;
    input.ignore(1);

    // potential whitespace
    ignoreWhitespace(input);

    // value
    std::shared_ptr<Object> value;
    bool result = parseValue(input, value);
    if(!result)
        return false;
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
        return false;
    }
    input.ignore(1);

    // potential whitespace
    ignoreWhitespace(input);

    return true;
}

bool consumeNumber(std::istringstream& input, std::string& object)
{
    char c = 0;
    c = input.peek();
    while(isdigit(c) || c == '-' || c == '+' || c == 'e' || c == 'E' || c == '.')
    {
        if(input.eof())
            return false;

        object += c;
        input.ignore(1);

        c = input.peek();
    }
    return true;
}

bool parseFloat(std::istringstream& input, double& object)
{
    std::string str;
    if(!consumeNumber(input, str))
        return false;

    try
    {
        object = std::stof(str);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool parseList(std::istringstream& input, ObjectList& object)
{
    // starting '['
    char c = input.peek();
    if(c != '[')
        return false;
    input.ignore(1);

    while(true)
    {
        // potential whitespace
        ignoreWhitespace(input);

        // value
        std::shared_ptr<Object> subObject;
        if(!parseValue(input, subObject))
            return false;
        if(!subObject)
            return false;
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
            return false;
        }
        input.ignore(1);

        // potential whitespace
        ignoreWhitespace(input);
    }

    return true;
}

bool parseMap(std::istringstream& input, ObjectMap& object)
{
    // find first '{'
    char c = input.peek();
    if(c != '{')
        return false;
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
            parsePair(input, object);
        }
        else
        {
            return false;
        }
    }
}

bool parseValue(std::istringstream& input, std::shared_ptr<Object>& object)
{
    char c = input.peek();
    if(c == '"')
    {
        std::shared_ptr<ObjectString> object2 = make<ObjectString>("");
        std::string value;
        if(!parseString(input, value))
            return false;
        object2->setString(value);
        object = object2;
        return true;
    }
    else if(isdigit(c) || c == '-' || c == '+')
    {
        std::shared_ptr<ObjectFloat> object2 = make<ObjectFloat>(0.0);
        double value;
        if(!parseFloat(input, value))
            return false;
        object2->setNumber(value);
        object = object2;
        return true;
    }
    else if(c == '[')
    {
        std::shared_ptr<ObjectList> object2 = make<ObjectList>();
        if(!parseList(input, *object2))
            return false;
        object = object2;
        return true;
    }
    else if(c == '{')
    {
        std::shared_ptr<ObjectMap> object2 = make<ObjectMap>();
        if(!parseMap(input, *object2))
            return false;
        object = object2;
        return true;
    }
    return false;
}

bool JSONConverter::in(std::istringstream& input, ObjectMap& object) const
{
    bool b = parseMap(input, object);
    if(!b)
        input.setstate(std::ios_base::failbit);
    return b;
}

bool JSONConverter::out(std::ostringstream& output, const ObjectMap& object) const
{
    output << object.toString();
    return true;
}

}

