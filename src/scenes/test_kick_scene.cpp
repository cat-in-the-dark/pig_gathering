#include "test_kick_scene.h"

#include <raylib.h>

#include "entities/entity.h"
#include "entities/pig.h"

void TestKickScene::Activate() {
  auto pig = Pig{{10, 20}};
  entities = {std::make_shared<Pig>(hlam::Vec2{50, 50}), std::make_shared<Pig>(hlam::Vec2{100, 50}),
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
  } else if (IsKeyDown(KEY_Q)) {
    kickPower -= 2 * dt;
  } else if (IsKeyDown(KEY_E)) {
    kickPower += 2 * dt;
  }

  auto index = -1;
  // dirty
  if (IsKeyDown(KEY_ONE)) {
    index = 0;
  } else if (IsKeyDown(KEY_TWO)) {
    index = 1;
  } else if (IsKeyDown(KEY_THREE)) {
    index = 2;
  } else if (IsKeyDown(KEY_FOUR)) {
    index = 3;
  }

  for (const auto &entity : entities) {
    entity->Update(dt);
  }

  if (index != -1) {
    auto pig = reinterpret_cast<Pig *>(entities[index].get());
    pig->DoKick({kickDir, kickPower});
  }
}

void TestKickScene::Draw() {
  DrawText(TextFormat("%0.2f %0.2f %0.2f", kickDir.x, kickDir.y, kickPower), 10, 20, 20, WHITE);
  for (const auto &entity : entities) {
    if (entity->elevation > 0.0f) {
      drawCircleShadow(entity->pos + hlam::Vec2{10, 10}, entity->width);
    }
    entity->Draw();
  }
}
