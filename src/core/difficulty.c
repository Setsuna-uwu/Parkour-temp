#include "core/difficulty.h"

#include <math.h>

static float clamp_float(float value, float min_value, float max_value)
{
    if (value < min_value) {
        return min_value;
    }
    if (value > max_value) {
        return max_value;
    }
    return value;
}

void difficulty_init(DifficultyParams *params)
{
    if (params == 0) {
        return;
    }
    params->speed = 330.0f;
    params->gap = 470.0f;
    params->hard_weight = 0.0f;
    params->level = 1;
}

DifficultyParams difficulty_at(float survival_seconds)
{
    DifficultyParams params;
    float t;

    t = clamp_float(survival_seconds, 0.0f, 180.0f);

    /* 330 -> 约 520 px/s；前 60 秒变化最明显，之后缓慢逼近上限。 */
    params.speed = 330.0f + 190.0f * (1.0f - expf(-t / 70.0f));

    /* 像素间距逐步收紧；模板内部还有自身宽度，难度还来自类型权重。 */
    params.gap = 470.0f - 110.0f * (t / 180.0f);
    params.hard_weight = clamp_float(t / 150.0f, 0.0f, 1.0f);

    params.level = 1 + (int)(t / 20.0f);
    if (params.level > 10) {
        params.level = 10;
    }
    if (params.level < 1) {
        params.level = 1;
    }

    return params;
}

const wchar_t *difficulty_level_name(int level)
{
    switch (level) {
    case 1:
        return L"轻松";
    case 2:
        return L"热身";
    case 3:
        return L"渐快";
    case 4:
        return L"加速";
    case 5:
        return L"紧张";
    case 6:
        return L"疾驰";
    case 7:
        return L"严酷";
    case 8:
        return L"极限";
    case 9:
        return L"风暴";
    default:
        return L"雪崩";
    }
}
