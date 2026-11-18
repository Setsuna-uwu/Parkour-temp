#include <string.h>

#include "core/config.h"
#include "core/difficulty.h"
#include "core/terrain.h"

#include "test_framework.h"

static int terrain_matches(const Terrain *a, const Terrain *b)
{
    int i;
    int j;

    if (a->segment_count != b->segment_count || a->generated != b->generated ||
        a->rng.state != b->rng.state) {
        return 0;
    }
    for (i = 0; i < a->segment_count; ++i) {
        const TerrainSegment *sa = &a->segments[i];
        const TerrainSegment *sb = &b->segments[i];
        if (sa->pattern != sb->pattern || sa->obstacle_count != sb->obstacle_count ||
            fabsf(sa->x - sb->x) > 0.001f || fabsf(sa->width - sb->width) > 0.001f) {
            return 0;
        }
        for (j = 0; j < sa->obstacle_count; ++j) {
            const Obstacle *oa = &sa->obstacles[j];
            const Obstacle *ob = &sb->obstacles[j];
            if (oa->type != ob->type || fabsf(oa->x - ob->x) > 0.001f ||
                fabsf(oa->y - ob->y) > 0.001f || fabsf(oa->w - ob->w) > 0.001f ||
                fabsf(oa->h - ob->h) > 0.001f) {
                return 0;
            }
        }
    }
    return 1;
}

void test_terrain_suite(void)
{
    Terrain terrain;
    Terrain copy;
    DifficultyParams difficulty;
    int i;
    int j;

    difficulty_init(&difficulty);
    terrain_init(&terrain, 12345u, SP_GROUND_Y, &difficulty);

    TF_CHECK(terrain.segment_count > 0);
    TF_CHECK(terrain.segment_count <= SP_TERRAIN_MAX_SEGMENTS);
    TF_CHECK(terrain.generated > 0u);
    TF_CHECK(terrain.next_x > (float)SP_SCREEN_WIDTH);
    TF_CHECK(terrain.segments[0].obstacles[0].x > SP_PLAYER_START_X + 600.0f);
    TF_CHECK(terrain_obstacle_count(&terrain) > 0);

    for (i = 0; i < terrain.segment_count; ++i) {
        const TerrainSegment *segment = &terrain.segments[i];
        TF_CHECK(segment->obstacle_count > 0);
        TF_CHECK(segment->obstacle_count <= SP_TERRAIN_MAX_OBSTACLES);
        for (j = 0; j < segment->obstacle_count; ++j) {
            const Obstacle *obstacle = &segment->obstacles[j];
            TF_CHECK(obstacle->x >= segment->x - 0.01f);
            TF_CHECK(obstacle->x + obstacle->w <= segment->x + segment->width + 0.01f);
            TF_CHECK_NEAR(obstacle->y + obstacle->h, SP_GROUND_Y, 0.01f);
        }
    }

    for (i = 1; i < terrain.segment_count; ++i) {
        const TerrainSegment *previous = &terrain.segments[i - 1];
        const TerrainSegment *current = &terrain.segments[i];
        const Obstacle *last = &previous->obstacles[previous->obstacle_count - 1];
        const Obstacle *first = &current->obstacles[0];
        TF_CHECK(first->x - (last->x + last->w) >= 250.0f);
    }

    /* 同种子 + 同样步数，得到完全一致的地形。 */
    terrain_init(&copy, 12345u, SP_GROUND_Y, &difficulty);
    TF_CHECK(terrain_matches(&terrain, &copy));

    /* 移动 5000px，确认回收后数量受控且仍然一致。 */
    copy = terrain;
    for (i = 0; i < 300; ++i) {
        float t = (float)i * SP_FIXED_DT;
        DifficultyParams current = difficulty_at(t);
        terrain_scroll(&terrain, current.speed * SP_FIXED_DT, &current);
        terrain_scroll(&copy, current.speed * SP_FIXED_DT, &current);
        TF_CHECK(terrain.segment_count > 0);
        TF_CHECK(terrain.segment_count <= SP_TERRAIN_MAX_SEGMENTS);
    }
    TF_CHECK(terrain_matches(&terrain, &copy));

    for (i = 0; i < terrain.segment_count; ++i) {
        const TerrainSegment *segment = &terrain.segments[i];
        TF_CHECK(segment->x + segment->width >= SP_TERRAIN_RECYCLE_X - 1.0f);
        for (j = 0; j < segment->obstacle_count; ++j) {
            const Obstacle *obstacle = &segment->obstacles[j];
            TF_CHECK(obstacle->x >= segment->x - 0.01f);
            TF_CHECK(obstacle->x + obstacle->w <= segment->x + segment->width + 0.01f);
        }
    }

    /* 障碍物必须与地形段同步滚动，而不是留在原地。 */
    {
        float before_segment;
        float before_obstacle;
        terrain_init(&terrain, 5150u, SP_GROUND_Y, &difficulty);
        before_segment = terrain.segments[0].x;
        before_obstacle = terrain.segments[0].obstacles[0].x;
        terrain_scroll(&terrain, 100.0f, &difficulty);
        TF_CHECK_NEAR(terrain.segments[0].x, before_segment - 100.0f, 0.01f);
        TF_CHECK_NEAR(terrain.segments[0].obstacles[0].x, before_obstacle - 100.0f, 0.01f);
    }

    /* 不同种子应产生不同模板序列。 */
    {
        Terrain other;
        terrain_init(&other, 987654u, SP_GROUND_Y, &difficulty);
        TF_CHECK(!terrain_matches(&terrain, &other));
    }

    /* 长局滚动，覆盖生成/回收的稳定性。 */
    difficulty_init(&difficulty);
    terrain_init(&terrain, 777u, SP_GROUND_Y, &difficulty);
    for (i = 0; i < 7200; ++i) {
        DifficultyParams current = difficulty_at((float)i * SP_FIXED_DT);
        terrain_scroll(&terrain, current.speed * SP_FIXED_DT, &current);
    }
    TF_CHECK(terrain.segment_count > 0);
    TF_CHECK(terrain.segment_count < SP_TERRAIN_MAX_SEGMENTS);
    TF_CHECK(terrain.generated > 100u);
    TF_CHECK(terrain.next_x > (float)SP_SCREEN_WIDTH);
    TF_CHECK(terrain_obstacle_count(&terrain) > 0);
}
