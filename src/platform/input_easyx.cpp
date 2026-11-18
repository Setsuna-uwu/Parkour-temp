#include "platform/platform.h"

#include <graphics.h>
#include <windows.h>

extern "C" void platform_poll_input(Platform *platform, Input *input)
{
    ExMessage message;
    HWND hwnd;

    if (input == 0) {
        return;
    }
    input_reset(input);

    if (platform == 0 || !platform->initialized) {
        return;
    }

    while (peekmessage(&message, EX_KEY | EX_MOUSE | EX_WINDOW)) {
        switch (message.message) {
        case WM_KEYDOWN:
            if (!message.prevdown) {
                switch (message.vkcode) {
                case VK_SPACE:
                    input->jump_pressed = true;
                    break;
                case 'P':
                    input->pause_pressed = true;
                    break;
                case 'R':
                    input->restart_pressed = true;
                    break;
                case VK_ESCAPE:
                    input->quit_pressed = true;
                    break;
                case VK_F1:
                    input->debug_pressed = true;
                    break;
                default:
                    break;
                }
            }
            break;
        case WM_LBUTTONDOWN:
            input->mouse_clicked = true;
            break;
        case WM_CLOSE:
        case WM_DESTROY:
            platform->quit = true;
            break;
        default:
            break;
        }
    }

    input->jump_held = ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0);

    hwnd = static_cast<HWND>(platform->hwnd);
    if (hwnd == 0 || !IsWindow(hwnd)) {
        platform->quit = true;
    }
}
