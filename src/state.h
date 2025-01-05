#pragma once

struct Stats {
  float time = 0;
  int pigs_gathered = 0;
  int wolves_kicked = 0;
  int pigs_stolen = 0;
};

struct GameState {
  Stats stats;
};
