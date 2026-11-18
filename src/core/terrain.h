/*
 * 程序化地形：模板池 + 显式随机种子。
 * 地形段只保存障碍模板，地面本身是连续的；段与段之间保留最小反应空隙。
 */
#ifndef SP_TERRAIN_H
#define SP_TERRAIN_H

#include <stdint.h>

#include "core/config.h"
#include "core/difficulty.h"
#include "core/rng.h"

typedef enum ObstacleType {
    OBSTACLE_SPIKE = 0,
    OBSTACLE_CRATE = 1,
    OBSTACLE_TALL = 2,
    OBSTACLE_WIDE = 3
} ObstacleType;

typedef struct Obstacle {
    float x;
    float y;
    float w;
    float h;
    uint8_t type;
} Obstacle;

typedef struct TerrainSegment {
    float x;
    float width;
    int pattern;
    int obstacle_count;
    Obstacle obstacles[SP_TERRAIN_MAX_OBSTACLES];
} TerrainSegment;

typedef struct Terrain {
    TerrainSegment segments[SP_TERRAIN_MAX_SEGMENTS];
    int segment_count;
    float next_x;
    float ground_y;
    Rng rng;
    uint32_t generated;
} Terrain;

void terrain_init(Terrain *terrain, uint32_t seed, float ground_y, const DifficultyParams *difficulty);
void terrain_scroll(Terrain *terrain, float dx, const DifficultyParams *difficulty);
int terrain_obstacle_count(const Terrain *terrain);

#endif /* SP_TERRAIN_H */
