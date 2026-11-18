# Harness 规格（可执行摘要）

## 项目

- 根目录：`C项目/`
- 游戏：雪原跑酷，Windows 10/11 x64 2D 横版无限跑酷 MVP
- 版本：0.1.0
- 核心语言：C11
- 适配层：C++17 + EasyX 26.10.25
- 构建：CMake + Visual Studio 生成器
- 音频：无

## 不可破坏的边界

1. `src/core/**` 只能包含标准 C 头文件；禁止 EasyX / Windows / C++ 头文件。
2. `src/platform/platform.h` 的 C 接口必须保持 `extern "C"`。
3. 不引入跨模块全局可变状态；`GameState` 由 `main` 持有并显式传递。
4. `core/` 不做文件 IO；最高分存储只在 `src/platform/storage_win32.cpp`。
5. 逻辑固定 60 Hz；不得改成可变 `dt` 直接驱动物理。
6. 随机源必须显式种子；禁止 `rand()` / `time()` 参与玩法。
7. 不把 EasyX 安装到系统目录；依赖保持在 `third_party/easyx`。

## 命令（门禁唯一入口）

```bat
:: 全新构建（默认 Release）
scripts\build.bat Release

:: Debug 构建
scripts\build.bat Debug

:: 全部测试（默认 Release）
scripts\test.bat

:: 按模块过滤
scripts\test.bat --filter physics
scripts\test.bat --filter collision
scripts\test.bat --filter terrain
scripts\test.bat --filter difficulty
scripts\test.bat --filter score
scripts\test.bat --filter game

:: Debug 测试（脚本自动补 Debug CRT PATH）
scripts\test.bat Debug

:: 运行
scripts\run.bat
```

## 交付物验收

| 项 | 检查 |
|---|---|
| 构建 | `build\Release\SnowParkour.exe` 存在，构建退出码 0 |
| 测试 | `sp_tests.exe` 退出码 0；765 checks / 0 failures |
| 窗口 | 1280×720 客户区；ESC 与关闭按钮均可退出 |
| 玩法 | 自动跑、跳跃/二段跳、障碍碰撞失败、P 暂停、R/鼠标重开 |
| 难度 | 120 s 自动玩家仍存活；speed > 480；level 7 |
| 最高分 | exe 旁生成 `highscore.dat`（20 字节）；重启后保留 |
| 泄漏 | Debug 正常退出后 `leak_report.txt` 为 0 字节 |
| 架构 | `rg "graphics.h|easyx.h|windows.h" src/core` 无结果 |

## 当前验收记录

- 构建工具链：Visual Studio 2026 / MSVC 19.51 / CMake 4.4.2
- EasyX：26.10.25
- 单元测试：765 assertions, 0 failures
- 自动跑酷：7200 逻辑步（120 s），保持 PLAYING
- 截图：`docs/screenshot.png`、`docs/screenshot-gameover.png`、`docs/screenshot-debug.png`
- Debug 泄漏：`leak_report.txt` 0 bytes
- 最高分示例：`build/Release/highscore.dat` 20 bytes，magic `SPHS0001`

## 任务

T01–T15 已完成，逐任务说明与门禁：

- `harness/tasks/T01/task.md` … `T15/task.md`
- 汇总门禁：`harness/gates.md`
- 需求基线：`docs/requirements.md`
- 架构决策：`docs/architecture.md`

## 失败处理约定

- 构建失败：先看 `build\*.log`（脚本重定向时）或 MSBuild 输出；修代码，不修改门禁来绕过。
- 测试失败：保留失败断言与复现步骤；不得删除测试或放宽断言。
- 画面/输入问题：用 F1 打开 AABB，确认坐标与状态。
- 最高分问题：检查 exe 同目录文件是否为 20 字节且 magic 正确；损坏文件应自动回退 0。
- Debug 无法启动：调用 `scripts\_debug_env.bat` 补 Debug CRT PATH。
