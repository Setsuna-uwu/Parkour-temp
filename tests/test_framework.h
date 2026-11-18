#ifndef SP_TEST_FRAMEWORK_H
#define SP_TEST_FRAMEWORK_H

#include <math.h>
#include <stdio.h>

extern int tf_checks;
extern int tf_failures;

#define TF_CHECK(condition)                                                          \
    do {                                                                             \
        tf_checks += 1;                                                              \
        if (!(condition)) {                                                          \
            tf_failures += 1;                                                        \
            printf("  FAIL  %s:%d  %s\n", __FILE__, __LINE__, #condition);           \
        }                                                                            \
    } while (0)

#define TF_CHECK_NEAR(actual, expected, tolerance)                                   \
    do {                                                                             \
        float tf_a_ = (float)(actual);                                               \
        float tf_e_ = (float)(expected);                                             \
        float tf_t_ = (float)(tolerance);                                            \
        tf_checks += 1;                                                              \
        if (fabsf(tf_a_ - tf_e_) > tf_t_) {                                          \
            tf_failures += 1;                                                        \
            printf("  FAIL  %s:%d  %s == %.6f, expected %.6f (+/- %.6f)\n",          \
                   __FILE__, __LINE__, #actual, (double)tf_a_, (double)tf_e_,         \
                   (double)tf_t_);                                                   \
        }                                                                            \
    } while (0)

#define TF_CHECK_INT(actual, expected)                                               \
    do {                                                                             \
        long long tf_a_ = (long long)(actual);                                       \
        long long tf_e_ = (long long)(expected);                                     \
        tf_checks += 1;                                                              \
        if (tf_a_ != tf_e_) {                                                        \
            tf_failures += 1;                                                        \
            printf("  FAIL  %s:%d  %s == %lld, expected %lld\n",                     \
                   __FILE__, __LINE__, #actual, tf_a_, tf_e_);                       \
        }                                                                            \
    } while (0)

void test_physics_suite(void);
void test_collision_suite(void);
void test_terrain_suite(void);
void test_difficulty_suite(void);
void test_score_suite(void);
void test_game_suite(void);

#endif /* SP_TEST_FRAMEWORK_H */
