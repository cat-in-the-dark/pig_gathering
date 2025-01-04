#include <raylib.h>
#include <rlgl.h>
#include <stdlib.h>

#include "const.h"

#define HLAM_SCENE_IMPLEMENTATION
#include <HLAM/scene.h>

#define HLAM_COROUTINES_IMPLEMENTATION
#include <HLAM/coroutines.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

using namespace hlam;

void DrawLineWired(std::vector<Vector2>& model, Color color) {
  rlBegin(RL_LINES);
  rlColor4ub(color.r, color.g, color.b, color.a);
  for (size_t i = 0; i < model.size() - 1; i++) {
    rlVertex2f(model[i].x, model[i].y);
    rlVertex2f(model[i + 1].x, model[i + 1].y);
  }
  auto& last = model[model.size() - 1];
  rlVertex2f(last.x, last.y);
  rlVertex2f(model[0].x, model[0].y);
  rlEnd();
}

class GameScene : public Scene {
  Timer timer = {2};
  std::vector<Vector2> points = {{5, 5}, {5, 64}, {32, 90}, {64, 64}, {64, 5}};

 public:
  void Activate() override {}
  void Exit() override {}
  void Update(float dt) override {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      auto pos = GetMousePosition();
      TraceLog(LOG_INFO, "BUM %f, %f", pos.x, pos.y);
    }

    timer.Update(GetFrameTime());
  }
  void Draw() override {
    ClearBackground(BLACK);
    if (timer.IsPassed()) {
      DrawRectangle(40, 64, 100, 64, BLUE);
    } else {
      DrawRectangle(40, 64, 100, 64, GREEN);
    }

    DrawLineWired(points, BLUE);
  }
};

void update(void* arg) {
  float dt = GetFrameTime();
  auto sm = reinterpret_cast<SceneManager*>(arg);
  // TODO: use pattern https://gameprogrammingpatterns.com/game-loop.html
  sm->Update(dt);
  BeginDrawing();
  if (!sm->Draw()) {
    ClearBackground(BLACK);
    DrawText(TextFormat("SCENE '%s' NOT FOUND", sm->Current().c_str()), 0, 0, 32, RED);
  }
  EndDrawing();
}

int main() {
  InitWindow(kWindowWidth, kWindowHeight, "Game");

  SceneManager sm;

  sm.Register<ComboScene>("title")
      ->With<TextureScene>(LoadTexture("assets/logo.png"), 400, 240)
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "game");

  sm.Register<ComboScene>("game")->With<GameScene>()->With<KeyAwaitScene>(&sm, KEY_SPACE, "test_collisions");

  sm.Change("title");

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(update, &sm, 0, 1);
#else
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    update(&sm);
  }
#endif

  CloseWindow();

  return 0;
}
