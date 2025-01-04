#pragma once

constexpr auto kWindowWidth = 400;
constexpr auto kWindowHeight = 240;
constexpr auto kPlayerSpeed = 128;
constexpr auto kDashTime = 0.2;
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
