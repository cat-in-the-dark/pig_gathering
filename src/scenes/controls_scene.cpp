#include "controls_scene.h"

#include "const.h"

using namespace hlam;

ControlsScene::ControlsScene() : player(0, Vec2{32, 32}, kPlayerSpeed) {}
void ControlsScene::Activate() {
  camera.offset = {kWindowWidth / 2, kWindowHeight / 2};
  camera.target = {0, 0};
  camera.rotation = 0;
  camera.zoom = 1;
}
void ControlsScene::Update(float dt) {
  player.Update(dt);
  camera.target = player.pos;
}
void ControlsScene::Draw() {
  BeginMode2D(camera);
  DrawRectangle(64, 64, 32, 32, RED);
  player.Draw();
  EndMode2D();
}
void ControlsScene::Exit() {}
