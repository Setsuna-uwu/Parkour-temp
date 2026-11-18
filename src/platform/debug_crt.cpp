#include "platform/platform.h"

#ifdef _DEBUG
#include <crtdbg.h>
#include <fcntl.h>
#include <io.h>
#include <stdint.h>
#include <sys/stat.h>
#endif

extern "C" void platform_crt_init(void)
{
#ifdef _DEBUG
    int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    int leak_fd;

    flags |= _CRTDBG_ALLOC_MEM_DF;
    flags |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(flags);

    /*
     * GUI 程序没有控制台。把 CRT 警告（包括退出时的泄漏报告）写入
     * 工作目录下的 leak_report.txt，便于 harness 直接检查。
     * fd 故意保持打开，由 CRT 在进程退出时完成泄漏转储。
     */
    leak_fd = _open("leak_report.txt", _O_CREAT | _O_WRONLY | _O_TRUNC, _S_IREAD | _S_IWRITE);
    if (leak_fd != -1) {
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, (_HFILE)(intptr_t)leak_fd);
    } else {
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    }
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif
}
