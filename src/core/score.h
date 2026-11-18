/*
 * 计分与最高分逻辑。core 不进行文件 IO。
 */
#ifndef SP_SCORE_H
#define SP_SCORE_H

#include <stdbool.h>

typedef struct ScoreState {
    int current;
    int high_score;
    float distance_px;
    float distance_m;
    bool new_record;
    bool high_score_dirty;
} ScoreState;

void score_reset(ScoreState *score);
void score_set_high_score(ScoreState *score, int value);
void score_update(ScoreState *score, float distance_px, float elapsed_seconds);
void score_finish(ScoreState *score);

#endif /* SP_SCORE_H */
