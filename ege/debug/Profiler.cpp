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

#include "Profiler.h"

#include <algorithm>
#include <ege/debug/Logger.h>
#include <ege/main/Config.h>
#include <ege/util/GlobalConfig.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/system.h>
#include <vector>

#define PROFILER_DEBUG 0

namespace EGE
{

Profiler::Profiler()
{
    m_root.m_name = "root";
    m_root.m_depth = 0;
    m_root.m_time = 0;
}

Profiler::~Profiler() {}

void Profiler::start()
{
    if(!EGE_GCONFIG_IS_SET(Profiler_Enable)) return;
    if(m_root.m_started) return;
    DBG(PROFILER_DEBUG, "--- START ---");
    m_root.m_started = true;
    m_root.m_startTime = getTime();
}

void Profiler::end()
{
    if(!EGE_GCONFIG_IS_SET(Profiler_Enable)) return;
    if(!m_root.m_started) return;
    DBG(PROFILER_DEBUG, "--- END ---");
    m_root.m_started = false;
    m_root.m_time += getTime() - m_root.m_startTime;
    while(!m_startedSections.empty()) m_startedSections.pop();
}

void Profiler::startSection(std::string const& name)
{
    if(!EGE_GCONFIG_IS_SET(Profiler_Enable)) return;
    //DBG(PROFILER_DEBUG, "--- START SECTION ---");
    startSectionLL(name);
}

void Profiler::endSection()
{
    if(!EGE_GCONFIG_IS_SET(Profiler_Enable)) return;
    //DBG(PROFILER_DEBUG, "--- END SECTION ---");
    endSectionLL(); //current
}

void Profiler::endStartSection(std::string const& name)
{
    if(!EGE_GCONFIG_IS_SET(Profiler_Enable)) return;
    //DBG(PROFILER_DEBUG, "--- END START SECTION ---");
    endSectionLL(); //current
    startSectionLL(name);
}

void Profiler::startSectionLL(std::string const& name)
{
    if(!EGE_GCONFIG_IS_SET(Profiler_Enable)) return;
    if constexpr(PROFILER_DEBUG) ege_log.info() << "Profiler::startSectionLL " << name << " depth=" << m_startedSections.size();
    ASSERT(m_root.m_started);

    Section* section = nullptr;
    Section* parentSection = nullptr;
    if(m_startedSections.empty())
    {
        parentSection = &m_root;
        section = m_root.findSubSection(name);
    }
    else
    {
        parentSection = m_startedSections.top();
        section = parentSection->findSubSection(name);
    }

    if(section == nullptr)
    {
        auto newSection = makeUnique<Section>();
        section = newSection.get();
        section->m_name = name;
        section->m_time = 0LL;
        section->m_depth = m_startedSections.size() + 1;

        if(m_startedSections.empty())
        {
            m_root.m_subSections[name] = std::move(newSection);
        }
        else
        {
            m_startedSections.top()->m_subSections[name] = std::move(newSection);
        }
    }
    section->m_startTime = getTime();
    section->m_started = true;

    m_startedSections.push(section);
}

void Profiler::endSectionLL()
{
    if(!EGE_GCONFIG_IS_SET(Profiler_Enable)) return;
    if constexpr(PROFILER_DEBUG) ege_log.info() << "Profiler::endSectionLL depth=" << m_startedSections.size();
    ASSERT(m_root.m_started);
    ASSERT(!m_startedSections.empty());

    Section* section = m_startedSections.top();
    ASSERT(section);

    section->m_started = false;
    section->m_time += getTime() - section->m_startTime;

    m_startedSections.pop();
}

std::string Profiler::toString() const
{
    // TODO: Switch to printTree()
    std::string str = "---- \e[1;33mEGE::Profiler results\e[0m ----\n\n";
    if(m_root.m_started || m_root.m_time > 0)
    {
        m_root.addSectionInfo(str, m_root.m_time, m_root.m_time);
    }
    else
    {
        str += "Profiler not started!\n";
    }
    return str;
}

long long Profiler::getTime() const
{
    return System::exactTime().nanoseconds();
}

Profiler::Section* Profiler::Section::findSubSection(std::string const& name)
{
    auto it = m_subSections.find(name);
    if(it == m_subSections.end())
        return nullptr;
    return it->second.get();
}

void Profiler::Section::addSectionInfo(std::string& info, long long parentTime, long long rootTime) const
{
    // this section
    for(int s = 0; s < m_depth; s++)
    {
        if(s == m_depth - 1)
            info += "|- ";
        else
            info += "|  ";
    }
    if(parentTime != 0)
    {
        info += "\e[33m" + std::to_string(float(m_time) * 100 / rootTime) + "% (" + std::to_string(float(m_time) * 100 / parentTime) + "%) \e[0m";
    }
    info += "\e[1;32m" + m_name + "\e[0m";
    if(parentTime != 0)
    {
        info += " (\e[37;1m";
        info += std::to_string(m_time) + "\e[0m ns)";
    }
    info += '\n';

    std::vector<Profiler::Section*> sections;

    // subsections
    for(auto& it: m_subSections)
        sections.push_back(it.second.get());

    std::sort(sections.begin(), sections.end(), [](Profiler::Section* _1, Profiler::Section* _2) { return _1->m_time > _2->m_time; } );

    for(auto section: sections)
        section->addSectionInfo(info, m_time, rootTime);
}

SharedPtr<ObjectMap> Profiler::Section::serialize() const
{
    SharedPtr<ObjectMap> map = make<ObjectMap>();

    // this section
    map->addObject("time", make<ObjectInt>(m_time));

    // sub sections
    std::vector<Profiler::Section*> sections;
    for(auto& it: m_subSections)
        sections.push_back(it.second.get());

    // FIXME: There is no point to sort here since ObjectMap does not
    // store information about element order
    std::sort(sections.begin(), sections.end(), [](Profiler::Section* _1, Profiler::Section* _2) { return _1->m_time > _2->m_time; } );
    SharedPtr<ObjectMap> sectionMap = make<ObjectMap>();

    for(auto& section: sections)
    {
        sectionMap->addObject(section->m_name, section->serialize());
    }
    map->addObject("sections", sectionMap);
    return map;
}

bool Profiler::Section::deserialize(SharedPtr<ObjectMap>)
{
    CRASH();
}

SharedPtr<ObjectMap> Profiler::serialize() const
{
    SharedPtr<ObjectMap> map = make<ObjectMap>();
    map->addObject("root", m_root.serialize());
    return map;
}

bool Profiler::deserialize(SharedPtr<ObjectMap>)
{
    CRASH();
}

}
