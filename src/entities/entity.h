#ifndef ENTITY_H
#define ENTITY_H

#include <HLAM/hlam_math.h>

class Entity {
 public:
  hlam::Vec2 pos;
  int width;
  int height;
  hlam::Vec2 speed;
  float elevation;

  Entity(hlam::Vec2 pos, int width, int height);
  virtual void Update(float dt) = 0;
  virtual void Draw() = 0;
  virtual ~Entity();
};

#endif  // ENTITY_H
