#include <math.h>

#include "core/difficulty.h"

#include "test_framework.h"

void test_difficulty_suite(void)
{
    DifficultyParams start;
    DifficultyParams middle;
    DifficultyParams end;
    DifficultyParams clamped_low;
    DifficultyParams clamped_high;

    start = difficulty_at(0.0f);
    middle = difficulty_at(60.0f);
    end = difficulty_at(180.0f);
    clamped_low = difficulty_at(-25.0f);
    clamped_high = difficulty_at(9999.0f);

    TF_CHECK(start.speed > 0.0f);
    TF_CHECK(middle.speed > start.speed);
    TF_CHECK(end.speed > middle.speed);
    TF_CHECK(end.speed < 550.0f);

    TF_CHECK(middle.gap < start.gap);
    TF_CHECK(end.gap < middle.gap);
    TF_CHECK(end.gap >= 350.0f);

    TF_CHECK(middle.hard_weight > start.hard_weight);
    TF_CHECK(end.hard_weight > middle.hard_weight);
    TF_CHECK_NEAR(end.hard_weight, 1.0f, 0.001f);

    TF_CHECK_INT(start.level, 1);
    TF_CHECK(middle.level > start.level);
    TF_CHECK(end.level <= 10);
    TF_CHECK(end.level > middle.level);

    TF_CHECK_NEAR(clamped_low.speed, start.speed, 0.001f);
    TF_CHECK_NEAR(clamped_low.gap, start.gap, 0.001f);
    TF_CHECK_NEAR(clamped_high.speed, end.speed, 0.001f);
    TF_CHECK_NEAR(clamped_high.gap, end.gap, 0.001f);

    TF_CHECK(isfinite(start.speed));
    TF_CHECK(isfinite(end.speed));
    TF_CHECK(difficulty_level_name(1) != 0);
    TF_CHECK(difficulty_level_name(10) != 0);
}
