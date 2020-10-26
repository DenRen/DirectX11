#pragma once

#include <cstdio>

#define _MY_DEBUG

#ifdef _MY_DEBUG											
	#define RETURN_FALSE									\
		printf ("FILE: %s, LINE: %d\n", __FILE__, __LINE__);\
		return false
#else
	#define RETURN_FALSE									\
		return false
#endif
	
#ifdef _MY_DEBUG											
#define RETURN_VOID_FALSE									\
		printf ("FILE: %s, LINE: %d\n", __FILE__, __LINE__);\
		return
#else
#define RETURN_VOID_FALSE									\
		return
#endif

#define RELEASE(res)        \
    if (res != nullptr)     \
    {                       \
        res->Release ();    \
        res = nullptr;      \
    }

#define CHECK_FAILED(hr)    \
    if (FAILED (hr))        \
    {                       \
        perror ("");        \
        RETURN_FALSE;       \
    }