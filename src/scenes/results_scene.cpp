#include "results_scene.h"

#include <raylib.h>

ResultsScene::ResultsScene(GameState* state) : state(state) {}
ResultsScene::~ResultsScene() = default;

void ResultsScene::Activate() {}
void ResultsScene::Update(float dt) {}
void ResultsScene::Draw() {
  const auto& stats = state->stats;
  DrawText(TextFormat("time: %0.2f sec\n"
                      "pigs collected: %d\n"
                      "pigs stolen: %d\n"
                      "wolves kicked: %d",
                      stats.time, stats.pigs_gathered, stats.pigs_stolen, stats.wolves_kicked),
           32, 32, 32, WHITE);
}
void ResultsScene::Exit() {
  state->stats = Stats{};
}
