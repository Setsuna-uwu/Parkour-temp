#include <stdio.h>
#include <string.h>

#include "test_framework.h"

int tf_checks = 0;
int tf_failures = 0;

typedef struct TestSuite {
    const char *name;
    void (*function)(void);
} TestSuite;

int main(int argc, char **argv)
{
    static const TestSuite suites[] = {
        {"physics", test_physics_suite},
        {"collision", test_collision_suite},
        {"terrain", test_terrain_suite},
        {"difficulty", test_difficulty_suite},
        {"score", test_score_suite},
        {"game", test_game_suite}
    };
    const char *filter = (argc > 1) ? argv[1] : 0;
    int matched = 0;
    int i;
    int failures_before;
    int checks_before;

    printf("SnowParkour core tests | %s\n", filter ? filter : "all");
    printf("------------------------------------------------------------\n");

    for (i = 0; i < (int)(sizeof(suites) / sizeof(suites[0])); ++i) {
        if (filter != 0 && strstr(suites[i].name, filter) == 0) {
            continue;
        }
        matched += 1;
        checks_before = tf_checks;
        failures_before = tf_failures;
        printf("[ RUN  ] %s\n", suites[i].name);
        suites[i].function();
        printf("[ %s ] %s  (%d checks, %d failures)\n",
               tf_failures == failures_before ? " OK " : "FAIL",
               suites[i].name,
               tf_checks - checks_before,
               tf_failures - failures_before);
    }

    printf("------------------------------------------------------------\n");
    if (matched == 0) {
        printf("No suite matched filter '%s'.\n", filter);
        return 2;
    }
    printf("RESULT: %d checks, %d failures\n", tf_checks, tf_failures);
    return (tf_failures == 0) ? 0 : 1;
}
