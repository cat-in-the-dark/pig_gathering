#include "entity.h"

Entity::Entity(hlam::Vec2 pos, int width, int height)
    : pos(pos), width(width), height(height), speed{0, 0}, elevation(0) {}

Entity::~Entity() = default;
