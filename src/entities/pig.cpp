#include "pig.h"

#include <HLAM/textures.h>
#include <raylib.h>

#include <algorithm>

#include "const.h"
#include "helpers/texture_helper.h"

Pig::Pig(hlam::Vec2 pos)
    : Entity(pos, pigSize),
      speed{0, 0},
      elevation(0),
      isDead(false),
      elevationSpeed_(0.0f),
      state_(State::IDLE),
      shadow_(generateShadow(Pig::pigWitdh / 2.0f)) {}

Pig::~Pig() {
  UnloadTexture(shadow_);
}

void Pig::DoKick(Kick kick) {
  elevationSpeed_ = kick.impulse * sinf(balance::kKickAngle);
  auto horizontalSpeed = kick.impulse * cosf(balance::kKickAngle);

  speed = kick.dir * horizontalSpeed;
  state_ = State::KICKED;
}

void Pig::Update(float dt) {
  if (isDead) {
    return;
  }

  dt *= 3;
  if (state_ == State::KICKED) {
    if (elevation >= 0.0f) {
      elevation += elevationSpeed_ * dt;
      elevationSpeed_ -= physics::gravityAcceleration * dt;
    }

    if (elevation < 0.0f) {
      state_ = State::IDLE;
      elevationSpeed_ = 0.0f;
      elevation = 0.0f;
    }
  }

  // horizontal update
  if (hlam::vec_length_sqr(speed) > 0.0f) {
    auto friction = physics::groundFriction * dt;
    if (elevation > 0.0f) {
      friction = physics::airFriction * dt;
    }

    auto dir = atan2f(speed.y, speed.x);
    speed -= hlam::vec_norm(speed) * friction;

    // check if friction overshoot
    auto newDir = atan2f(speed.y, speed.x);
    if (fabsf(dir - newDir) > physics::epsilon) {
      speed = {0, 0};
    } else {
      // Check world boundaries
      auto newPos = pos + speed;
      if (newPos.x + pigWitdh > kWorldPosRight || newPos.x - pigWitdh < kWorldPosLeft) {
        speed.x *= -1;
      }
      if (newPos.y + pigHeight > kWorldPosDown || newPos.y - pigHeight < kWorldPosUp) {
        speed.y *= -1;
      }
    }
  }

  // FIXME: this was added to balance out previous bug. Please fix it along with dt * 3
  pos += speed;
}

void Pig::Draw() {
  if (isDead) {
    return;
  }

  auto shadowYOffset = hlam::Vec2{0.0f, -size.x / 2};

  if (elevation > 0) {
    DrawTextureV(shadow_, pos - drawDelta + shadowYOffset, WHITE);
  }

  DrawEllipse(pos.x, pos.y - elevation * 50, size.x / 2, size.y / 2, BLACK);
  DrawEllipseLines(pos.x, pos.y - elevation * 50, size.y / 2, size.x / 2, WHITE);
}

void Pig::Kidnapped() {
  state_ = State::KIDNAPPED;
}
