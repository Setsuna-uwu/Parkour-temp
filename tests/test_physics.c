#include "core/config.h"
#include "core/input_abstraction.h"
#include "core/physics.h"

#include "test_framework.h"

void test_physics_suite(void)
{
    Player player;
    Input input;
    int i;

    physics_reset(&player, 100.0f, SP_GROUND_Y);
    TF_CHECK(player.on_ground);
    TF_CHECK_INT(player.jumps_used, 0);
    TF_CHECK_NEAR(player.y, SP_GROUND_Y - SP_PLAYER_H, 0.01f);

    input_reset(&input);
    input.jump_pressed = true;
    physics_update(&player, &input, SP_GROUND_Y, SP_FIXED_DT);
    TF_CHECK(!player.on_ground);
    TF_CHECK(player.vy < 0.0f);
    TF_CHECK_INT(player.jumps_used, 1);

    input_clear_edges(&input);
    for (i = 0; i < 14; ++i) {
        physics_update(&player, &input, SP_GROUND_Y, SP_FIXED_DT);
    }

    input.jump_pressed = true;
    physics_update(&player, &input, SP_GROUND_Y, SP_FIXED_DT);
    TF_CHECK_INT(player.jumps_used, 2);
    TF_CHECK(player.vy < 0.0f);

    input_clear_edges(&input);
    input.jump_pressed = true;
    physics_update(&player, &input, SP_GROUND_Y, SP_FIXED_DT);
    TF_CHECK_INT(player.jumps_used, 2);

    input_reset(&input);
    for (i = 0; i < 300 && !player.on_ground; ++i) {
        physics_update(&player, &input, SP_GROUND_Y, SP_FIXED_DT);
    }
    TF_CHECK(player.on_ground);
    TF_CHECK_INT(player.jumps_used, 0);

    /* 落地前按下跳跃：缓冲不应浪费，落地后的下一次更新应起跳。 */
    physics_reset(&player, 100.0f, SP_GROUND_Y);
    player.on_ground = false;
    player.y = SP_GROUND_Y - SP_PLAYER_H - 5.0f;
    player.vy = 0.0f;
    player.jumps_used = SP_MAX_JUMPS;
    player.coyote_timer = 0.0f;

    input_reset(&input);
    input.jump_pressed = true;
    physics_update(&player, &input, SP_GROUND_Y, SP_FIXED_DT);
    input_clear_edges(&input);
    for (i = 0; i < 20 && !player.on_ground; ++i) {
        physics_update(&player, &input, SP_GROUND_Y, SP_FIXED_DT);
    }
    TF_CHECK(player.on_ground);
    TF_CHECK(player.jump_buffer_timer > 0.0f);
    physics_update(&player, &input, SP_GROUND_Y, SP_FIXED_DT);
    TF_CHECK(!player.on_ground);
    TF_CHECK_INT(player.jumps_used, 1);

    /* 土狼时间：刚离开地面仍可起跳。 */
    physics_reset(&player, 100.0f, SP_GROUND_Y);
    player.on_ground = false;
    player.y = SP_GROUND_Y - SP_PLAYER_H - 2.0f;
    player.vy = 0.0f;
    player.coyote_timer = 0.08f;
    player.jumps_used = 0;
    input_reset(&input);
    input.jump_pressed = true;
    physics_update(&player, &input, SP_GROUND_Y, SP_FIXED_DT);
    TF_CHECK_INT(player.jumps_used, 1);
    TF_CHECK(player.vy < 0.0f);

    /* 下落速度有上限。 */
    physics_reset(&player, 100.0f, SP_GROUND_Y);
    player.on_ground = false;
    player.y = 0.0f;
    player.vy = 9000.0f;
    input_reset(&input);
    physics_update(&player, &input, SP_GROUND_Y, SP_FIXED_DT);
    TF_CHECK(player.vy <= SP_MAX_FALL_SPEED + 0.01f);

    /* dt 为 0 时状态不变。 */
    physics_reset(&player, 100.0f, SP_GROUND_Y);
    physics_update(&player, &input, SP_GROUND_Y, 0.0f);
    TF_CHECK(player.on_ground);
    TF_CHECK_NEAR(player.y, SP_GROUND_Y - SP_PLAYER_H, 0.01f);
}
