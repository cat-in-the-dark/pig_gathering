#include "pig.h"

#include <HLAM/textures.h>
#include <raylib.h>

#include <algorithm>

#include "assets.h"
#include "const.h"
#include "helpers/texture_helper.h"

Pig::Pig(hlam::Vec2 pos)
    : Entity(pos, pigSize),
      speed{0, 0},
      elevation(0),
      isDead(false),
      anim(pigIdleFrames, 1.0f, true),
      elevationSpeed_(0.0f),
      state_(State::IDLE),
      shadow_(generateShadow(Pig::pigWitdh / 2.0f)),
      pigSoundVariant(GetRandomValue(0, pigKickFXs.size() - 1)) {
  // do desync pigs
  anim.time = GetRandomValue(0, 60) / 60.0f;
}

Pig::~Pig() {
  UnloadTexture(shadow_);
}

void Pig::DoKick(Kick kick) {
  elevationSpeed_ = kick.impulse * sinf(balance::kKickAngle);
  auto horizontalSpeed = kick.impulse * cosf(balance::kKickAngle);

  speed = kick.dir * horizontalSpeed;
  state_ = State::KICKED;

  PlaySound(pigKickFXs[pigSoundVariant]);
}

void Pig::Update(float dt) {
  if (isDead) {
    return;
  }

  anim.Update(dt);

  dt *= 3;
  if (state_ == State::KICKED) {
    rotation += rotationSpeed * dt;
    if (elevation >= 0.0f) {
      elevation += elevationSpeed_ * dt;
      elevationSpeed_ -= physics::gravityAcceleration * dt;
    }

    if (elevation < 0.0f) {
      state_ = State::IDLE;
      rotation = 0.0f;
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

  auto elevationOffset = hlam::Vec2{0.0f, elevation * 50};
  auto drawPos = pos - elevationOffset;
  auto texture = anim.GetFrame();

  if (state_ == State::KICKED) {
    texture = pigDamagedFrame;
  }

  Rectangle source = {0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
  Rectangle dest = {drawPos.x, drawPos.y, static_cast<float>(texture.width), static_cast<float>(texture.height)};
  hlam::Vec2 origin = {texture.width / 2.0f, texture.height / 2.0f};

  DrawTexturePro(texture, source, dest, origin, rotation * 180 / PI, WHITE);
}

void Pig::Kidnapped() {
  state_ = State::KIDNAPPED;
}
