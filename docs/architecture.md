# 架构与决策记录

## 1. 总览

```text
┌─────────────────────────────────────────────────────────────┐
│ main.c (C11)                                                │
│  - 栈上持有 Platform / GameState / Input                    │
│  - QPC 累积器 + 固定 60 Hz 更新                             │
│  - 观察 score.high_score_dirty 并调用平台存储               │
└───────────────┬────────────────────────────┬────────────────┘
                │ extern "C"                 │ extern "C"
┌───────────────▼────────────────┐  ┌────────▼────────────────┐
│ core/ (纯 C11，可无 EasyX 测试) │  │ platform/ (C++17)       │
│ game_state  game               │  │ window_easyx            │
│ physics     collision          │  │ render_easyx            │
│ terrain     difficulty         │  │ input_easyx  time_win32 │
│ score       rng                │  │ storage_win32 debug_crt │
│ input_abstraction              │  │                         │
└────────────────────────────────┘  └─────────────────────────┘
```

依赖方向严格单向：

```text
main ──► core
  └────► platform ──► EasyX / Win32
platform ──► core（只读 GameState / 调用纯函数）
core ──► 标准 C 库
```

`platform/` 可以依赖 `core/`，`core/` 永不反向依赖平台。渲染层只读取 `const GameState *`。

## 2. 为什么这样分层

### ADR-001：C 核心 + C++ 薄适配层

EasyX 的头文件只支持 C++，但游戏规则没有理由绑定 C++ 或 EasyX。因此：

- `core/` 全部 `.c`，使用 `stdint.h` / `stdbool.h` / `stddef.h` / `string.h` / `math.h`。
- `platform/` 用 C++ 编译，但不承载玩法逻辑。
- 平台边界 `src/platform/platform.h` 用 `extern "C"` 包裹，避免 C/C++ 名称修饰不一致。

收益：核心测试无需图形环境，可在 CI/容器/无桌面机器运行；EasyX 可替换为其他渲染后端。

### ADR-002：固定时间步 + 累积器

物理与生成必须是确定性的，不能用可变 `dt`。

- `SP_FIXED_DT = 1/60 s`。
- 主循环用 QPC 计算真实帧间隔，累积后按固定步消费。
- 单帧最多 8 步；再多则丢弃积压，避免“死亡螺旋”式追帧。
- 输入边沿只让**第一个子步**看到，避免一帧内二段跳被重复触发。

收益：同一输入步序列可复现；不同帧率下玩法一致。

### ADR-003：显式状态，无全局可变变量

`GameState` 由 `main` 栈上创建，`game_update(&state, &input)`、`platform_render(&platform, &state, fps)` 显式传递。core 中只有 `static const` 模板表，没有可变全局。

收益：测试可创建多个独立世界；并行、回放、重置都简单。

### ADR-004：地形用“模板池 + 固定地面”

MVP 的地面是连续平地，地形段只保存障碍模板。这样：

- 地面绘制和碰撞简单，不会出现坑洞导致的“掉出世界”。
- 模板池仍满足“分段模板拼接 + 难度权重”的要求。
- 段之间的空隙可独立调整，便于保证最小反应距离。

后续若要加坑洞，可把 `TerrainSegment` 扩展为地面高度/缺口描述，渲染和地面碰撞增加采样即可，不影响 `physics` 的跳跃状态机。

### ADR-005：最高分文件校验 + 原子写

要求文件损坏时回退为 0，且不能写坏旧记录。

- 固定 20 字节记录，`#pragma pack(1)`。
- magic `SPHS0001`、`int32 score`、`uint32 checksum`、`uint32 version`。
- checksum：`(uint32)score * 2654435761 ^ 0xA5A5C0DE`。
- 先写 `highscore.tmp`，`fflush` 后 `MoveFileExW(..., MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)`。
- 读取任一项校验失败 → 返回 false，main 保持 0。

### ADR-006：EasyX 依赖 vendored

目标机器不要求预装 EasyX。`third_party/easyx` 包含：

- `include/easyx.h`、`graphics.h`
- `lib/x64/EasyXw.lib`、`EasyXa.lib`
- `lib/mingw/x64/libeasyxw.a`、`libeasyx.a`

CMake 直接链接 vendored `.lib`，并通过 `target_include_directories` 指向头文件。项目不写系统目录。

## 3. 数据流

### 启动

```text
main
  platform_crt_init()          Debug CRT 标志 + leak_report.txt
  platform_init()              DPI / initgraph / 居中 / BeginBatchDraw / QPC
  game_init(&state, seed)      清零，设屏幕/地面，开始第 1 局
  platform_storage_load...     读取最高分，校验失败则为 0
```

### 每帧

```text
frame_start = QPC
platform_pump()               检测窗口是否被关闭
platform_poll_input(&input)   消息队列 → Input 边沿/按住
frame_time = QPC - last
accumulator += min(frame_time, 0.25)
while accumulator >= 1/60:
    game_update(&state, &step_input)
    input_clear_edges(&step_input)
    accumulator -= 1/60
save highscore if dirty
platform_render(&platform, &state, fps)
platform_present()            FlushBatchDraw
sleep 到 16.67 ms 目标
```

### game_update

```text
1. 记录 last_input
2. F1 调试开关
3. ESC → quit_requested
4. GAME_OVER：累计 death_time；R / 鼠标 → game_restart
5. P：PLAYING ↔ PAUSED；PAUSED 直接返回
6. 非 PLAYING 返回
7. elapsed += dt；difficulty_at(elapsed)
8. distance += speed*dt
9. terrain_scroll(dx)：段和障碍一起左移、回收、补新段
10. physics_update：缓冲/土狼/跳跃/重力/落地
11. score_update
12. collision_check_player：命中则 GAME_OVER + score_finish
13. fixed_steps += 1
```

## 4. 模块职责

| 模块 | 职责 | 关键不变量 |
|---|---|---|
| `rng.c` | xorshift32 | 0 种子替换为非零；同种子同序列 |
| `physics.c` | 垂直运动、跳跃状态机 | 玩家 x 不变；落地才重置 jumps_used |
| `terrain.c` | 模板抽取、段/障碍滚动、回收 | 障碍坐标始终与段同步；段间空隙 ≥ 260 |
| `difficulty.c` | 时间 → 速度/间距/权重/等级 | 纯函数；负时间与超时钳制 |
| `collision.c` | AABB、玩家/障碍判定框 | 接触不算碰撞；零尺寸不碰撞 |
| `score.c` | 分数、最高分标记 | 不碰文件；reset 保留 high_score |
| `game.c` | 状态机与模块编排 | 暂停/结束冻结世界；唯一修改 GameState 的地方 |
| `platform/*` | 窗口/渲染/输入/计时/存储/CRT | 不改变玩法规则 |

## 5. 渲染层

`render_easyx.cpp` 是纯绘制代码，读取 `GameState`：

- 天空渐变、星、月、三层视差山、松林、雪地、土层。
- 角色由几何图元组成（身体、头、围巾、四肢），跑动相位来自距离；不是精灵动画。
- 障碍按类型绘制：冰锥、木箱、高冰柱、宽冰墙。
- HUD：分数、最高分、距离、速度、难度、等级、操作提示。
- 暂停/结算面板：固定居中，不依赖透明混合。
- F1：AABB 线框与参数面板。

所有坐标由相机（世界滚动）决定，渲染不修改状态。

## 6. 输入抽象

`Input` 是纯数据：

```c
bool jump_pressed;    /* 边沿 */
bool jump_held;       /* 按住 */
bool pause_pressed;
bool restart_pressed;
bool quit_pressed;
bool debug_pressed;
bool mouse_clicked;
```

- `input_easyx.cpp` 用 `peekmessage(&msg, EX_KEY | EX_MOUSE | EX_WINDOW)` 收集。
- `WM_KEYDOWN` + `!prevdown` 产生按下边沿。
- `GetAsyncKeyState(VK_SPACE)` 更新按住状态。
- `input_clear_edges` 在第一个固定子步后清边沿。

## 7. 测试策略

- 单元测试只链接 `sp_core`，不链接 EasyX，使用轻量宏断言。
- 测试套件：physics / collision / terrain / difficulty / score / game。
- 地形测试包含 7200 次滚动（120 s 等效）。
- 集成测试内置自动玩家：根据障碍宽度/高度和跳跃曲线计算起跳窗口，验证 2 分钟可通关。
- 确定性测试并排运行两个 `GameState`，逐项比对。
- 平台层通过实机验证：窗口、截图、关闭、最高分文件、Debug 泄漏报告。

## 8. 构建

- CMake ≥ 3.21，`project(... LANGUAGES C CXX RC)`。
- Visual Studio 生成器（脚本依据 vswhere 自动选择 2019/2022/2026）。
- 目标 `sp_core`（C 静态库）+ `SnowParkour`（GUI）+ `sp_tests`（控制台）。
- `/utf-8`、`/W4`、`/ENTRY:mainCRTStartup`（GUI 子系统但仍用 `main`）。
- EasyX 头文件与库全部相对项目路径。
- Release/Debug 输出到 `build/<Config>/`。

## 9. 已知取舍与后续

- 目前没有深坑/高低平台；地面碰撞只按固定 `ground_y`。
- 没有音频和资源加载；所有画面程序化。
- 未实现回放文件；确定性已经具备，后续可把输入步序列写入 `.replay`。
- 可以增加 MinGW CI；CMake 和 vendored MinGW 库已就位，但本次只验收 MSVC。
