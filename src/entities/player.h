#pragma once

#include <raylib.h>
// raylib must be before
#include <HLAM/coroutines.h>
#include <HLAM/hlam_math.h>

#include "const.h"

class Player {
  hlam::Vec2 playerSpeedVec;
  hlam::Vec2 physSize = {24, 32};
  hlam::Timer dashAnim = {kDashTime, kDashTime};
  hlam::Cooldown dashCooldown = {kDashCooldown};
  float dashPower = kDashPower;
  hlam::Vec2 lastControlsDir = {0, 0};

  void UpdateControls(float dt);

 public:
  int index;
  hlam::Vec2 pos;
  float playerSpeed;

  Player(int index, hlam::Vec2 pos, float playerSpeed);
  void Draw();
  void Update(float dt);
};
