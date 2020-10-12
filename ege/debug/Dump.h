/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/util/ObjectMap.h>
#include <iostream>
#include <memory>
#include <string>

namespace EGE
{

struct HexDumpSettings
{
    size_t width = 16;
    std::ostream& stream = std::cerr;
};

void hexDump(const void* data, size_t size, HexDumpSettings settings = HexDumpSettings());

void printObject(std::shared_ptr<Object> object);

}
