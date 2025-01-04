#pragma once

#include <HLAM/hlam_math.h>

enum WolfState {
  IDLE,
  WANDER,
  CHASE,
};

class Wolf {
  // hlam::Vec2 target;
  // WolfState state;

 public:
  hlam::Vec2 pos;
  hlam::Vec2 size;
  hlam::Vec2 speed;
  hlam::Vec2 runSpeed;

  Wolf(hlam::Vec2 pos, hlam::Vec2 size, hlam::Vec2 speed, hlam::Vec2 runSpeed);
  void Update(float dt);
  void Draw();
};
