# 任务门禁

所有任务均已完成。门禁命令从项目根目录执行。

| # | 任务 | 门禁命令 | 通过条件 | 状态 |
|---|---|---|---|---|
| T01 | 项目骨架 + CMake | `scripts\build.bat Release` | 退出码 0；生成 exe | DONE |
| T02 | 平台抽象接口 | `scripts\build.bat Release` | `platform.h` 仅 C 兼容声明；core 无平台头 | DONE |
| T03 | EasyX 窗口 + 主循环 | `scripts\run.bat` | 1280×720；60 FPS；ESC/关闭退出 | DONE |
| T04 | 输入抽象 | `scripts\test.bat --filter game` | 空格/P/R/ESC/F1/鼠标均映射；F1 HUD | DONE |
| T05 | GameState + 固定步 | `scripts\test.bat --filter game` | 暂停冻结；显式 `&state`；无 core 全局 | DONE |
| T06 | 玩家 + 地面渲染 | `scripts\run.bat` | 角色、地面、HUD 可见 | DONE |
| T07 | 重力 + 跳跃 + 二段跳 | `scripts\test.bat --filter physics` | 全部物理断言通过 | DONE |
| T08 | 程序化地形 v0 | `scripts\test.bat --filter terrain` | 生成/滚动/回收/确定性通过 | DONE |
| T09 | AABB + 失败 | `scripts\test.bat --filter collision` | 边界/失败/判定框通过 | DONE |
| T10 | 难度递增 v0 | `scripts\test.bat --filter difficulty` | 速度↑、间距↓、权重/等级↑ | DONE |
| T11 | 分数 + 最高分 | `scripts\test.bat --filter score` + 实机 | 计分；20 字节持久化；损坏回退 | DONE |
| T12 | 暂停 / 重开 | `scripts\test.bat --filter game` | P 冻结/恢复；R/点击重开 | DONE |
| T13 | CRT 泄漏检测 | `scripts\test.bat Debug` + Debug 运行 | 退出后 `leak_report.txt` 0 字节 | DONE |
| T14 | 核心单元测试 | `scripts\test.bat` | 765 checks / 0 failures | DONE |
| T15 | MVP 验收 | `scripts\test.bat` + `scripts\run.bat` | 2 分钟自动跑；全绿；截图 | DONE |

## 一键总门禁

```bat
scripts\test.bat
scripts\build.bat Release
```

预期：

```text
RESULT: 765 checks, 0 failures
[test] OK
[build] OK -> build\Release\SnowParkour.exe
```

## 额外人工检查

```text
[ ] 窗口标题为“雪原跑酷 · C 项目”
[ ] 1280×720 客户区，无裁切
[ ] 空格跳跃、空中二段跳
[ ] 撞冰锥/木箱后 GAME OVER
[ ] R 或鼠标点击重开，分数归零、最高分保留
[ ] P 暂停期间距离/时间冻结
[ ] F1 显示 FPS、AABB、seed
[ ] ESC 与标题栏 × 都能退出
[ ] build\Release\highscore.dat 重启后仍存在
```
