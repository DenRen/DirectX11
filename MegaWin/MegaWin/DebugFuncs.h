#pragma once

#include <cerrno>
#include "Config.h"

void NOP ();
void PrintLogInfo (const char file[], int line, bool inFile);

#ifdef TURN_DEBUG
	#define DUMP_DEBUG_INFO	PrintLogInfo (__FILE__, __LINE__, false)
#else
	#define DUMP_DEBUG_INFO PrintLogInfo (__FILE__, __LINE__, true)
#endif