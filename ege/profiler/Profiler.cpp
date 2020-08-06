/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Profiler.h"

#include <ege/main/Config.h>

namespace EGE
{

Profiler::Profiler()
{
    m_root.m_name = "root";
    m_root.m_depth = 0;
    m_root.m_time = 0;
}

Profiler::~Profiler() {}

void Profiler::startSection(std::string name)
{
    DBG(PROFILER_DEBUG, "--- START SECTION ---");
    endSectionLL(); //unspecified
    startSectionLL(name);
    startSectionLL("<unspecified>");
}

void Profiler::endSection()
{
    DBG(PROFILER_DEBUG, "--- END SECTION ---");
    endSectionLL(); //unspecified
    endSectionLL(); //current
    startSectionLL("<unspecified>");
}

void Profiler::startSectionLL(std::string name)
{
    ASSERT(m_root.m_started);

    Section* section = NULL;
    Section* parentSection = NULL;
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

    if(section == NULL)
    {
        section = new Section;
        section->m_name = name;
        section->m_time = 0LL;
        section->m_depth = m_startedSections.size() + 1;

        if(m_startedSections.empty())
        {
            m_root.m_subSections[name] = std::shared_ptr<Section>(section);
        }
        else
        {
            m_startedSections.top()->m_subSections[name] = std::shared_ptr<Section>(section);
        }
    }
    section->m_startTime = getTime();
    section->m_started = true;

    m_startedSections.push(section);
    DBG(PROFILER_DEBUG, "startSection " + name);
}
void Profiler::endSectionLL()
{
    ASSERT(m_root.m_started);
    if(m_startedSections.empty())
    {
        end();
    }
    else
    {
        Section* section = m_startedSections.top();
        ASSERT(section);

        section->m_started = false;
        section->m_time += getTime() - section->m_startTime;

        m_startedSections.pop();
        DBG(PROFILER_DEBUG, "endSection " + section->m_name);
    }
}

void Profiler::endStartSection(std::string name)
{
    DBG(PROFILER_DEBUG, "--- END START SECTION ---");
    endSectionLL(); //unspecified
    endSectionLL(); //current

    startSectionLL(name);
    startSectionLL("<unspecified>");
}

void Profiler::start()
{
    DBG(PROFILER_DEBUG, "--- START ---");
    m_root.m_started = true;
    m_root.m_startTime = getTime();
    startSectionLL("<unspecified>");
}

void Profiler::end()
{
    DBG(PROFILER_DEBUG, "--- END ---");
    m_root.m_started = false;
    m_root.m_time += getTime() - m_root.m_startTime;
    while(!m_startedSections.empty()) m_startedSections.pop();
}

std::string Profiler::toString()
{
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

long long Profiler::getTime()
{
    timespec _ts;
    if(clock_gettime(CLOCK_REALTIME, &_ts) < 0)
        return 0.0;

    long long time = _ts.tv_sec * 1000000000 + _ts.tv_nsec;
    DUMP(0, time);
    return time;
}

Profiler::Section* Profiler::Section::findSubSection(std::string name)
{
    auto it = m_subSections.find(name);
    if(it == m_subSections.end())
    {
        return NULL;
    }
    return it->second.get();
}

void Profiler::Section::addSectionInfo(std::string& info, long long parentTime, long long rootTime)
{
    // this section
    for(size_t s = 0; s < m_depth; s++)
    {
        if(s == m_depth - 1)
            info += "|- ";
        else
            info += "|  ";
    }
    if(parentTime != 0)
    {
        info += "\e[33m" + std::to_string(float(m_time) * 100 / parentTime) + "%: \e[0m";
    }
    info += "\e[1;32m" + m_name + "\e[0m";
    if(parentTime != 0)
    {
        info += " (\e[37;1m";
        info += std::to_string(m_time) + "\e[0m ns, ";
        info += "\e[37;1m" + std::to_string(float(m_time) * 100 / rootTime) + "%\e[0m of root)";
    }
    info += '\n';

    // empty space
    for(size_t s = 0; s < m_depth; s++)
    {
        info += "|  ";
    }
    if(!m_subSections.empty())
        info += "|  ";
    info += '\n';

    // subsections
    for(auto it: m_subSections)
    {
        it.second.get()->addSectionInfo(info, m_time, rootTime);
    }
}

}
