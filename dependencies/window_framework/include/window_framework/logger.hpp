#pragma once
#include <iostream>

#define LOG(tag, msg) std::cerr << '[' << (tag) << "] " << (msg) << '\n'

#define LOG_ERROR(condition, tag, msg) if (!(condition)) { \
    LOG((tag), (msg)); \
    exit(-1); \
}