#include "results_scene.h"

#include <raylib.h>

ResultsScene::ResultsScene(GameState* state) : state(state) {}
ResultsScene::~ResultsScene() = default;

void ResultsScene::Activate() {}
void ResultsScene::Update(float dt) {}
void ResultsScene::Draw() {
  DrawText(TextFormat("time: %0.2 secf\npigs: %d", state->stats.time, state->stats.pigs_gathered), 32, 32, 32, WHITE);
}
void ResultsScene::Exit() {
  state->stats = Stats{};
}
