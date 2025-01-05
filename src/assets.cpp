#include "assets.h"

void LoadAssets() {
  truckTex.push_back(LoadTexture("assets/truck1.png"));
  truckTex.push_back(LoadTexture("assets/truck2.png"));

  playerIdleFrames.push_back(LoadTexture("assets/player_st1.png"));
  playerIdleFrames.push_back(LoadTexture("assets/player_st2.png"));

  playerWalkFrames.push_back(LoadTexture("assets/player_go1.png"));
  playerWalkFrames.push_back(LoadTexture("assets/player_go2.png"));
  playerWalkFrames.push_back(LoadTexture("assets/player_go3.png"));
  playerWalkFrames.push_back(LoadTexture("assets/player_go4.png"));

  pigIdleFrames.push_back(LoadTexture("assets/pig_idle1.png"));
  pigIdleFrames.push_back(LoadTexture("assets/pig_idle2.png"));

  pigDamagedFrame = LoadTexture("assets/pig_damaged.png");
}

std::vector<Texture2D> truckTex;
std::vector<Texture2D> playerIdleFrames;
std::vector<Texture2D> playerWalkFrames;
std::vector<Texture2D> pigIdleFrames;
Texture2D pigDamagedFrame;
