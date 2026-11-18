# T10 · 难度递增 v0

状态：**DONE**（已验收）

## 目标

根据存活时间输出速度、间距、复杂模板权重和等级，让难度可感知递增。

## 交付物

- src/core/difficulty.c/.h
- HUD 显示 speed/gap/level
- tests/test_difficulty.c

## 约束 / 禁止事项

- 纯函数，只依赖时间
- 速度与权重单调升，间距单调降
- 负时间/超过 180 s 钳制

## 门禁

```bat
scripts\test.bat --filter difficulty
```

## 验收证据

difficulty 22 checks；120 秒自动局 speed > 480、level 7。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)