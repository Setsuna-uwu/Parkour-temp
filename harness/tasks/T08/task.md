# T08 · 程序化地形 v0

状态：**DONE**（已验收）

## 目标

用 8 种障碍模板生成无限地形，段和障碍同步滚动、回收，并保证最小反应距离。

## 交付物

- src/core/terrain.c/.h：模板池、生成、滚动、回收
- rng.c/.h：显式种子 xorshift32
- tests/test_terrain.c

## 约束 / 禁止事项

- 同种子同序列
- 障碍坐标必须与段同步移动
- 初始安全区、段间最小空隙 260 px
- 段数上限 40

## 门禁

```bat
scripts\test.bat --filter terrain
```

## 验收证据

terrain 套件 644 checks；120 秒 7200 次滚动不越界。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)