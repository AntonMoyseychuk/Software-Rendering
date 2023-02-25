#pragma once
#include <iostream>

#define LOG(tag, msg) std::cerr << '[' << (tag) << "]\t" << (msg) << '\n'

#define LOG_WIN_ERROR(condition, tag, msg) if (!(condition)) { \
    LOG((tag), (msg)); \
    exit(-1); \
}

#define LOG_WIN_INFO(msg) LOG("window info", (msg))

#define LOG_WIN_EVENT(event_type, msg) LOG("event", "type: " + std::string(event_type) + ". " + msg)