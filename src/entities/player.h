#pragma once

#include <raylib.h>
// raylib must be before
#include <HLAM/hlam_math.h>

class Player {
  hlam::Vec2 playerSpeedVec;
  hlam::Vec2 physSize = {24, 32};

  void UpdateControls(float dt);

 public:
  int index;
  hlam::Vec2 pos;
  float playerSpeed;

  Player(int index, hlam::Vec2 pos, float playerSpeed);
  void Draw();
  void Update(float dt);
};
