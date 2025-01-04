#pragma once

#include <HLAM/hlam_math.h>

class Truck {
 public:
  hlam::Vec2 pos;
  hlam::Vec2 size;
  hlam::Vec2 speed;

  Truck(hlam::Vec2 pos, hlam::Vec2 size, hlam::Vec2 speed);
  void Update(float dt);
  void Draw();
};
