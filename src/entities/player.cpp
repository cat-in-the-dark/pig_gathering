#include "player.h"

#include <raylib.h>
// raylib must be before
#include <HLAM/hlam_math.h>

using namespace hlam;

Player::Player(int index, hlam::Vec2 pos, float playerSpeed) : index(index), pos(pos), playerSpeed(playerSpeed) {}
void Player::Draw() {
  DrawRectangleV(pos - physSize / 2, physSize, WHITE);
}
void Player::Update(float dt) {
  UpdateControls(dt);

  pos += playerSpeedVec;
}

void Player::UpdateControls(float dt) {
  Vec2 speed{0, 0};
  if (index == 0) {
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
      speed.y = 1;
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
      speed.y = -1;
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
      speed.x = 1;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
      speed.x = -1;
    }
  }

  playerSpeedVec = vec_norm(speed) * playerSpeed * dt;
}
