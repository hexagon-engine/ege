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

#include <cstddef>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <ege/util/TrackedSharedPtr.h>

namespace EGE
{

//////////////////////////////////////////////////////
// Primitives compatible with serialization system. //
//////////////////////////////////////////////////////

// Signed integers
typedef char      Int8;
typedef short     Int16;
typedef int       Int32;
typedef long long Int64;
typedef Int64     MaxInt;

// Unsigned integers
typedef unsigned char      Uint8;
typedef unsigned short     Uint16;
typedef unsigned int       Uint32;
typedef unsigned long long Uint64;
typedef Uint64             MaxUint;

// Boolean and floating-point
typedef float       Float;      // 4B
typedef double      Double;     // 8B
typedef long double LongDouble; // 12B
typedef LongDouble  MaxFloat;

typedef bool        Boolean;    // 1B

// Strings
typedef std::string String;
typedef std::wstring WString;
typedef std::basic_string<Uint32> UString; // Unicode String

// String Views
using StringView = std::string_view;
using UStringView = std::basic_string_view<Uint32>; // Unicode StringView

using namespace std::literals;

/////////////////
// Other types //
/////////////////

typedef size_t      Size;
typedef size_t      IdType;
typedef long long   TickCount;
typedef long long   UidType;

//////////////
// Pointers //
//////////////

template<class T>
using FlatPtr = T*;

template<class T>
using UniquePtr = std::unique_ptr<T>;

#define SHARED_PTR_TRACKING

template<class T>
#ifdef SHARED_PTR_TRACKING
    using SharedPtr = TrackedSharedPtr<T>;
#else
    using SharedPtr = std::shared_ptr<T>;
#endif

template<class T>
using WeakPtr = std::weak_ptr<T>;

template<class T>
using FlatRef = T&;

template<class T>
using RValueRef = T&&;

////////////////
// Containers //
////////////////

template<class T>
using FlatArray = T[];

template<class T, size_t S>
using Array = std::array<T, S>;

template<class T>
using Vector = std::vector<T>;

template<class K, class V>
using PairVector = std::vector<std::pair<K, V>>;

// Used for Object system
template<class T>
using SharedPtrVector = Vector<SharedPtr<T>>;

template<class T>
using List = std::list<T>;

template<class T>
using Set = std::set<T>;

template<class T>
using UnorderedSet = std::unordered_set<T>;

typedef Set<String> StringSet;
typedef Set<IdType> IdSet;

typedef UnorderedSet<String> UnorderedStringSet;
typedef UnorderedSet<IdType> UnorderedIdSet;

template<class K, class V>
using Map = std::map<K,V>;

template<class K, class V>
using UnorderedMap = std::unordered_map<K,V>;

template<class V>
using StringMap = Map<String, V>;

template<class V>
using UnorderedStringMap = UnorderedMap<String, V>;

template<class K, class V>
using SharedPtrMap = Map<K, SharedPtr<V>>;

template<class K, class V>
using UnorderedSharedPtrMap = UnorderedMap<K, SharedPtr<V>>;

// Used for Object system
template<class V>
using SharedPtrStringMap = Map<String, SharedPtr<V>>;

template<class K, class V>
using UnorderedSharedPtrStringMap = UnorderedMap<K, SharedPtr<V>>;

template<class V>
using IdMap = Map<IdType, V>;

template<class... Args>
using Function = std::function<Args...>;

}
