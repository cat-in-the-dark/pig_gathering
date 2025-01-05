#include "game_scene.h"

#include <HLAM/collisions2D.h>

#include <algorithm>

#include "const.h"
#include "entities/wolf.h"

using namespace hlam;

GameScene::GameScene(SceneManager* sm, GameState* gameState)
    : sm(sm),
      gameState(gameState),
      players(),
      truck(Vec2{kTruckPosX, kTruckPosY}, {64, 32}, {kTruckSpeedX, kTruckSpeedY}) {
  auto grassImg = LoadImage("assets/grass-0001.png");
  ImageFormat(&grassImg, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
  ImageColorInvert(&grassImg);
  grass = LoadTextureFromImage(grassImg);
  UnloadImage(grassImg);

  players.push_back(std::make_unique<Player>(0, Vec2{kPlayerSpawnPosX, kPlayerSpawnPosY}, kPlayerSpeed));
  players.push_back(std::make_unique<Player>(1, Vec2{kPlayerSpawnPosX + 64, kPlayerSpawnPosY}, kPlayerSpeed));

  // DEBUG wolfs
  wolfs.push_back(std::make_unique<Wolf>(Vec2{360, 200}, Vec2{16, 16}, kWolfSpeed, kWolfRunSpeed));
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
  truck = {Vec2{kTruckPosX, kTruckPosY}, {64, 32}, {kTruckSpeedX, kTruckSpeedY}};
  gameState->stats = {};
  camera.offset = {kWindowHeight / 2, kWindowHeight / 2};
  camera.target = {0, 0};
  camera.rotation = 0;
  camera.zoom = 1;
  pigs = spawnPigs();
}
void GameScene::Update(float dt) {
  truck.Update(dt);
  gameState->stats.time += dt;
  hlam::Vec2 avgPos{0, 0};
  for (auto& player : players) {
    player->Update(dt);
    avgPos += player->pos;
  }

  if (players.size() > 0) {
    camera.target.x =
        std::clamp(avgPos.x / players.size(), kWorldPosLeft + camera.offset.x, kWorldPosRight - camera.offset.x);
    camera.target.y =
        std::clamp(avgPos.y / players.size(), kWorldPosUp + camera.offset.y, kWorldPosDown - camera.offset.y);
  }

  if (truck.pos.x >= kWorldPosRight) {
    sm->Change("results");
  }

  std::sort(pigs.begin(), pigs.end(), [](auto& p1, auto& p2) { return p1->elevation < p2->elevation; });
  std::sort(players.begin(), players.end(),
            [](const auto& p1, const auto& p2) { return p1->IsDashing() && !p2->IsDashing(); });

  auto truckCenter = truck.pos + truck.size / 2;
  auto truckRect = Rect{truck.pos.x, truck.pos.y, truck.size.x, truck.size.y};

  for (auto& player : players) {
    // player - truck collisions
    if (CheckCollisionCircleRec(player->pos, player->physSize.x / 2, truckRect)) {
      auto nearestPoint = collision_point_circle_rectangle({player->pos, player->physSize.x / 2}, truckRect);
      auto diff = player->pos - nearestPoint;
      player->pos = nearestPoint + hlam::vec_norm(diff) * player->physSize / 2;
    }

    // player - player collisions
    for (auto& player1 : players) {
      if (player1 == player) {
        continue;
      }

      if (CheckCollisionCircles(player1->pos, Player::physSize.x / 2, player->pos, Player::physSize.x / 2)) {
        auto diff = player1->pos - player->pos;
        player1->pos = player->pos + hlam::vec_norm(diff) * Player::physSize.x;
      }
    }

    // player - pig collisions
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

  for (auto I = pigs.begin(); I != pigs.end();) {
    auto& pig = *I;
    if (pig->elevation > 0) {
      I++;
      continue;
    }

    // pig - truck collisions
    if (CheckCollisionCircleRec(pig->pos, pig->width / 2, {truck.pos.x, truck.pos.y, truck.size.x, truck.size.y})) {
      I = pigs.erase(I);
      gameState->stats.pigs_gathered++;
    } else {
      I++;
    }
  }

  // pig - pig collisions
  for (auto& pig : pigs) {
    if (pig->elevation > 0) {
      continue;
    }

    auto truck_center_pos = truck.pos + truck.size / 2;
    if (hlam::vec_dist_sqr(pig->pos, truck_center_pos) <
        balance::kTruckInfluenceRadius * balance::kTruckInfluenceRadius) {
      auto diff = pig->pos - truck_center_pos;
      pig->pos += hlam::vec_norm(diff) * balance::kTruckInfluenceRadius * dt;
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

  for (auto& wolf : wolfs) {
    if (wolf->closestPig == nullptr || wolf->closestPig->isKicked()) {
      // TODO: or pig is dead/gathered...
      Pig* closestPig = nullptr;
      float closestPigDist = 100000000;
      for (auto& pig : pigs) {
        float dist = hlam::vec_length(wolf->pos - pig->pos);
        if (dist < closestPigDist) {
          closestPigDist = dist;
          closestPig = pig.get();
        }
      }
      wolf->closestPig = closestPig;
    }

    wolf->Update(dt);
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

  for (auto& wolf : wolfs) {
    wolf->Draw();
  }

  EndMode2D();
}
void GameScene::Exit() {}
