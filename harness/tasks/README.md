# Harness 任务索引

| # | 任务 | 门禁 | 状态 |
|---|---|---|---|
| T01 | 项目骨架 + CMake | `scripts\build.bat Release` | DONE |
| T02 | 平台抽象接口定义 | `scripts\build.bat Release` | DONE |
| T03 | EasyX 空窗口 + 主循环 | `scripts\run.bat` | DONE |
| T04 | 输入抽象 | `scripts\test.bat --filter game` | DONE |
| T05 | GameState + 固定步 | `scripts\test.bat --filter game` | DONE |
| T06 | 玩家 + 地面渲染 | `scripts\run.bat` | DONE |
| T07 | 重力 + 跳跃 + 二段跳 | `scripts\test.bat --filter physics` | DONE |
| T08 | 程序化地形 v0 | `scripts\test.bat --filter terrain` | DONE |
| T09 | AABB + 失败条件 | `scripts\test.bat --filter collision` | DONE |
| T10 | 难度递增 v0 | `scripts\test.bat --filter difficulty` | DONE |
| T11 | 分数 + 最高分 | `scripts\test.bat --filter score` | DONE |
| T12 | 暂停 / 重开 | `scripts\test.bat --filter game` | DONE |
| T13 | CRT 泄漏检测 | `scripts\test.bat Debug` | DONE |
| T14 | 核心单元测试 | `scripts\test.bat` | DONE |
| T15 | MVP 验收 | 全量测试 + 实机 | DONE |

逐任务详情：`T01/task.md` … `T15/task.md`。  
汇总门禁：[../gates.md](../gates.md)。  
需求：[../../docs/requirements.md](../../docs/requirements.md)。
