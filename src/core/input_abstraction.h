/*
 * 输入抽象：platform 层负责填充，core 层只读取。
 */
#ifndef SP_INPUT_ABSTRACTION_H
#define SP_INPUT_ABSTRACTION_H

#include <stdbool.h>

typedef struct Input {
    bool jump_pressed;    /* 本帧按下（边沿） */
    bool jump_held;       /* 当前按住 */
    bool pause_pressed;   /* 本帧按下 P */
    bool restart_pressed; /* 本帧按下 R */
    bool quit_pressed;    /* 本帧按下 ESC */
    bool debug_pressed;   /* 本帧按下 F1 */
    bool mouse_clicked;   /* 本帧左键按下 */
} Input;

void input_reset(Input *input);
void input_clear_edges(Input *input);

#endif /* SP_INPUT_ABSTRACTION_H */
