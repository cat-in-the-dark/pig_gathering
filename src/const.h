#pragma once

constexpr auto kWindowWidth = 400;
constexpr auto kWindowHeight = 240;
constexpr auto kPlayerSpeed = 128.0f;
constexpr auto kDashTime = 0.2f;
constexpr auto kDashCooldown = kDashTime * 2;
constexpr auto kDashPower = kPlayerSpeed * 4;

constexpr float kWorldPosLeft = 0;
constexpr float kWorldPosRight = 800;
constexpr float kWorldPosUp = 0;
constexpr float kWorldPosDown = kWindowHeight;

constexpr float kPlayerPosLeft = kWorldPosLeft;
constexpr float kPlayerPosRight = kWorldPosRight;
constexpr float kPlayerPosUp = kWorldPosUp;
constexpr float kPlayerPosDown = kWorldPosDown;

constexpr float kPlayerSpawnPosX = 32;
constexpr float kPlayerSpawnPosY = kWindowHeight - 48;

constexpr float kTruckPosX = 32;
constexpr float kTruckPosY = 32;
constexpr float kTruckSpeedX = 8;
constexpr float kTruckSpeedY = 0;

namespace balance {
constexpr auto kPigsCount = 10;
constexpr auto kickPower = 2.5f;
constexpr auto kTruckInfluenceRadius = 50.0f;
constexpr auto kPigEscaneVelocity = 5.0f;
}  // namespace balance
