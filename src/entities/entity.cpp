#include "entity.h"

Entity::Entity(hlam::Vec2 pos, int width, int height) : pos(pos), width(width), height(height) {}

Entity::~Entity() = default;
