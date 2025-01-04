#include "controls_scene.h"

#include <algorithm>

#include "const.h"

using namespace hlam;

ControlsScene::ControlsScene() : player(0, Vec2{32, 32}, kPlayerSpeed) {}
void ControlsScene::Activate() {
  camera.offset = {kWindowHeight / 2, kWindowHeight / 2};
  camera.target = {0, 0};
  camera.rotation = 0;
  camera.zoom = 1;
}
void ControlsScene::Update(float dt) {
  player.Update(dt);

  camera.target.x = std::clamp(player.pos.x, kWorldPosLeft + camera.offset.x, kWorldPosRight - camera.offset.x);
  camera.target.y = std::clamp(player.pos.y, kWorldPosUp + camera.offset.y, kWorldPosDown - camera.offset.y);
}
void ControlsScene::Draw() {
  BeginMode2D(camera);
  DrawRectangleLines(kPlayerPosLeft, kPlayerPosUp, kPlayerPosRight, kPlayerPosDown, RED);
  DrawRectangle(64, 64, 32, 32, RED);
  player.Draw();
  EndMode2D();
}
void ControlsScene::Exit() {}
