/*
 * 难度曲线：只依赖存活时间，输出速度 / 间距 / 高难模板权重。
 */
#ifndef SP_DIFFICULTY_H
#define SP_DIFFICULTY_H

#include <stddef.h>

typedef struct DifficultyParams {
    float speed;       /* 世界滚动速度 px/s */
    float gap;         /* 模板之间的基础空隙 px */
    float hard_weight; /* 0..1，越高越偏向复杂模板 */
    int level;         /* 1..10 */
} DifficultyParams;

void difficulty_init(DifficultyParams *params);
DifficultyParams difficulty_at(float survival_seconds);
const wchar_t *difficulty_level_name(int level);

#endif /* SP_DIFFICULTY_H */
