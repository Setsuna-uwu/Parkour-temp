# T02 · 平台抽象接口定义

状态：**DONE**（已验收）

## 目标

把窗口、输入、渲染、计时、存储的边界固定为 C 接口，保证 core 可脱离 EasyX。

## 交付物

- src/platform/platform.h：extern "C" 生命周期接口
- src/core/input_abstraction.h/.c：Input 位掩码与边沿清理
- src/core/game_state.h：纯数据结构与模块组合

## 约束 / 禁止事项

- platform.h 不暴露 HWND、ExMessage、EasyX 类型
- core 头文件只包含标准 C 头
- 不实现具体 EasyX 调用

## 门禁

```bat
scripts\build.bat Release
```

## 验收证据

core 与 platform 分别编译；include 边界检查通过。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)