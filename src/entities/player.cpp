#include "player.h"

#include <raylib.h>

#include <algorithm>
// raylib must be before
#include <HLAM/hlam_math.h>

#include "assets.h"

using namespace hlam;

Player::Player(int index, hlam::Vec2 pos, float playerSpeed)
    : idleAnim(playerIdleFrames, 0.5, true),
      walkAnim(playerWalkFrames, 0.2, true),
      currentAnim(&idleAnim),
      index(index),
      pos(pos),
      playerSpeed(playerSpeed) {
  if (index == 0) {
    key_up = KEY_W;
    key_down = KEY_S;
    key_left = KEY_A;
    key_right = KEY_D;
    key_dash = KEY_SPACE;
  }
  if (index == 1) {
    key_up = KEY_UP;
    key_down = KEY_DOWN;
    key_left = KEY_LEFT;
    key_right = KEY_RIGHT;
    key_dash = KEY_RIGHT_SHIFT;
  }
}
void Player::Draw() {
  // DrawRectangleV(pos - physSize / 2, physSize, WHITE);
  auto frame = currentAnim->GetFrame();
  Rect src = {0, 0, static_cast<float>(frame.width) * dirX, static_cast<float>(frame.height)};
  Rect dst = {pos.x - physSize.x / 2, pos.y - physSize.y / 2, static_cast<float>(frame.width),
              static_cast<float>(frame.height)};

  DrawTexturePro(frame, src, dst, Vector2{0, 0}, 0, WHITE);
}
void Player::Update(float dt) {
  currentAnim->Update(dt);
  dashAnim.Update(dt);
  dashCooldown.Update(dt);
  UpdateControls(dt);

  if (hlam::vec_length(playerSpeedVec) < 0.1) {
    currentAnim = &idleAnim;
  } else {
    currentAnim = &walkAnim;
    dirX = playerSpeedVec.x < 0 ? -1 : 1;
  }
  pos += playerSpeedVec;
  pos.x = std::clamp(pos.x, kPlayerPosLeft, kPlayerPosRight);
  pos.y = std::clamp(pos.y, kPlayerPosUp, kPlayerPosDown);
}

bool Player::IsDashing() const {
  return !dashAnim.IsPassed();
}

static bool IsGamepadActionPressed(int gamepad) {
  return IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) ||
         IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_UP) ||
         IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) ||
         IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
}

void Player::UpdateControls(float dt) {
  if (dashAnim.IsPassed()) {  // forbid moving in dash
    lastControlsDir.x = 0;
    lastControlsDir.y = 0;

    if (IsKeyDown(key_down) || IsGamepadButtonDown(index, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
      lastControlsDir.y = 1;
    }
    if (IsKeyDown(key_up) || IsGamepadButtonDown(index, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
      lastControlsDir.y = -1;
    }
    if (IsKeyDown(key_right) || IsGamepadButtonDown(index, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
      lastControlsDir.x = 1;
    }
    if (IsKeyDown(key_left) || IsGamepadButtonDown(index, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
      lastControlsDir.x = -1;
    }
  }

  if ((IsKeyDown(key_dash) || IsGamepadActionPressed(index)) && dashCooldown.Invoke()) {
    dashAnim.Reset();
  }

  float speed = playerSpeed + dashPower * (1 - dashAnim.Percentage());
  playerSpeedVec = vec_norm(lastControlsDir) * speed * dt;
}

void Player::KickedPig() {
  dashAnim.Finish();
  if (IsGamepadAvailable(index)) {
    SetGamepadVibration(index, 0.5f, 0.5f, 0.2f);
  }
}
