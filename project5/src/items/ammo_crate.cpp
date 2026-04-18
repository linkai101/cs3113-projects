#include "items/ammo_crate.h"

static int frameForGunType(Gun::Type type) {
  switch (type) {
    case Gun::Type::RIFLE: return 0;
    case Gun::Type::PISTOL: return 1;
    case Gun::Type::SHOTGUN: return 2;
  }
  return 0;
}

AmmoCrate::AmmoCrate(Vector2 position, Gun::Type gunType, int amount, Assets& assets) :
  Item(&assets.ammoCrateSheet, frameForGunType(gunType), RENDER_SIZE),
  gunType(gunType),
  amount(amount)
{
  placeOnGround(position);
}
