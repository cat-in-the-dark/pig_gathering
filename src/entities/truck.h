#pragma once

#include <HLAM/hlam_math.h>

class Truck {
 public:
  static constexpr hlam::Vec2 kTruck2Size = {64, 32};
  static constexpr hlam::Vec2 kTruckInfluence = kTruck2Size * 1.3f;

  hlam::Vec2 pos;
  hlam::Vec2 size;
  hlam::Vec2 speed;

  Truck(hlam::Vec2 pos, hlam::Vec2 size, hlam::Vec2 speed);
  void Update(float dt);
  void Draw();
};
