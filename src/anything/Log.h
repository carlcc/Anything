#pragma once

#include <fmt/format.h>

#define INFO(fmtStr, ...)  fmt::print("[INFO ][{}:{}:{}]: {}\n", __FILE__, __LINE__, __FUNCTION__, fmt::format(fmtStr, ##__VA_ARGS__))
#define WARN(fmtStr, ...)  fmt::print("[WARN ][{}:{}:{}]: {}\n", __FILE__, __LINE__, __FUNCTION__, fmt::format(fmtStr, ##__VA_ARGS__))
#define FATAL(fmtStr, ...)  fmt::print("[ERROR][{}:{}:{}]: {}\n", __FILE__, __LINE__, __FUNCTION__, fmt::format(fmtStr, ##__VA_ARGS__))

#ifdef NDEBUG
#define DEBUG(...)
#else
#define DEBUG(fmtStr, ...)  fmt::print("[DEBUG][{}:{}:{}]: {}\n", __FILE__, __LINE__, __FUNCTION__, fmt::format(fmtStr, ##__VA_ARGS__))
#endif