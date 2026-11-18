#include "core/score.h"

#include "core/config.h"

void score_reset(ScoreState *score)
{
    if (score == 0) {
        return;
    }
    score->current = 0;
    score->distance_px = 0.0f;
    score->distance_m = 0.0f;
    score->new_record = false;
    /* high_score / high_score_dirty 属于跨局数据，保留。 */
}

void score_set_high_score(ScoreState *score, int value)
{
    if (score == 0) {
        return;
    }
    if (value < 0) {
        value = 0;
    }
    if (value > SP_HIGH_SCORE_LIMIT) {
        value = SP_HIGH_SCORE_LIMIT;
    }
    score->high_score = value;
}

void score_update(ScoreState *score, float distance_px, float elapsed_seconds)
{
    int distance_score;
    int time_score;

    if (score == 0) {
        return;
    }
    if (distance_px < 0.0f) {
        distance_px = 0.0f;
    }
    if (elapsed_seconds < 0.0f) {
        elapsed_seconds = 0.0f;
    }

    score->distance_px = distance_px;
    score->distance_m = distance_px / SP_SCORE_PIXELS_PER_METER;

    distance_score = (int)score->distance_m;
    time_score = (int)(elapsed_seconds * SP_SCORE_TIME_BONUS);
    score->current = distance_score + time_score;
    if (score->current > SP_HIGH_SCORE_LIMIT) {
        score->current = SP_HIGH_SCORE_LIMIT;
    }
}

void score_finish(ScoreState *score)
{
    if (score == 0) {
        return;
    }

    score->new_record = false;
    if (score->current > score->high_score) {
        score->high_score = score->current;
        score->new_record = true;
        score->high_score_dirty = true;
    }
}
