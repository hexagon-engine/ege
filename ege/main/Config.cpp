/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Config.h"

#include <stdio.h>

void _ege_assertion_failed(const char* expr, const char* message, const char* file, unsigned line)
{
    fprintf(stderr, "\e[31;1mASSERTION FAILED:\e[0m %s [ %s ] -- In \e[33m%s\e[0m:\e[95m%d\e[0m\n", message, expr, file, line);
    abort();
}
