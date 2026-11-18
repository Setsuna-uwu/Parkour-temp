/*
 * AABB 碰撞。core 不负责绘制，也不负责把失败写回 GameState。
 */
#ifndef SP_COLLISION_H
#define SP_COLLISION_H

#include <stdbool.h>

#include "core/physics.h"
#include "core/terrain.h"

typedef struct Aabb {
    float x;
    float y;
    float w;
    float h;
} Aabb;

typedef struct CollisionResult {
    bool hit;
    int segment_index;
    int obstacle_index;
} CollisionResult;

bool collision_aabb_overlap(Aabb a, Aabb b);
Aabb collision_player_box(const Player *player);
Aabb collision_obstacle_box(const Obstacle *obstacle);
bool collision_check_player(const Player *player, const Terrain *terrain, CollisionResult *result);

#endif /* SP_COLLISION_H */
