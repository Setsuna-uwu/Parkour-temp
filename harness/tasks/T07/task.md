# T07 · 基础物理：重力 + 跳跃 + 二段跳

状态：**DONE**（已验收）

## 目标

实现重力、起跳、二段跳、土狼时间、跳跃缓冲与落地重置。

## 交付物

- src/core/physics.c/.h
- Player 状态结构
- tests/test_physics.c

## 约束 / 禁止事项

- 重力 2100；起跳 -880；最多两段
- 跳跃缓冲 0.13 s；土狼 0.10 s
- 不允许 core 依赖计时/平台

## 门禁

```bat
scripts\test.bat --filter physics
```

## 验收证据

physics 套件 20 checks / 0 failures。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)