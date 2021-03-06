/*
EGE - Extentable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

class Event;

template<class T>
class EventCast
{
	T* m_value;
public:
	template<class U>
	EventCast(U* u)
	: m_value(dynamic_cast<T*>(u))
	{
		ASSERT(dynamic_cast<Event*>(u));
	}

	operator T*()
	{
	    ASSERT(m_value);
		return m_value;
	}
	T* operator->()
	{
	    ASSERT(m_value);
	    return m_value;
	}
	T& operator*()
	{
	    ASSERT(m_value);
	    return *m_value;
	}
};

}
