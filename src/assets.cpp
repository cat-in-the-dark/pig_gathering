#include "assets.h"

void LoadAssets() {
  truckTex.push_back(LoadTexture("assets/truck1.png"));
  truckTex.push_back(LoadTexture("assets/truck2.png"));
}

std::vector<Texture2D> truckTex;
