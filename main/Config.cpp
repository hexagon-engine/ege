/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Config.h"

#include <stdio.h>

void _ege_assertion_failed(const char* expr, const char* file, size_t line)
{
    fprintf(stderr, "0003 EGE/main: \e[31;1mASSERTION FAILED:\e[0m [ %s ] -- In \e[33m%s\e[0m:\e[95m%d\e[0m\n", expr, file, line);
    exit(1234);
}
