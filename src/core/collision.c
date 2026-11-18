#include "core/collision.h"

#include "core/config.h"

bool collision_aabb_overlap(Aabb a, Aabb b)
{
    if (a.w <= 0.0f || a.h <= 0.0f || b.w <= 0.0f || b.h <= 0.0f) {
        return false;
    }
    return (a.x < b.x + b.w) && (a.x + a.w > b.x) &&
           (a.y < b.y + b.h) && (a.y + a.h > b.y);
}

Aabb collision_player_box(const Player *player)
{
    Aabb box = {0.0f, 0.0f, 0.0f, 0.0f};

    if (player == 0) {
        return box;
    }

    /* 留少量宽容，避免视觉上擦边却判死。 */
    box.x = player->x + 5.0f;
    box.y = player->y + 6.0f;
    box.w = SP_PLAYER_W - 10.0f;
    box.h = SP_PLAYER_H - 7.0f;
    return box;
}

Aabb collision_obstacle_box(const Obstacle *obstacle)
{
    Aabb box = {0.0f, 0.0f, 0.0f, 0.0f};

    if (obstacle == 0) {
        return box;
    }

    box.x = obstacle->x;
    box.y = obstacle->y;
    box.w = obstacle->w;
    box.h = obstacle->h;

    if (obstacle->type == OBSTACLE_SPIKE) {
        box.x += 5.0f;
        box.y += 7.0f;
        box.w -= 10.0f;
        box.h -= 7.0f;
    } else if (obstacle->type == OBSTACLE_WIDE) {
        box.x += 3.0f;
        box.w -= 6.0f;
    }

    return box;
}

bool collision_check_player(const Player *player, const Terrain *terrain, CollisionResult *result)
{
    Aabb player_box;
    int i;
    int j;

    if (result != 0) {
        result->hit = false;
        result->segment_index = -1;
        result->obstacle_index = -1;
    }
    if (player == 0 || terrain == 0) {
        return false;
    }

    player_box = collision_player_box(player);
    for (i = 0; i < terrain->segment_count; ++i) {
        const TerrainSegment *segment = &terrain->segments[i];

        /* 已经完全离开屏幕左侧的段不参与判定。 */
        if (segment->x + segment->width < -20.0f) {
            continue;
        }
        for (j = 0; j < segment->obstacle_count; ++j) {
            Aabb obstacle_box = collision_obstacle_box(&segment->obstacles[j]);
            if (collision_aabb_overlap(player_box, obstacle_box)) {
                if (result != 0) {
                    result->hit = true;
                    result->segment_index = i;
                    result->obstacle_index = j;
                }
                return true;
            }
        }
    }
    return false;
}
