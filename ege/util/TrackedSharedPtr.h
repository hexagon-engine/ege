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

#include <ege/util/StringUtils.h>

#include <iostream>
#include <memory>

namespace EGE
{

namespace Internal
{

class _TrackedSharedPtrBase
{
protected:
    enum MessageType
    {
        DefaultConstructed,
        AssignedNullptr,
        AssignedSharedPtr,
        AssignedWeakPtr,
        CopyAssigned,
        MoveAssigned,
        CastAssigned,
        Destructed
    };

    void printMessage(MessageType msgType, std::string_view typeof_);

    virtual void* getFlat() const = 0;
    virtual size_t refCount() const = 0;
};

}

template<class T>
class TrackedSharedPtr : Internal::_TrackedSharedPtrBase
{
public:
    // Default constructor
    TrackedSharedPtr()
    {
        printMessage(MessageType::DefaultConstructed, typeof_<T>());
    }

    // Null constructor
    TrackedSharedPtr(std::nullptr_t val) { operator=(val); }
    TrackedSharedPtr& operator=(std::nullptr_t)
    {
        printMessage(MessageType::AssignedNullptr, typeof_<T>());
        return *this;
    }

    // Copy constructor/assignment
    TrackedSharedPtr(TrackedSharedPtr<T> const& ptr) { *this = ptr; }

    TrackedSharedPtr& operator=(TrackedSharedPtr<T> const& ptr)
    {
        if(this == &ptr)
            return *this;
        m_ptr = ptr.m_ptr;
        printMessage(MessageType::CopyAssigned, typeof_<T>());
        return *this;
    }

    // Move constructor/assignment
    TrackedSharedPtr(TrackedSharedPtr<T>&& ptr) { *this = std::move(ptr); }

    TrackedSharedPtr& operator=(TrackedSharedPtr<T>&& ptr)
    {
        if(this == &ptr)
            return *this;
        m_ptr = std::move(ptr.m_ptr);
        printMessage(MessageType::MoveAssigned, typeof_<T>());
        return *this;
    }

    // Constructor/assignment with cast
    template<class U>
    TrackedSharedPtr(TrackedSharedPtr<U> const& ptr) { *this = ptr; }

    template<class U>
    TrackedSharedPtr& operator=(TrackedSharedPtr<U> const& ptr)
    {
        m_ptr = std::static_pointer_cast<T>(ptr.stdPtr());
        printMessage(MessageType::CastAssigned, typeof_<T>());
        return *this;
    }

    // Destructor
    ~TrackedSharedPtr()
    {
        printMessage(MessageType::Destructed, typeof_<T>());
    }

    // Constructor from shared_ptr
    TrackedSharedPtr(std::shared_ptr<T> const& ptr) { *this = ptr; }

    TrackedSharedPtr& operator=(std::shared_ptr<T> const& ptr)
    {
        m_ptr = ptr;
        printMessage(MessageType::AssignedSharedPtr, typeof_<T>());
        return *this;
    }

    // Constructor from weak_ptr
    TrackedSharedPtr(std::weak_ptr<T> const& ptr) { *this = ptr; }

    TrackedSharedPtr& operator=(std::weak_ptr<T> const& ptr)
    {
        m_ptr = ptr;
        printMessage(MessageType::AssignedWeakPtr, typeof_<T>());
        return *this;
    }

    T* get() const { return m_ptr.get(); }
    virtual void* getFlat() const override { return get(); }
    virtual size_t refCount() const override { return m_ptr.use_count(); }
    T& operator*() const { return *get(); }
    T* operator->() const { return get(); }

    bool operator!() const { return !m_ptr; }
    operator bool() const { return !!m_ptr; }

    bool operator==(TrackedSharedPtr<T> const& other) { return m_ptr == other.m_ptr; }
    bool operator!=(TrackedSharedPtr<T> const& other) { return m_ptr != other.m_ptr; }

    operator std::weak_ptr<T>() const { return m_ptr; }

    std::shared_ptr<T>& stdPtr() { return m_ptr; }
    std::shared_ptr<T> const& stdPtr() const { return m_ptr; }

private:
    std::shared_ptr<T> m_ptr;
};

}

namespace std
{

template<class T, class U>
EGE::TrackedSharedPtr<T> static_pointer_cast(EGE::TrackedSharedPtr<U> const& other)
{
    return EGE::TrackedSharedPtr<T>(other);
}

template<class T, class U>
EGE::TrackedSharedPtr<T> dynamic_pointer_cast(EGE::TrackedSharedPtr<U> const& other)
{
    return EGE::TrackedSharedPtr<T>(std::dynamic_pointer_cast<T>(other.stdPtr()));
}

}
