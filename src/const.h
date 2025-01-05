#pragma once

constexpr auto kCanvasWidth = 400;
constexpr auto kCanvasHeight = 240;
constexpr auto kWindowWidth = kCanvasWidth * 2;
constexpr auto kWindowHeight = kCanvasHeight * 2;
constexpr auto kPlayerSpeed = 128.0f;
constexpr auto kDashTime = 0.2f;
constexpr auto kDashCooldown = kDashTime * 2;
constexpr auto kDashPower = kPlayerSpeed * 4;

constexpr float kWorldPosLeft = 0;
constexpr float kWorldPosRight = 800;
constexpr float kWorldPosUp = 0;
constexpr float kWorldPosDown = kCanvasHeight + 32;

constexpr float kPlayerPosLeft = kWorldPosLeft;
constexpr float kPlayerPosRight = kWorldPosRight;
constexpr float kPlayerPosUp = kWorldPosUp;
constexpr float kPlayerPosDown = kWorldPosDown;

constexpr float kPlayerSpawnPosX = 32;
constexpr float kPlayerSpawnPosY = kCanvasHeight - 48;

constexpr float kTruckPosX = 32;
constexpr float kTruckPosY = 32;
constexpr float kTruckSpeedX = 8;
constexpr float kTruckSpeedY = 0;

constexpr float kWolfSpeed = 32;
constexpr float kWolfRunSpeed = kWolfSpeed * 2;
constexpr int kMinWolfWanderDist = 32;
constexpr int kMaxWolfWanderDist = 32 * 3;
constexpr float kWolfChaseTriggerDist = 96;

constexpr auto kMaxWolfCount = 3;
constexpr float kWolfSpawnCooldownTime = 5;

namespace balance {
constexpr auto kPigsCount = 10;
constexpr auto kickPower = 2.5f;
constexpr auto kPigEscaneVelocity = 5.0f;
constexpr auto kPlayerEscaneVelocity = 10.0f;
}  // namespace balance
