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

#include <functional>

#include <ege/main/Config.h>

namespace EGE
{

class ComponentBase;

class Behaviour
{
public:
    Behaviour(ComponentBase& component)
    : m_component(component) {}

    virtual ~Behaviour() = default;

    Behaviour(Behaviour const&) = delete;
    Behaviour(Behaviour&&) = delete;
    Behaviour& operator=(Behaviour const&) = delete;
    Behaviour& operator=(Behaviour&&) = delete;

    virtual void onUpdate() = 0;

protected:
    ComponentBase& m_component;
};

class SimpleBehaviour : public Behaviour
{
public:
    // TODO: Don't allow creating this from any objects!!
    SimpleBehaviour(ComponentBase& component, std::function<void(ComponentBase&)> onupdate)
    : Behaviour(component), m_onUpdate(onupdate) { ASSERT(m_onUpdate); }

    virtual void onUpdate() override
    {
        m_onUpdate(m_component);
    }

private:
    std::function<void(ComponentBase&)> m_onUpdate;
};

}
