#include "test_kick_scene.h"

#include <raylib.h>

#include <algorithm>

#include "entities/entity.h"
#include "entities/pig.h"

void TestKickScene::Activate() {
  auto pig = Pig{{10, 20}};
  pigs = {std::make_shared<Pig>(hlam::Vec2{50, 50}), std::make_shared<Pig>(hlam::Vec2{100, 50}),
          std::make_shared<Pig>(hlam::Vec2{150, 50}), std::make_shared<Pig>(hlam::Vec2{50, 100})};
}

void TestKickScene::Exit() {}

void drawCircleShadow(hlam::Vec2 pos, float radius) {
  auto img = GenImageChecked(radius, radius, radius, radius, BLACK, WHITE);
  auto mask = GenImageColor(radius, radius, {0, 0, 0, 0});
  ImageDrawCircle(&mask, radius / 2, radius / 2, radius, WHITE);
  ImageAlphaMask(&img, mask);
  Texture shadowTex = LoadTextureFromImage(img);
  UnloadImage(img);
  UnloadImage(mask);

  DrawTextureV(shadowTex, pos, WHITE);
}

void TestKickScene::Update(float dt) {
  if (IsKeyDown(KEY_A)) {
    kickDir.x -= dt;
  } else if (IsKeyDown(KEY_D)) {
    kickDir.x += dt;
  } else if (IsKeyDown(KEY_W)) {
    kickDir.y -= dt;
  } else if (IsKeyDown(KEY_S)) {
    kickDir.y += dt;
  }

  kickPower = 2.5f;

  auto index = -1;
  auto key = GetKeyPressed();
  // dirty
  if (KEY_ONE <= key && key <= KEY_FOUR) {
    index = key - KEY_ONE;
  }

  if (index != -1) {
    auto pig = pigs[index];
    pig->DoKick({hlam::vec_norm(kickDir), kickPower});
  }

  for (const auto& pig : pigs) {
    pig->Update(dt);
  }
}

void TestKickScene::Draw() {
  DrawText(TextFormat("%0.2f %0.2f %0.2f", kickDir.x, kickDir.y, kickPower), 10, 20, 20, WHITE);
  std::sort(pigs.begin(), pigs.end(), [](auto p1, auto p2) {
    auto el1 = p1->elevation;
    auto el2 = p2->elevation;

    if (el1 < 0) {
      el1 = 0.0f;
    }

    if (el2 < 0) {
      el2 = 0.0f;
    }

    return el1 < el2;
  });

  for (const auto& pig : pigs) {
    pig->Draw();
  }
}
