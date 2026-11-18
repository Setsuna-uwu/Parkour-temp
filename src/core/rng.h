/*
 * 显式种子的 xorshift32 随机数发生器。
 * 不使用 time()/rand()，保证同一种子产生同一序列。
 */
#ifndef SP_RNG_H
#define SP_RNG_H

#include <stdint.h>

typedef struct Rng {
    uint32_t state;
} Rng;

void rng_seed(Rng *rng, uint32_t seed);
uint32_t rng_next_u32(Rng *rng);
int rng_range(Rng *rng, int min_inclusive, int max_exclusive);
float rng_float01(Rng *rng);

#endif /* SP_RNG_H */
