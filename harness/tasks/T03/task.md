# T03 · EasyX 空窗口 + 主循环

状态：**DONE**（已验收）

## 目标

打开 1280×720 窗口，以 QPC 固定 60 Hz 主循环运行，ESC/关闭按钮正常退出。

## 交付物

- src/platform/window_easyx.cpp：DPI、居中、图标、BeginBatchDraw
- src/platform/time_win32.cpp：QPC + 精确 sleep
- src/main.c：累积器、固定步、帧率上限

## 约束 / 禁止事项

- 客户区必须 1280×720
- 逻辑 dt 固定 1/60 s
- 不忙等整帧；最后 2 ms 才自旋

## 门禁

```bat
scripts\run.bat
```

## 验收证据

窗口截图 1302×776（含边框），客户区 1280×720；ESC 与标题栏关闭均退出。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)