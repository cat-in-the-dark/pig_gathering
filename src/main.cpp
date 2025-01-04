#include <raylib.h>
#include <rlgl.h>
#include <stdlib.h>

#include "const.h"
#include "scenes/game_scene.h"
#include "scenes/results_scene.h"

#define HLAM_SCENE_IMPLEMENTATION
#include <HLAM/scene.h>

#define HLAM_COROUTINES_IMPLEMENTATION
#include <HLAM/coroutines.h>

#define HLAM_VIEWPORT_IMPLEMENTATION
#include <HLAM/viewport.h>

#define HLAM_MATH_IMPLEMENTATION
#include <HLAM/hlam_math.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "scenes/test_kick_scene.h"

using namespace hlam;

static RenderTexture2D canvas;
static Viewport viewport = {kWindowWidth, kWindowHeight, Vector2{0, 0}};

void update(void* arg) {
  float dt = GetFrameTime();
  auto sm = reinterpret_cast<SceneManager*>(arg);
  // TODO: use pattern https://gameprogrammingpatterns.com/game-loop.html
  sm->Update(dt);

  viewport.update();
  BeginTextureMode(canvas);
  ClearBackground(BLACK);
  if (!sm->Draw()) {
    ClearBackground(BLACK);
    DrawText(TextFormat("SCENE '%s' NOT FOUND", sm->Current().c_str()), 0, 0, 32, RED);
  }
  for (int gamepad = 0; gamepad < 4; gamepad++) {
    if (IsGamepadAvailable(gamepad)) {
      DrawText(TextFormat("GP%d: %s", gamepad, GetGamepadName(gamepad)), 8, 190 + 10 * (4 - gamepad), 10, WHITE);
    }
  }
  EndTextureMode();

  // actual render in screen
  BeginDrawing();
  ClearBackground(BLACK);
  viewport.draw(canvas.texture);
  DrawFPS(5, 5);
  EndDrawing();
}

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(kWindowWidth, kWindowHeight, "Game");
  canvas = LoadRenderTexture(viewport.canvas_width, viewport.canvas_height);
  SetTextureFilter(canvas.texture, TEXTURE_FILTER_POINT);

  SceneManager sm;
  GameState gameState;

  sm.Register<ComboScene>("title")
      ->With<TextureScene>(LoadTexture("assets/logo.png"), 400, 240)
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "game");

  sm.Register<TestKickScene>("test_kick");

  sm.Register<GameScene>("game", &sm);

  sm.Register<ComboScene>("results")->With<ResultsScene>(&gameState)->With<KeyAwaitScene>(&sm, KEY_SPACE, "game");

  sm.Change("game");

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
