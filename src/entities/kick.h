#pragma once

#include <HLAM/hlam_math.h>

struct Kick {
  // normalized direction vector
  hlam::Vec2 dir;

  float impulse;
};
