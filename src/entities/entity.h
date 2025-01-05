#ifndef ENTITY_H
#define ENTITY_H

#include <HLAM/hlam_math.h>

class Entity {
 public:
  hlam::Vec2 pos;
  hlam::Vec2 size;

  Entity(hlam::Vec2 pos, hlam::Vec2 size);
  virtual void Update(float dt) = 0;
  virtual void Draw() = 0;
  virtual ~Entity();
};

#endif  // ENTITY_H
