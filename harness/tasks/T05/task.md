# T05 · GameState + 固定步更新骨架

状态：**DONE**（已验收）

## 目标

定义纯数据 GameState，建立 game_init/game_update/game_restart 状态机，main 显式传递指针。

## 交付物

- src/core/game_state.h
- src/core/game.c/.h
- 暂停/结束/重开状态转换

## 约束 / 禁止事项

- core 不允许全局可变状态
- GameState 由 main 栈上持有
- 暂停时 elapsed/distance 冻结

## 门禁

```bat
scripts\test.bat --filter game
```

## 验收证据

game 套件覆盖初始化、暂停冻结、恢复、重开、ESC。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)