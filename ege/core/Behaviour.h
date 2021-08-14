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

#pragma once

#include <ege/core/EventTarget.h>
#include <ege/main/Config.h>
#include <ege/util/Time.h>

#include <functional>

namespace EGE
{

class ComponentBase;

namespace Internal
{

class _BehaviourBase : public EventTarget
{
public:
    _BehaviourBase(ComponentBase& component)
    : m_component(component) {}

    _BehaviourBase(_BehaviourBase const&) = delete;
    _BehaviourBase(_BehaviourBase&&) = delete;
    _BehaviourBase& operator=(_BehaviourBase const&) = delete;
    _BehaviourBase& operator=(_BehaviourBase&&) = delete;

    virtual ~_BehaviourBase() = default;

    virtual void onUpdate() = 0;

protected:
    ComponentBase& m_component;
};

}

template<class T>
class Behaviour : public Internal::_BehaviourBase
{
public:
    using ComponentType = T;

    Behaviour(T& component)
    : Internal::_BehaviourBase(component) {}

protected:
    T& component() { return static_cast<T>(m_component); }
};

template<class T>
class SimpleBehaviour : public Behaviour<T>
{
public:
    SimpleBehaviour(T& component, std::function<void(T&)> onupdate)
    : Behaviour<T>(component), m_onUpdate(onupdate) { ASSERT(m_onUpdate); }

    virtual void onUpdate() override
    {
        m_onUpdate(static_cast<T&>(this->m_component));
    }

private:
    std::function<void(T&)> m_onUpdate;
};

}
