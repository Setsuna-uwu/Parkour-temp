# T12 · 暂停 / 重开完整流程

状态：**DONE**（已验收）

## 目标

P 暂停/恢复；GAME OVER 后 R 或鼠标点击重开；状态机无非法转换。

## 交付物

- game_update 状态机
- render_easyx.cpp 暂停/结算面板
- Input 的 pause/restart/mouse 字段

## 约束 / 禁止事项

- 暂停时时间、距离、物理冻结
- GAME OVER 时 P/跳跃无效
- 重开分数归零、最高分保留

## 门禁

```bat
scripts\test.bat --filter game
```

## 验收证据

game 套件覆盖暂停冻结、恢复、R 重开；docs/screenshot-gameover.png。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)