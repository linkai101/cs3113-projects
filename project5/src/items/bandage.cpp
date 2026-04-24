#include "items/bandage.h"

Bandage::Bandage(Vector2 position, Assets& assets) :
  Item(assets.bandagePickableTexture, RENDER_SIZE)
{
  placeOnGround(position);
}