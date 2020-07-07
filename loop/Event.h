/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <string>

#define EGE_EVENT(type) \
public: \
    static EGE::Event::EventType getTypeStatic() { return type; } \
    virtual EGE::Event::EventType getType() { return getTypeStatic(); } \

namespace EGE
{

class Event
{
public:
    typedef std::string EventType;
    EGE_EVENT("EGE::Event (unknown)");

    bool isCanceled() { return m_canceled; }
    void cancel() { m_canceled = true; }

private:
    bool m_canceled;
};

}
