#pragma once
#include <iostream>

/**
 * Logging macro that supports printf-style formatting.
 * Usage: LOG("Value: %d", x); or LOG("Hello");
 */
#define LOG(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)