/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "SystemImpl.h"

#include <memory>

namespace EGE
{

namespace System
{

extern std::unique_ptr<Internal::SystemImpl> g_impl;
Internal::SystemImpl g_defaultImpl;

Internal::SystemImpl& impl()
{
    return g_impl ? *g_impl : g_defaultImpl;
}

} // System

}
