# T14 · 核心单元测试

状态：**DONE**（已验收）

## 目标

在无 EasyX 环境下覆盖 physics/collision/terrain/difficulty/score/game 的边界条件。

## 交付物

- tests/test_framework.h
- tests/test_main.c + 6 个套件
- CMake/CTest 接入
- test.bat --filter

## 约束 / 禁止事项

- 测试只链接 sp_core
- 不依赖桌面/窗口/文件
- 失败保留断言，不放宽门禁

## 门禁

```bat
scripts\test.bat
```

## 验收证据

765 checks / 0 failures；6 个 CTest 用例。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)