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
  pigKickFXs.push_back(LoadSound("assets/fx_pig_kick_1.wav"));
  pigKickFXs.push_back(LoadSound("assets/fx_pig_kick_2.wav"));
  wolfKickFX = LoadSound("assets/fx_uwu.wav");
  wolfIdleFrames.push_back(LoadTexture("assets/SWolchara1.png"));
  wolfIdleFrames.push_back(LoadTexture("assets/SWolchara2.png"));
  wolfWalkFrames.push_back(LoadTexture("assets/Wolchara1.png"));
  wolfWalkFrames.push_back(LoadTexture("assets/Wolchara2.png"));
}

std::vector<Texture2D> truckTex;
std::vector<Texture2D> playerIdleFrames;
std::vector<Texture2D> playerWalkFrames;
std::vector<Texture2D> pigIdleFrames;
Texture2D pigDamagedFrame;
std::vector<Sound> pigKickFXs;
Sound wolfKickFX;
std::vector<Texture2D> wolfIdleFrames;
std::vector<Texture2D> wolfWalkFrames;
