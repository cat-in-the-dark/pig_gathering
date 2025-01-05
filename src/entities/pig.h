#ifndef PIG_H
#define PIG_H

#include <HLAM/animations.h>

#include "entity.h"
#include "kick.h"

class Pig : public Entity {
 public:
  enum class State { IDLE, KICKED, KIDNAPPED };

  static constexpr auto rotationSpeed = 2 * PI / 3.0f;
  static constexpr auto pigWitdh = 24.0f;
  static constexpr auto pigHeight = 24.0f;
  static constexpr hlam::Vec2 pigSize = {pigWitdh, pigHeight};
  static constexpr hlam::Vec2 drawDelta = pigSize / 2.0f;

  hlam::Vec2 speed;
  float elevation;
  bool isDead;
  hlam::Animation anim;
  float rotation = 0.0f;

  explicit Pig(hlam::Vec2 pos);

  virtual ~Pig();

  void DoKick(Kick kick);

  virtual void Update(float dt) override;

  virtual void Draw() override;

  void Release() {
    state_ = State::IDLE;
  }

  State GetState() const {
    return state_;
  }

  void Kidnapped();

 private:
  float elevationSpeed_;
  State state_;
  Texture shadow_;
  int pigSoundVariant = 0;
};

#endif  // PIG_H
