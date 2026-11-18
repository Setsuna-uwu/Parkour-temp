#include "platform/platform.h"

#include <graphics.h>
#include <windows.h>

#include <mmsystem.h>

extern "C" bool platform_init(Platform *platform, const wchar_t *title, int width, int height)
{
    HWND hwnd;
    RECT rect;
    int window_width;
    int window_height;
    int x;
    int y;

    if (platform == 0 || width <= 0 || height <= 0) {
        return false;
    }

    platform->initialized = false;
    platform->quit = false;
    platform->width = width;
    platform->height = height;
    platform->hwnd = 0;
    platform->start_time = 0.0;

    SetProcessDPIAware();
    timeBeginPeriod(1);

    hwnd = initgraph(width, height);
    if (hwnd == 0) {
        timeEndPeriod(1);
        return false;
    }

    if (title != 0) {
        SetWindowTextW(hwnd, title);
    }

    {
        HICON icon = LoadIconW(GetModuleHandleW(0), MAKEINTRESOURCEW(1));
        if (icon != 0) {
            SendMessageW(hwnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
            SendMessageW(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
        }
    }

    GetWindowRect(hwnd, &rect);
    window_width = rect.right - rect.left;
    window_height = rect.bottom - rect.top;
    x = (GetSystemMetrics(SM_CXSCREEN) - window_width) / 2;
    y = (GetSystemMetrics(SM_CYSCREEN) - window_height) / 2;
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    SetWindowPos(hwnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

    setbkcolor(RGB(9, 13, 30));
    cleardevice();
    BeginBatchDraw();

    platform->hwnd = hwnd;
    platform->initialized = true;
    platform_time_init(platform);
    platform->start_time = platform_now_seconds(platform);

    return true;
}

extern "C" void platform_shutdown(Platform *platform)
{
    if (platform == 0) {
        return;
    }

    if (platform->initialized) {
        HWND hwnd = static_cast<HWND>(platform->hwnd);
        if (hwnd != 0 && IsWindow(hwnd)) {
            EndBatchDraw();
            closegraph();
        }
        timeEndPeriod(1);
    }

    platform->initialized = false;
    platform->hwnd = 0;
}

extern "C" void platform_pump(Platform *platform)
{
    if (platform == 0 || !platform->initialized) {
        return;
    }
    if (platform->hwnd == 0 || !IsWindow(static_cast<HWND>(platform->hwnd))) {
        platform->quit = true;
    }
}

extern "C" void platform_present(Platform *platform)
{
    if (platform != 0 && platform->initialized && !platform->quit) {
        FlushBatchDraw();
    }
}

extern "C" bool platform_should_quit(const Platform *platform)
{
    return (platform == 0) || platform->quit;
}
