#include <string.h>

#include "core/collision.h"
#include "core/config.h"
#include "core/physics.h"
#include "core/terrain.h"

#include "test_framework.h"

void test_collision_suite(void)
{
    Aabb a;
    Aabb b;
    Player player;
    Terrain terrain;
    CollisionResult result;
    Obstacle *obstacle;

    a.x = 10.0f;
    a.y = 10.0f;
    a.w = 20.0f;
    a.h = 20.0f;
    b = a;
    TF_CHECK(collision_aabb_overlap(a, b));

    b.x = 30.0f;
    TF_CHECK(!collision_aabb_overlap(a, b));
    b.x = 29.9f;
    TF_CHECK(collision_aabb_overlap(a, b));

    b = a;
    b.y = 30.0f;
    TF_CHECK(!collision_aabb_overlap(a, b));

    b = a;
    b.w = 0.0f;
    TF_CHECK(!collision_aabb_overlap(a, b));

    physics_reset(&player, 100.0f, SP_GROUND_Y);
    memset(&terrain, 0, sizeof(terrain));
    terrain.ground_y = SP_GROUND_Y;
    terrain.segment_count = 1;
    terrain.segments[0].x = 0.0f;
    terrain.segments[0].width = 400.0f;
    terrain.segments[0].obstacle_count = 1;

    obstacle = &terrain.segments[0].obstacles[0];
    obstacle->x = 120.0f;
    obstacle->y = 550.0f;
    obstacle->w = 40.0f;
    obstacle->h = 62.0f;
    obstacle->type = OBSTACLE_CRATE;

    TF_CHECK(collision_check_player(&player, &terrain, &result));
    TF_CHECK(result.hit);
    TF_CHECK_INT(result.segment_index, 0);
    TF_CHECK_INT(result.obstacle_index, 0);

    /* 障碍左边缘等于玩家碰撞盒右边缘时不算碰撞。 */
    obstacle->x = collision_player_box(&player).x + collision_player_box(&player).w;
    TF_CHECK(!collision_check_player(&player, &terrain, &result));

    obstacle->x = collision_player_box(&player).x + collision_player_box(&player).w - 0.5f;
    TF_CHECK(collision_check_player(&player, &terrain, &result));

    /* 冰锥的判定框向内收缩。 */
    obstacle->x = 100.0f;
    obstacle->y = SP_GROUND_Y - 42.0f;
    obstacle->w = 34.0f;
    obstacle->h = 42.0f;
    obstacle->type = OBSTACLE_SPIKE;
    a = collision_obstacle_box(obstacle);
    TF_CHECK_NEAR(a.x, 105.0f, 0.01f);
    TF_CHECK_NEAR(a.w, 24.0f, 0.01f);

    terrain.segment_count = 0;
    TF_CHECK(!collision_check_player(&player, &terrain, &result));
    TF_CHECK(!result.hit);
    TF_CHECK_INT(result.segment_index, -1);

    TF_CHECK(!collision_check_player(0, &terrain, 0));
    TF_CHECK(!collision_check_player(&player, 0, 0));
}
