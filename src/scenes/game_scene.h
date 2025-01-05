#pragma once

#include <raylib.h>

// raylib must be before
#include <HLAM/coroutines.h>
#include <HLAM/hlam_math.h>
#include <HLAM/scene.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "entities/pig.h"
#include "entities/player.h"
#include "entities/truck.h"
#include "state.h"

class Wolf;  // forward

class GameScene : public hlam::Scene {
  hlam::SceneManager* sm;
  GameState* gameState;
  std::vector<std::unique_ptr<Player>> players;
  std::vector<std::unique_ptr<Pig>> pigs;
  std::vector<std::unique_ptr<Wolf>> wolfs;
  Truck truck;
  Camera2D camera;
  hlam::Cooldown wolfSpawnCooldown{kWolfSpawnCooldownTime};

  Texture2D grass;
  void ConnectPlayer();
  void TrySpawnWolf();

 public:
  GameScene(hlam::SceneManager* sm, GameState* gameState);
  void Activate() override;
  void Update(float dt) override;
  void Draw() override;
  void Exit() override;
  ~GameScene();
};
