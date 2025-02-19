#include "game_scene.h"

#include <HLAM/collisions2D.h>

#include <algorithm>

#include "assets.h"
#include "const.h"
#include "entities/wolf.h"

using namespace hlam;

GameScene::GameScene(SceneManager* sm, GameState* gameState)
    : sm(sm),
      gameState(gameState),
      players(),
      truck(Vec2{kTruckPosX, kTruckPosY}, Truck::kTruck2Size, {kTruckSpeedX, kTruckSpeedY}) {
  auto grassImg = LoadImage("assets/grass-0001.png");
  ImageFormat(&grassImg, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
  ImageColorInvert(&grassImg);
  grass = LoadTextureFromImage(grassImg);
  UnloadImage(grassImg);

  // players.push_back(std::make_unique<Player>(0, Vec2{kPlayerSpawnPosX, kPlayerSpawnPosY}, kPlayerSpeed));
  // players.push_back(std::make_unique<Player>(1, Vec2{kPlayerSpawnPosX + 64, kPlayerSpawnPosY}, kPlayerSpeed));

  // DEBUG wolfs
  // wolfs.push_back(std::make_unique<Wolf>(Vec2{360, 200}, Vec2{16, 16}, kWolfSpeed, kWolfRunSpeed));
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
  truck = {Vec2{kTruckPosX, kTruckPosY}, Truck::kTruck2Size, {kTruckSpeedX, kTruckSpeedY}};
  gameState->stats = {};
  camera.offset = {kCanvasWidth / 2, kCanvasHeight / 2};
  camera.target = {0, 0};
  camera.rotation = 0;
  camera.zoom = 1;
  pigs = spawnPigs();
  wolfs.clear();
  players.clear();
}
void GameScene::ConnectPlayer() {
  if (players.size() == 2) {
    // TODO: right now only for 2 players
    return;
  }
  hlam::Vec2 spawnPos{kPlayerSpawnPosX, kPlayerSpawnPosY};
  if (players.size() == 1) {
    spawnPos = players[0]->pos;
  }

  auto gamepad0 =
      IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP) ||
      IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) ||
      IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_UP) ||
      IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);

  auto gamepad1 =
      IsGamepadButtonDown(1, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || IsGamepadButtonDown(1, GAMEPAD_BUTTON_LEFT_FACE_UP) ||
      IsGamepadButtonDown(1, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || IsGamepadButtonDown(1, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) ||
      IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_UP) ||
      IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) || IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);

  auto keyboard0 = IsKeyPressed(KEY_W) || IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) ||
                   IsKeyPressed(KEY_SPACE);
  auto keyboard1 = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_RIGHT) ||
                   IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_RIGHT_SHIFT);

  if (keyboard0 || gamepad0) {
    for (auto& player : players) {
      // TODO: it must be hashmap but it requires other code to be rewritten...
      if (player->index == 0) {
        return;
      }
    }
    players.push_back(std::make_unique<Player>(0, spawnPos, kPlayerSpeed));
  }
  if (keyboard1 || gamepad1) {
    for (auto& player : players) {
      // TODO: it must be hashmap but it requires other code to be rewritten...
      if (player->index == 1) {
        return;
      }
    }
    players.push_back(std::make_unique<Player>(1, spawnPos, kPlayerSpeed));
  }
}

void GameScene::Update(float dt) {
  if (players.size() > 0) {
    gameState->stats.time += dt;
    wolfSpawnCooldown.Update(dt);
    truck.Update(dt);
  }
  ConnectPlayer();
  TrySpawnWolf();
  hlam::Vec2 avgPos{0, 0};
  for (auto& player : players) {
    player->Update(dt);
    avgPos += player->pos / players.size();
  }

  if (players.size() == 1) {
    // for 1 user smooth camera
    float minSpeed = 128;
    float minEffectLength = 128;
    float fractionSpeed = 0.95f;

    auto diff = hlam::Vec2{
        players[0]->pos.x - camera.target.x,
        players[0]->pos.y - camera.target.y,
    };
    auto len = hlam::vec_length(diff);
    if (len > minEffectLength) {
      float speed = fmaxf(fractionSpeed * len, minSpeed);
      camera.target.x += diff.x * speed * dt / len;
      camera.target.y += diff.y * speed * dt / len;
    }
  }

  if (players.size() > 1) {
    // simple center of the screen if two users
    camera.target.x = avgPos.x;
    camera.target.y = avgPos.y;
  }

  if (players.size() == 2) {
    auto diff = players[0]->pos - players[1]->pos;
    auto scaleX = std::clamp(kCanvasWidth / std::abs(diff.x), 0.5f, 1.0f);
    auto scaleY = std::clamp(kCanvasHeight / std::abs(diff.y), 0.5f, 1.0f);
    camera.zoom = std::min(scaleX, scaleY);
  }

  camera.target.x = std::clamp(camera.target.x, kWorldPosLeft + camera.offset.x, kWorldPosRight - camera.offset.x);
  camera.target.y = std::clamp(camera.target.y, kWorldPosUp + camera.offset.y, kWorldPosDown - camera.offset.y);

  if (truck.pos.x >= kWorldPosRight) {
    sm->Change("results");
  }

  std::sort(players.begin(), players.end(),
            [](const auto& p1, const auto& p2) { return p1->IsDashing() && !p2->IsDashing(); });

  auto truckRect = Rect{truck.pos.x, truck.pos.y, truck.size.x, truck.size.y};

  for (auto& player : players) {
    // player - truck collisions
    // TODO: make player rectangle here?
    if (CheckCollisionCircleRec(player->pos, player->physSize.x / 2, truckRect)) {
      auto nearestPoint = collision_point_circle_rectangle({player->pos, player->physSize.x / 2}, truckRect);
      auto diff = player->pos - nearestPoint;
      player->pos = nearestPoint + hlam::vec_norm(diff) * player->physSize.x / 2;
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

    // player - wolf collision
    for (auto& wolf : wolfs) {
      if (CheckCollisionCircles(wolf->pos, wolf->size.x / 2, player->pos, Player::physSize.x / 2)) {
        auto diff = wolf->pos - player->pos;
        if (player->IsDashing()) {
          constexpr auto kickSpreadAngle = PI / 2;
          auto kickDir = atan2(diff.y, diff.x);
          auto wolfKickDir = kickDir;
          bool kickPig = false;
          if (wolf->GetState() == KIDNAPPING && wolf->closestPig != nullptr) {
            kickPig = true;
            wolfKickDir += kickSpreadAngle / 2;
          }

          if (kickPig) {
            auto pigKickDir = kickDir - kickSpreadAngle / 2;
            auto pigKickVec = hlam::Vec2{cosf(pigKickDir), sinf(pigKickDir)};
            wolf->closestPig->DoKick({pigKickVec, balance::kickPower});
          }

          auto wolfKickVec = hlam::Vec2{cosf(wolfKickDir), sinf(wolfKickDir)};
          wolf->DoKick({wolfKickVec, balance::kickPower});
          player->KickedPig();
        } else {
          wolf->pos = player->pos + hlam::vec_norm(diff) * (wolf->size.x / 2 + Player::physSize.x / 2);
        }
      }
    }

    // player - pig collisions
    for (auto& pig : pigs) {
      // player collides with kidnapped pigs
      if (pig->GetState() == Pig::State::KICKED || pig->isDead) {
        continue;
      }

      if (CheckCollisionCircles(pig->pos, pig->size.x / 2, player->pos, Player::physSize.x / 2)) {
        auto diff = pig->pos - player->pos;
        if (player->IsDashing()) {
          pig->DoKick({hlam::vec_norm(diff), balance::kickPower});
          player->KickedPig();
        } else {
          pig->pos = player->pos + hlam::vec_norm(diff) * (pig->size.x / 2 + Player::physSize.x / 2);
        }
      }
    }
  }

  for (auto& pig : pigs) {
    if (pig->GetState() != Pig::State::IDLE || pig->isDead) {
      continue;
    }

    pig->pos = hlam::fit_in_bounds(pig->pos, pig->size, {kWorldPosLeft, kWorldPosUp, kWorldPosRight, kWorldPosDown});

    // pig - truck collisions
    if (CheckCollisionCircleRec(pig->pos, pig->size.x / 2, truckRect)) {
      auto nearestPoint = collision_point_circle_rectangle({pig->pos, pig->size.x / 2}, truckRect);
      auto diff = pig->pos - nearestPoint;
      pig->pos = nearestPoint + hlam::vec_norm(diff) * pig->size.x / 2;
    }

    // pig - hopper collisions
    auto hopper = truck.GetHopperRect();
    if (CheckCollisionCircleRec(pig->pos, pig->size.x / 2, hopper)) {
      pig->isDead = true;
      gameState->stats.pigs_gathered++;
    }

    // pig - pig collisions
    for (auto& pig1 : pigs) {
      if (pig == pig1) {
        continue;
      }

      if (pig1->GetState() != Pig::State::IDLE || pig1->isDead) {
        continue;
      }

      if (CheckCollisionCircles(pig->pos, pig->size.x / 2, pig1->pos, pig1->size.x / 2)) {
        auto diff = pig1->pos - pig->pos;
        pig1->pos = pig->pos + hlam::vec_norm(diff) * (pig->size.x);
      }
    }
  }

  for (const auto& pig : pigs) {
    pig->Update(dt);
  }

  for (auto& wolf : wolfs) {
    if (wolf->closestPig == nullptr || wolf->closestPig->GetState() == Pig::State::KICKED || wolf->closestPig->isDead) {
      // TODO: or pig is dead/gathered...
      Pig* closestPig = nullptr;
      float closestPigDist = 100000000;
      for (auto& pig : pigs) {
        if (pig->GetState() != Pig::State::IDLE || pig->isDead) {
          continue;
        }

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

  auto wolves_kicked = 0;
  auto pigs_stolen = 0;
  wolfs.erase(std::remove_if(wolfs.begin(), wolfs.end(),
                             [&pigs_stolen, &wolves_kicked](const auto& wolf) {
                               if (wolf->IsDead()) {
                                 if (wolf->GetState() == KIDNAPPING) {
                                   if (wolf->closestPig != nullptr) {
                                     pigs_stolen++;
                                   }
                                 } else {
                                   wolves_kicked++;
                                 }
                               }
                               return wolf->IsDead();
                             }),
              wolfs.end());
  gameState->stats.pigs_stolen += pigs_stolen;
  gameState->stats.wolves_kicked += wolves_kicked;

  pigs.erase(std::remove_if(pigs.begin(), pigs.end(), [](const auto& pig) { return pig->isDead; }), pigs.end());
  if (pigs.empty()) {
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
  for (auto& player : players) {
    player->Draw();
  }

  std::sort(pigs.begin(), pigs.end(), [](auto& p1, auto& p2) { return p1->elevation < p2->elevation; });
  for (auto& pig : pigs) {
    pig->Draw();
  }

  for (auto& wolf : wolfs) {
    wolf->Draw();
  }

  EndMode2D();
}
void GameScene::Exit() {}

void GameScene::TrySpawnWolf() {
  if (wolfs.size() >= kMaxWolfCount) {
    return;
  }
  if (wolfSpawnCooldown.Invoke()) {
    float posX = GetRandomValue(kWorldPosLeft + 128, kWorldPosRight - 128);
    float posY = kWorldPosUp + 2;
    if (GetRandomValue(0, 1) == 1) {
      posY = kWorldPosDown - 2;
    }

    TraceLog(LOG_INFO, "SPAWN %f %f", posX, posY);
    wolfs.push_back(std::make_unique<Wolf>(Vec2{posX, posY}, Vec2{16, 16}, kWolfSpeed, kWolfRunSpeed));
    PlaySound(wolfAwooFX);
  }
}
