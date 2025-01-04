#include "player.h"

#include <raylib.h>

#include <algorithm>
// raylib must be before
#include <HLAM/hlam_math.h>

using namespace hlam;

Player::Player(int index, hlam::Vec2 pos, float playerSpeed) : index(index), pos(pos), playerSpeed(playerSpeed) {}
void Player::Draw() {
  DrawRectangleV(pos - physSize / 2, physSize, WHITE);
}
void Player::Update(float dt) {
  dashAnim.Update(dt);
  dashCooldown.Update(dt);
  UpdateControls(dt);

  pos += playerSpeedVec;
  pos.x = std::clamp(pos.x, kPlayerPosLeft, kPlayerPosRight);
  pos.y = std::clamp(pos.y, kPlayerPosUp, kPlayerPosDown);
}

void Player::UpdateControls(float dt) {
  if (dashAnim.IsPassed()) {  // forbid moving in dash
    lastControlsDir.x = 0;
    lastControlsDir.y = 0;

    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S) || IsGamepadButtonDown(index, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
      lastControlsDir.y = 1;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsGamepadButtonDown(index, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
      lastControlsDir.y = -1;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsGamepadButtonDown(index, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
      lastControlsDir.x = 1;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || IsGamepadButtonDown(index, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
      lastControlsDir.x = -1;
    }
  }

  if (IsKeyDown(KEY_SPACE) && dashCooldown.Invoke()) {
    dashAnim.Reset();
  }

  float speed = playerSpeed + dashPower * (1 - dashAnim.Percentage());
  playerSpeedVec = vec_norm(lastControlsDir) * speed * dt;
}
