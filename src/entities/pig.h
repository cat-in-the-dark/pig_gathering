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

  static constexpr hlam::Vec2 drawDelta = {pigWitdh / 2, pigHeight / 2};

  hlam::Vec2 speed;
  float elevation;

  explicit Pig(hlam::Vec2 pos);

  virtual ~Pig();

  void DoKick(Kick kick);

  virtual void Update(float dt) override;

  virtual void Draw() override;

  bool isKicked() const {
    return isKicked_;
  }

  void Kidnapped();

 private:
  float elevationSpeed_;
  bool isKicked_;
  Texture shadow_;
};

#endif  // PIG_H
