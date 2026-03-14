#include "spritesheet.h"

Rectangle Spritesheet::getFrame(int index) const {
  return {
    (index % columns) * frameSize.x,
    (index / columns) * frameSize.y,
    frameSize.x,
    frameSize.y
  };
}