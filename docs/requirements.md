# 需求规格 v0.1（已实现基线）

项目：**雪原跑酷 / Snow Parkour**  
目标：用 AI harness 驱动开发，在 Windows 10/11 上交付一个 C 为主的 2D 横版无限跑酷小游戏；本阶段交付可玩 MVP。

## 1. 目标与范围

- 自动向右跑酷，玩家只控制跳跃与二段跳。
- 程序化生成无限地形，难度随存活时间递增。
- 碰撞即失败，可暂停、重开、持久化最高分。
- 首版无音频、无精灵动画、无粒子、无商店、无手工关卡。
- 架构必须允许 `core/` 脱离 EasyX 独立编译和测试。

## 2. 平台与技术约束

| 项 | 决定 | 实现 |
|---|---|---|
| 目标系统 | Windows 10 / 11 x64 | 是 |
| 图形库 | EasyX | EasyX 26.10.25，vendored |
| 构建系统 | CMake，MSVC 默认 | 是；Visual Studio 生成器 |
| 语言标准 | C11 核心，C++14/17 适配层 | C11 + C++17 |
| 内存策略 | `malloc` / `free`；Debug CRT 泄漏检测 | core 不使用堆；Debug 启用 `_CRTDBG_LEAK_CHECK_DF` |
| 音频 | 首版无音频 | 无 |
| 资源 | 免费/可商用素材并记录来源；首版可占位 | 全部程序化绘制，无外部美术 |
| 窗口 | 1280×720 | 1280×720 客户区 |
| 帧率 | 60 FPS 固定时间步 | 逻辑 60 Hz，QPC 累积器 |

## 3. 架构边界（硬约束）

```text
main.c (C)
   │
   ├── core/       纯 C11，仅标准 C 头文件
   └── platform/   C++17 + EasyX / Win32
```

- `core/` 禁止 `#include <graphics.h>` / `<easyx.h>` / `<windows.h>`。
- `platform/platform.h` 以 `extern "C"` 暴露接口。
- 不允许跨模块全局可变状态；`GameState` 由 `main` 栈上持有并显式传递。
- `core/` 不负责文件 IO；最高分持久化只在 `platform/storage_win32.cpp`。

## 4. 功能需求

| ID | 需求 | 验收要点 | 状态 |
|---|---|---|---|
| F-01 | 窗口初始化与主循环 | 1280×720；60 FPS 固定时间步；ESC/关闭按钮正常退出 | 已完成 |
| F-02 | 自动向右跑酷 | 玩家屏幕 x 恒定；世界/地形向左滚动；距离计分 | 已完成 |
| F-03 | 跳跃 + 二段跳 | 空格；输入缓冲；土狼时间；最多两段 | 已完成 |
| F-04 | 程序化无限地形 | 8 种模板；段间最小空隙；回收；难度权重 | 已完成 |
| F-05 | 障碍碰撞即失败 | AABB；GAME OVER；F1 绘制碰撞盒 | 已完成 |
| F-06 | 暂停 / 重开 | P 暂停/恢复；GAME OVER 后 R 或鼠标点击重开 | 已完成 |
| F-07 | 分数 + 最高分 | 距离/时间计分；`highscore.dat`；损坏回退 0 | 已完成 |
| F-08 | 难度递增 | 速度↑、间距↓、复杂模板权重↑，集中在 `difficulty.c` | 已完成 |
| F-09 | 调试 HUD | FPS、AABB、难度参数；`SP_DEBUG_HUD` 宏开关 | 已完成 |

### F-01 细节

- 窗口标题：`雪原跑酷 · C 项目`。
- 主循环：`platform_poll_input` → 固定步 `game_update` → `platform_render` → `FlushBatchDraw`。
- QPC 累积器；单帧最多追 8 个逻辑步，超出则清空积压。
- 帧末按 60 Hz 目标休眠，使用 1 ms 定时器分辨率 + 最后 2 ms 自旋。

### F-02 细节

- 玩家屏幕碰撞盒 x 固定为 260 px，宽 42、高 56。
- 世界滚动距离累计到 `distance_px`；地形段与所属障碍一起左移。
- 距离换算：45 px = 1 m。

### F-03 细节

- 重力 2100 px/s²；起跳速度 -880 px/s。
- 二段跳初速为一段的 0.96 倍。
- 土狼时间 0.10 s；跳跃缓冲 0.13 s。
- 落地重置跳跃次数；下落速度上限 1500 px/s。

### F-04 细节

- 模板：单刺、单箱、双刺、高冰柱、宽冰墙、三连刺、高柱+刺、双箱。
- 生成参数来自 `DifficultyParams`；随机源为显式种子的 xorshift32。
- 段与段之间至少 260 px 物理空隙；初始安全区首个障碍在 1180 px。
- 左侧回收线 -180 px；上限 40 段，正常运行时约 5–8 段。

### F-05 细节

- 玩家 AABB 向内缩 5 px（水平）/ 6 px（顶部），减少视觉擦边误判。
- 冰锥判定框向内缩 5 px / 7 px；宽障碍左右各缩 3 px。
- 碰撞后进入 `GAME_PHASE_GAME_OVER`，冻结物理，更新最高分。

### F-06 / F-07 细节

- 暂停时时间、距离、物理全部冻结；暂停界面显示当前分数。
- GAME OVER 后 `R` 或鼠标左键重开；`P` / 跳跃无效。
- `highscore.dat` 紧邻 exe：8 字节 magic `SPHS0001` + int32 分数 + checksum + version。
- 写入先写 `highscore.tmp`，再 `MoveFileExW` 原子替换。
- 读取校验 magic / version / 范围和 checksum；任一失败返回 0。

### F-08 细节

`difficulty_at(t)`：

- 速度：`330 + 190 * (1 - exp(-t / 70))`，上限约 520 px/s。
- 间距：`470 - 110 * (t / 180)`，下限 360 px。
- 高难权重：`clamp(t / 150, 0, 1)`。
- 等级：`1 + floor(t / 20)`，封顶 10。

### F-09 细节

- `F1` 开关。
- 显示 FPS、速度、间距、hard weight、等级、状态、局数、seed、y/vy、跳跃/土狼/缓冲、输入、段数、碰撞索引。
- 玩家 AABB 红框，障碍 AABB 黄框，地面线橙色。
- CMake `SP_ENABLE_DEBUG_HUD=OFF` 可完全编译掉 F1 与调试绘制。

## 5. 非功能需求

### 可测试性

- `tests/` 只链接 `sp_core`，不链接 EasyX。
- 6 个测试套件，765 项断言，Release/Debug 均可运行。
- `scripts/test.bat --filter <suite>` 支持按模块过滤。

### 确定性

- 不使用 `time()` / `rand()`。
- 种子显式；每局由 `base_seed + run_index * 0x9E3779B9` 派生。
- 固定时间步；同种子 + 同输入步序列 ⇒ 相同地形、位置、分数、碰撞结果。
- 测试覆盖：同种子 600 步逐项对比。

### 内存安全

- core 不使用堆分配。
- Debug 构建设置 `_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF`，退出自动写 `leak_report.txt`。
- 已验证 Debug 正常退出时报告文件为 0 字节。
- Release 不启用额外检查。

### Harness 友好

- 每步可 `scripts/build.bat` 编译。
- 每步可 `scripts/test.bat` 跑测试。
- 任务拆分为 T01–T15，门禁在 `harness/gates.md`。

## 6. 明确不做（MVP）

- 精灵动画、粒子系统、音频。
- 鼠标/手柄玩法、设置菜单、商店/解锁。
- 手工关卡、Boss、多角色。
- 关卡编辑器、联网排行榜。

## 7. MVP 验收结论

| 验收项 | 结果 |
|---|---|
| 连续跑 2 分钟无崩溃 | 自动玩家 7200 逻辑步通过，仍在 PLAYING |
| 难度可感知递增 | 120 s 时 speed > 480，level 7 |
| 失败 / 重开 / 最高分 | 单元测试 + 实机截图 + `highscore.dat` 持久化 |
| `test.bat` 全绿 | 765 checks / 0 failures |
| 窗口 / 画面 | 1280×720 客户区，截图见 `docs/` |
| Debug 泄漏报告 | 0 字节 |
