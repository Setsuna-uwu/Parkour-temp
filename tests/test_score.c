#include "core/config.h"
#include "core/score.h"

#include "test_framework.h"

void test_score_suite(void)
{
    ScoreState score;

    score.current = 0;
    score.high_score = 0;
    score.distance_px = 0.0f;
    score.distance_m = 0.0f;
    score.new_record = false;
    score.high_score_dirty = false;

    score_update(&score, 0.0f, 0.0f);
    TF_CHECK_INT(score.current, 0);

    /* 4500px = 100m，10 秒 = 60 分。 */
    score_update(&score, 4500.0f, 10.0f);
    TF_CHECK_NEAR(score.distance_m, 100.0f, 0.01f);
    TF_CHECK_INT(score.current, 160);

    score.high_score = 500;
    score_update(&score, 4500.0f, 10.0f);
    score_finish(&score);
    TF_CHECK_INT(score.high_score, 500);
    TF_CHECK(!score.new_record);
    TF_CHECK(!score.high_score_dirty);

    score.high_score = 100;
    score_update(&score, 9000.0f, 20.0f);
    score_finish(&score);
    TF_CHECK(score.new_record);
    TF_CHECK(score.high_score_dirty);
    TF_CHECK_INT(score.high_score, score.current);
    TF_CHECK(score.current > 100);

    {
        int preserved_high_score = score.high_score;
        score_reset(&score);
        TF_CHECK_INT(score.current, 0);
        TF_CHECK_INT(score.high_score, preserved_high_score);
        TF_CHECK(score.high_score_dirty);
    }

    score_finish(&score);
    TF_CHECK(!score.new_record);

    score_set_high_score(&score, -5);
    TF_CHECK_INT(score.high_score, 0);
    score_set_high_score(&score, SP_HIGH_SCORE_LIMIT + 10);
    TF_CHECK_INT(score.high_score, SP_HIGH_SCORE_LIMIT);
}
