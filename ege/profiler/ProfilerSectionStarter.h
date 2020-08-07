/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Profiler.h"
#include <SFML/System/Lock.hpp>

namespace EGE
{

class ProfilerSectionStarter
{
    Profiler& m_profiler;
public:
    ProfilerSectionStarter(Profiler& profiler, std::string sectionName)
    : m_profiler(profiler)
    {
        m_profiler.startSection(sectionName);
    }
    ~ProfilerSectionStarter()
    {
        m_profiler.endSection();
    }

private:
    ProfilerSectionStarter& operator=(const ProfilerSectionStarter&) = delete;
    ProfilerSectionStarter(const ProfilerSectionStarter&) = delete;
};

}
