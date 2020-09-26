/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ObjectString.h"

#include <ege/main/Config.h>

namespace EGE
{

std::shared_ptr<Object> ObjectString::copy() const
{
    return make<ObjectString>(*this);
}

std::string ObjectString::toString() const
{
    std::string str;

    // escape string
    if(!m_string.empty())
    {
        for(size_t s = 0; s < m_string.size(); s++)
        {
            const char c = m_string[s];
            switch(c)
            {
            case '\n':
                str += "\\n";
                break;
            case '\t':
                str += "\\t";
                break;
            case '\\':
            case '"':
                str += "\\";
                str += c;
                break;
            default:
                str += c;
                break;
            }
        }
    }

    return "\"" + str + "\"";
}

}
