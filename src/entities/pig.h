#ifndef PIG_H
#define PIG_H

#include "entity.h"

struct Kick {
  // normalized direction vector
  hlam::Vec2 dir;

  float impulse;
};

class Pig : public Entity {
 public:
  static constexpr auto pigWitdh = 24;
  static constexpr auto pigHeight = 24;

  explicit Pig(hlam::Vec2 pos);

  void DoKick(Kick kick);

  virtual void Update(float dt) override;

  virtual void Draw() override;

 private:
  float elevationSpeed_;
};

#endif  // PIG_H
