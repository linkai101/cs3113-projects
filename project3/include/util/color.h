#pragma once
#include "raylib.h"

/**
 * Converts a hex color string like "#RRGGBB" or "RRGGBBAA" to a raylib Color.
 * Falls back to RAYWHITE if parsing fails.
 * @param hex The hex color string to convert.
 * @return The converted Color.
 */
Color ColorFromHex(const char* hex);
