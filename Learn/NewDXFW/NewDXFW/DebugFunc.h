#pragma once

#include <cerrno>
#include <stdexcept>
#include "Config.h"

void NOP ();
void PrintLogInfo (const char file[], int line, bool inFile, bool end = false);
void DebugEndMain ();

#define RELEASE(res)        \
    if (res != nullptr)     \
    {                       \
        res->Release ();    \
        res = nullptr;      \
    }

#ifdef TURN_DEBUG
    #define DUMP_DEBUG_INFO     PrintLogInfo (__FILE__, __LINE__, false, false)
    #define END_DUMP_DEBUG_INFO	PrintLogInfo (__FILE__, __LINE__, false, true)
#else
    #define DUMP_DEBUG_INFO     PrintLogInfo (__FILE__, __LINE__, true, false)
    #define END_DUMP_DEBUG_INFO	PrintLogInfo (__FILE__, __LINE__, true, true)
#endif

#define RETURN_FALSE    \
{                       \
    DUMP_DEBUG_INFO;    \
    return false;       \
} NOP ()

#define CHECK_FAILED(val)   \
    if (FAILED (val))       \
    {                       \
        RETURN_FALSE;       \
    }

#define CHECK_NULLPTR(val)  \
    if (val == nullptr)     \
    {                       \
        RETURN_FALSE;       \
    }