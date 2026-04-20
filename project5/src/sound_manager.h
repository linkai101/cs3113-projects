#pragma once
#include "raylib.h"
#include "assets.h"

enum class SFX {
  PUNCH,
  BAT_SWING,
  PISTOL_FIRE,
  RIFLE_FIRE,
  SHOTGUN_FIRE,
  GUN_EMPTY,
  RELOAD_MAG,
  RELOAD_END_MAG,
  RELOAD_SHELL,
  PICKUP,
  BANDAGE,
};

enum class MusicTrack {
  AMBIENT_WIND,
};

class SoundManager {
public:
  static SoundManager& get() {
    static SoundManager instance;
    return instance;
  }

  void init(Assets* assets) {
    this->assets = assets;
  }

  // Call once per frame to keep the active music stream buffered.
  void update() {
    if (currentTrack) UpdateMusicStream(*currentTrack);
  }

  void play(SFX sfx) {
    switch (sfx) {
      case SFX::PUNCH: PlaySound(assets->punchSound); break;
      case SFX::BAT_SWING: PlaySound(assets->batSwingSound); break;
      case SFX::PISTOL_FIRE: PlaySound(assets->pistolFireSound); break;
      case SFX::RIFLE_FIRE: PlaySound(assets->rifleFireSound); break;
      case SFX::SHOTGUN_FIRE: PlaySound(assets->shotgunFireSound); break;
      case SFX::GUN_EMPTY: PlaySound(assets->gunEmptySound); break;
      case SFX::RELOAD_MAG: PlaySound(assets->reloadMagSound); break;
      case SFX::RELOAD_END_MAG: PlaySound(assets->reloadEndMagSound); break;
      case SFX::RELOAD_SHELL: PlaySound(assets->reloadShellSound); break;
      case SFX::PICKUP: PlaySound(assets->pickupSound); break;
      case SFX::BANDAGE: PlaySound(assets->bandageSound); break;
    }
  }

  void playMusic(MusicTrack track) {
    if (currentTrack) StopMusicStream(*currentTrack);
    currentTrack = &musicStream(track);
    PlayMusicStream(*currentTrack);
  }

  void stopMusic() {
    if (currentTrack) {
      StopMusicStream(*currentTrack);
      currentTrack = nullptr;
    }
  }

private:
  SoundManager() = default;

  Music& musicStream(MusicTrack track) {
    switch (track) {
      case MusicTrack::AMBIENT_WIND: return assets->ambientWindMusic;
    }
  }

  Assets* assets = nullptr;
  Music* currentTrack = nullptr;
};
