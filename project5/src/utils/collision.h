#pragma once
#include "raylib.h"

bool CheckPointInRect(Vector2 point, const Rectangle& rect);

bool CheckRectCollision(const Rectangle& a, const Rectangle& b);
