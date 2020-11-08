#pragma once

#include <cstdio>
#include <cerrno>
#define _MY_DEBUG

#ifdef _MY_DEBUG
    #define PRINT_DEBUG_INFO                                    \
        {                                                       \
            perror ("");                                        \
            printf ("FILE: %s, LINE: %d\n", __FILE__, __LINE__);\
        }
#else
    #define PRINT_DEBUG_INFO  
#endif

#ifdef _MY_DEBUG											
#define RETURN_FALSE	    \
		PRINT_DEBUG_INFO    \
		return false
#else
#define RETURN_FALSE		 \
		return false
#endif

#ifdef _MY_DEBUG											
#define RETURN_VOID_FALSE   \
		PRINT_DEBUG_INFO    \
		return
#else
#define RETURN_VOID_FALSE	\
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