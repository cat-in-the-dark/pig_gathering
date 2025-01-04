#include <raylib.h>
#include <rlgl.h>
#include <stdlib.h>

#include "const.h"
#include "scenes/controls_scene.h"
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
  }
};

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
  EndTextureMode();

  // actual render in screen
  BeginDrawing();
  ClearBackground(BLACK);
  viewport.draw(canvas.texture);
  DrawFPS(5, 5);
  EndDrawing();
}

int main() {
  InitWindow(kWindowWidth, kWindowHeight, "Game");
  canvas = LoadRenderTexture(viewport.canvas_width, viewport.canvas_height);
  SetTextureFilter(canvas.texture, TEXTURE_FILTER_POINT);

  SceneManager sm;
  GameState gameState;

  sm.Register<ComboScene>("title")
      ->With<TextureScene>(LoadTexture("assets/logo.png"), 400, 240)
      ->With<KeyAwaitScene>(&sm, KEY_SPACE, "game");

  sm.Register<ComboScene>("game")->With<GameScene>()->With<KeyAwaitScene>(&sm, KEY_SPACE, "test_collisions");

  sm.Register<TestKickScene>("test_kick");

  sm.Register<ControlsScene>("controls_scene", &sm);

  sm.Register<ComboScene>("results")->With<ResultsScene>(&gameState)->With<KeyAwaitScene>(&sm, KEY_SPACE, "game");

  sm.Change("controls_scene");

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
