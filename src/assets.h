#include <raylib.h>

#include <vector>

void LoadAssets();

extern std::vector<Texture2D> truckTex;
extern std::vector<Texture2D> playerIdleFrames;
extern std::vector<Texture2D> playerWalkFrames;
extern std::vector<Texture2D> pigIdleFrames;
extern Texture2D pigDamagedFrame;

extern std::vector<Sound> pigKickFXs;
extern Sound wolfKickFX;
extern Sound wolfAwooFX;
extern std::vector<Texture2D> wolfIdleFrames;
extern std::vector<Texture2D> wolfWalkFrames;
