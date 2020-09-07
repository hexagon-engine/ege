/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Dump.h"

#include <iomanip>

namespace EGE
{

void hexDump(const void* data, size_t size, HexDumpSettings settings)
{
    std::ostream& _str = settings.stream;

    for(size_t s = 0; s < size / settings.width + 1; s++)
    {
        _str << std::hex << std::setfill('0') << std::setw(8) << s * settings.width << "   ";

        // data as HEX DUMP
        for(size_t t = 0; t < settings.width; t++)
        {
            size_t off = s * settings.width + t;
            if(off < size)
            {
                unsigned char _data = ((unsigned char*)data)[off] & 0xFF;
                _str << std::hex << std::setfill('0') << std::setw(2) << (int)_data << " ";
            }
            else
            {
                _str << "   ";
            }
        }

        _str << "  ";

        // and as CHARACTERS
        for(size_t t = 0; t < settings.width; t++)
        {
            size_t off = s * settings.width + t;
            if(off < size)
            {
                unsigned char _data = ((unsigned char*)data)[off] & 0xFF;
                if(_data < 32 || _data >= 127)
                    _data = '.';
                _str << _data << " ";
            }
        }
        _str << std::dec << std::endl;
    }
}

}
