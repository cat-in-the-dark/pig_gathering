#include "game_scene.h"

#include <HLAM/collisions2D.h>

#include <algorithm>

#include "const.h"

using namespace hlam;

GameScene::GameScene(SceneManager* sm)
    : sm(sm), players(), truck(Vec2{kTruckPosX, kTruckPosY}, {64, 32}, {kTruckSpeedX, kTruckSpeedY}) {
  auto grassImg = LoadImage("assets/grass-0001.png");
  ImageFormat(&grassImg, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
  ImageColorInvert(&grassImg);
  grass = LoadTextureFromImage(grassImg);
  UnloadImage(grassImg);

  players.push_back(std::make_unique<Player>(0, Vec2{kPlayerSpawnPosX, kPlayerSpawnPosY}, kPlayerSpeed));
  players.push_back(std::make_unique<Player>(1, Vec2{kPlayerSpawnPosX + 64, kPlayerSpawnPosY}, kPlayerSpeed));
}
GameScene::~GameScene() {
  UnloadTexture(grass);
}

auto spawnPigs() {
  std::vector<std::unique_ptr<Pig>> result{};
  // simple uniform distribution
  for (auto i = 0; i < balance::kPigsCount; i++) {
    result.push_back(std::make_unique<Pig>(hlam::Vec2{
        static_cast<float>(GetRandomValue(kWorldPosLeft + Pig::pigWitdh / 2, kWorldPosRight - Pig::pigWitdh / 2)),
        static_cast<float>(GetRandomValue(kWorldPosUp + Pig::pigHeight / 2, kWorldPosDown - Pig::pigHeight / 2))}));
  }

  return result;
}

void GameScene::Activate() {
  camera.offset = {kWindowHeight / 2, kWindowHeight / 2};
  camera.target = {0, 0};
  camera.rotation = 0;
  camera.zoom = 1;
  pigs = spawnPigs();
}
void GameScene::Update(float dt) {
  truck.Update(dt);
  for (auto& player : players) {
    player->Update(dt);
  }

  if (players.size() > 0) {
    // TODO: get average point between all players
    camera.target.x = std::clamp(players[0]->pos.x, kWorldPosLeft + camera.offset.x, kWorldPosRight - camera.offset.x);
    camera.target.y = std::clamp(players[0]->pos.y, kWorldPosUp + camera.offset.y, kWorldPosDown - camera.offset.y);
  }

  if (truck.pos.x >= kWindowWidth) {
    sm->Change("results");
  }

  std::sort(pigs.begin(), pigs.end(), [](auto& p1, auto& p2) { return p1->elevation < p2->elevation; });
  std::sort(players.begin(), players.end(),
            [](const auto& p1, const auto& p2) { return p1->IsDashing() && !p2->IsDashing(); });

  for (auto& player : players) {
    for (auto& player1 : players) {
      if (player1 == player) {
        continue;
      }

      if (CheckCollisionCircles(player1->pos, Player::physSize.x / 2, player->pos, Player::physSize.x / 2)) {
        auto diff = player1->pos - player->pos;
        player1->pos = player->pos + hlam::vec_norm(diff) * Player::physSize.x;
      }
    }

    for (auto& pig : pigs) {
      pig->Update(dt);
      if (pig->elevation > 0) {
        continue;
      }

      if (CheckCollisionCircles(pig->pos, pig->width / 2, player->pos, Player::physSize.x / 2)) {
        auto diff = pig->pos - player->pos;
        if (player->IsDashing()) {
          pig->DoKick({hlam::vec_norm(diff), balance::kickPower});
          player->dashAnim.Finish();
        } else {
          pig->pos = player->pos + hlam::vec_norm(diff) * (pig->width / 2 + Player::physSize.x / 2);
        }
      }
    }
  }

  for (auto& pig : pigs) {
    if (pig->elevation > 0) {
      continue;
    }

    for (auto& pig1 : pigs) {
      if (pig == pig1) {
        continue;
      }

      if (pig1->elevation > 0) {
        continue;
      }

      if (CheckCollisionCircles(pig->pos, pig->width / 2, pig1->pos, pig1->width / 2)) {
        auto diff = pig1->pos - pig->pos;
        pig1->pos = pig->pos + hlam::vec_norm(diff) * (pig->width);
      }
    }
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
  for (auto& player : players) {
    player->Draw();
  }

  for (auto& pig : pigs) {
    pig->Draw();
  }

  EndMode2D();
}
void GameScene::Exit() {}
