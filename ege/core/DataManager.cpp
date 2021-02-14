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

#include "DataManager.h"

#include <ege/debug/Logger.h>
#include <ege/util/Converter.h>

#include <fstream>

namespace EGE
{

const ObjectMap::_Object DataManager::getValue(String key) const
{
    if(!m_data)
        return {};

    return m_data->getObject(key);
}

void DataManager::setValue(String key, SharedPtr<Object> value)
{
    if(!m_data)
        return;

    m_data->addObject(key, value);
    m_dirty = true;
}

bool DataManager::save(const IOStreamConverter& converter)
{
    if(!m_dirty)
        return true;

    if(!m_data)
        return false;

    std::ofstream file(m_fileName);
    if(!file.good())
    {
        err() << "DataManager: Failed to open file";
        return false;
    }

    if(!(file << objectOut(*m_data, converter)))
    {
        err() << "DataManager: Failed to generate JSON";
        return false;
    }

    m_dirty = false;
    return true;
}

bool DataManager::load(const IOStreamConverter& converter)
{
    m_error = true;
    std::ifstream file(m_fileName);
    if(!file.good())
    {
        err() << "DataManager: Failed to open file";
        return false;
    }

    SharedPtr<Object> object;
    if(!(file >> objectIn(object, converter)))
    {
        err() << "DataManager: Failed to parse JSON";
        return false;
    }

    auto data = Object::cast<ObjectMap>(object);
    if(!data.hasValue())
    {
        err() << "DataManager: Failed to load JSON map";
        return false;
    }

    m_data = data.value();
    m_error = false;
    return true;
}

}

