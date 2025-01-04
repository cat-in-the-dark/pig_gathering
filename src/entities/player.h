#pragma once

#include <raylib.h>
// raylib must be before
#include <HLAM/coroutines.h>
#include <HLAM/hlam_math.h>

#include "const.h"

class Player {
  KeyboardKey key_up;
  KeyboardKey key_down;
  KeyboardKey key_left;
  KeyboardKey key_right;
  KeyboardKey key_dash;

  hlam::Vec2 playerSpeedVec;
  hlam::Cooldown dashCooldown = {kDashCooldown};
  float dashPower = kDashPower;
  hlam::Vec2 lastControlsDir = {0, 0};

  void UpdateControls(float dt);

 public:
  static constexpr hlam::Vec2 physSize = {24, 32};

  int index;
  hlam::Vec2 pos;
  float playerSpeed;
  hlam::Timer dashAnim = {kDashTime, kDashTime};

  Player(int index, hlam::Vec2 pos, float playerSpeed);
  void Draw();
  void Update(float dt);
  bool IsDashing() const;
};
