#include "core/input_abstraction.h"

#include <string.h>

void input_reset(Input *input)
{
    if (input != 0) {
        memset(input, 0, sizeof(*input));
    }
}

void input_clear_edges(Input *input)
{
    if (input == 0) {
        return;
    }
    input->jump_pressed = false;
    input->pause_pressed = false;
    input->restart_pressed = false;
    input->quit_pressed = false;
    input->debug_pressed = false;
    input->mouse_clicked = false;
}
