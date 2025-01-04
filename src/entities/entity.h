#ifndef ENTITY_H
#define ENTITY_H

#include <HLAM/hlam_math.h>

class Entity {
 public:
  Entity(hlam::Vec2 pos, int width, int height);

  hlam::Vec2 pos;
  hlam::Vec2 speed;
  float elevation;

  int width;
  int height;

  virtual void Update(float dt) = 0;
  virtual void Draw() = 0;
};

#endif  // ENTITY_H
