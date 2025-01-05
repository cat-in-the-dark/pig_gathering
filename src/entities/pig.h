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
  enum class State { IDLE, KICKED, KIDNAPPED };

  static constexpr auto pigWitdh = 24.0f;
  static constexpr auto pigHeight = 24.0f;
  static constexpr hlam::Vec2 pigSize = {pigWitdh, pigHeight};
  static constexpr hlam::Vec2 drawDelta = pigSize / 2.0f;

  hlam::Vec2 speed;
  float elevation;
  bool isDead;

  explicit Pig(hlam::Vec2 pos);

  virtual ~Pig();

  void DoKick(Kick kick);

  virtual void Update(float dt) override;

  virtual void Draw() override;

  State GetState() const {
    return state_;
  }

  void Kidnapped();

 private:
  float elevationSpeed_;
  State state_;
  Texture shadow_;
};

#endif  // PIG_H
