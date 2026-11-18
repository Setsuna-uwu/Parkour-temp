# T11 · 分数 + 最高分持久化

状态：**DONE**（已验收）

## 目标

按距离/时间计分；最高分写入 exe 同目录 highscore.dat，损坏自动回退 0。

## 交付物

- src/core/score.c/.h
- src/platform/storage_win32.cpp
- tests/test_score.c

## 约束 / 禁止事项

- core 不做文件 IO
- 文件带 magic/version/checksum
- 原子替换写；损坏返回 0
- reset 保留最高分

## 门禁

```bat
scripts\test.bat --filter score + 实机
```

## 验收证据

score 16 checks；highscore.dat 为 20 字节，magic SPHS0001，重开后保留。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)