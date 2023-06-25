#pragma once
#include <stdio.h>

#ifdef _DEBUG
    #define ASSERT(condition, title, msg) \
        if (!(condition)) { \
            fprintf_s(stderr, "[%s]: %s\n[file]: %s (%d)\n[function]: %s\n\n", (title), (msg), __FILE__, __LINE__, __FUNCTION__); \
            abort(); \
        }
#else
    #define ASSERT(condition, title, msg)
#endif