#include "core/rng.h"

void rng_seed(Rng *rng, uint32_t seed)
{
    if (rng == 0) {
        return;
    }
    /* 0 是 xorshift 的固定点，替换为非零常量。 */
    rng->state = (seed != 0u) ? seed : 0x9E3779B9u;
}

uint32_t rng_next_u32(Rng *rng)
{
    uint32_t x;

    if (rng == 0) {
        return 0u;
    }
    x = rng->state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    if (x == 0u) {
        x = 0x6D2B79F5u;
    }
    rng->state = x;
    return x;
}

int rng_range(Rng *rng, int min_inclusive, int max_exclusive)
{
    uint32_t span;
    uint32_t value;

    if (max_exclusive <= min_inclusive) {
        return min_inclusive;
    }
    span = (uint32_t)(max_exclusive - min_inclusive);
    value = rng_next_u32(rng) % span;
    return min_inclusive + (int)value;
}

float rng_float01(Rng *rng)
{
    /* 使用高 24 位，结果严格位于 [0, 1)。 */
    return (float)(rng_next_u32(rng) >> 8) * (1.0f / 16777216.0f);
}
