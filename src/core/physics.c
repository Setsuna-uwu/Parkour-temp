#include "core/physics.h"

#include "core/config.h"

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

void physics_reset(Player *player, float x, float ground_y)
{
    if (player == 0) {
        return;
    }
    player->x = x;
    player->y = ground_y - SP_PLAYER_H;
    player->vy = 0.0f;
    player->on_ground = true;
    player->jumps_used = 0;
    player->coyote_timer = SP_COYOTE_TIME;
    player->jump_buffer_timer = 0.0f;
    player->run_phase = 0.0f;
}

void physics_update(Player *player, const Input *input, float ground_y, float dt)
{
    bool wants_jump;

    if (player == 0 || input == 0) {
        return;
    }
    if (dt <= 0.0f) {
        return;
    }

    if (player->on_ground) {
        player->coyote_timer = SP_COYOTE_TIME;
    } else {
        player->coyote_timer = clamp_float(player->coyote_timer - dt, 0.0f, SP_COYOTE_TIME);
    }

    player->jump_buffer_timer = clamp_float(player->jump_buffer_timer - dt, 0.0f, SP_JUMP_BUFFER_TIME);
    if (input->jump_pressed) {
        player->jump_buffer_timer = SP_JUMP_BUFFER_TIME;
    }

    wants_jump = (player->jump_buffer_timer > 0.0f);
    if (wants_jump) {
        if (player->on_ground || player->coyote_timer > 0.0f) {
            player->vy = SP_JUMP_SPEED;
            player->on_ground = false;
            player->jumps_used = 1;
            player->coyote_timer = 0.0f;
            player->jump_buffer_timer = 0.0f;
        } else if (player->jumps_used < SP_MAX_JUMPS) {
            player->vy = SP_JUMP_SPEED * 0.96f;
            player->on_ground = false;
            player->jumps_used += 1;
            player->jump_buffer_timer = 0.0f;
        }
    }

    player->vy = clamp_float(player->vy + SP_GRAVITY * dt, SP_JUMP_SPEED, SP_MAX_FALL_SPEED);
    player->y += player->vy * dt;

    if (player->y + SP_PLAYER_H >= ground_y) {
        player->y = ground_y - SP_PLAYER_H;
        if (player->vy > 0.0f) {
            player->vy = 0.0f;
        }
        player->on_ground = true;
        player->jumps_used = 0;
        player->coyote_timer = SP_COYOTE_TIME;
    } else {
        player->on_ground = false;
    }
}
