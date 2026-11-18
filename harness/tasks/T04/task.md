# T04 · 输入抽象

状态：**DONE**（已验收）

## 目标

把空格、P、R、ESC、F1 与鼠标左键映射到 Input，并让调试 HUD 显示按键/状态。

## 交付物

- src/platform/input_easyx.cpp：peekmessage + GetAsyncKeyState
- Input 边沿字段与 input_clear_edges
- F1 调试面板中的输入行

## 约束 / 禁止事项

- 按键重复（prevdown）不能产生重复边沿
- 边沿只允许第一个固定子步消费
- 鼠标只用于 GAME OVER 重开

## 门禁

```bat
scripts\test.bat --filter game
```

## 验收证据

F1 截图可见“空格 松开 / P 0 / R 0”；游戏测试覆盖 ESC/F1/暂停/重开。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)