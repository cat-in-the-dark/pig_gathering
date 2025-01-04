#include "game_scene.h"

#include <algorithm>

#include "const.h"

using namespace hlam;

GameScene::GameScene(SceneManager* sm)
    : sm(sm),
      player(0, Vec2{kPlayerSpawnPosX, kPlayerSpawnPosY}, kPlayerSpeed),
      truck(Vec2{kTruckPosX, kTruckPosY}, {64, 32}, {kTruckSpeedX, kTruckSpeedY}) {
  auto grassImg = LoadImage("assets/grass-0001.png");
  ImageFormat(&grassImg, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
  ImageColorInvert(&grassImg);
  grass = LoadTextureFromImage(grassImg);
  UnloadImage(grassImg);
}
GameScene::~GameScene() {
  UnloadTexture(grass);
}

void GameScene::Activate() {
  camera.offset = {kWindowHeight / 2, kWindowHeight / 2};
  camera.target = {0, 0};
  camera.rotation = 0;
  camera.zoom = 1;
}
void GameScene::Update(float dt) {
  truck.Update(dt);
  player.Update(dt);

  camera.target.x = std::clamp(player.pos.x, kWorldPosLeft + camera.offset.x, kWorldPosRight - camera.offset.x);
  camera.target.y = std::clamp(player.pos.y, kWorldPosUp + camera.offset.y, kWorldPosDown - camera.offset.y);

  if (truck.pos.x >= kWindowWidth) {
    sm->Change("results");
  }
}
void GameScene::Draw() {
  BeginMode2D(camera);
  DrawRectangleLines(kPlayerPosLeft + 2, kPlayerPosUp + 2, kPlayerPosRight - 4, kPlayerPosDown - 4, RED);

  for (int i = 0; i < kWorldPosRight / 16; i++) {
    for (int j = 0; j < kWorldPosDown / 16; j++) {
      if ((i + j) % 2 == 0) {
        DrawTexture(grass, i * 16, j * 16, WHITE);
      }
    }
  }

  truck.Draw();
  player.Draw();

  EndMode2D();
}
void GameScene::Exit() {}
