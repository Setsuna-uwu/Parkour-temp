# T13 · CRT 泄漏检测接入

状态：**DONE**（已验收）

## 目标

Debug 构建退出时自动执行 CRT 泄漏检查，并输出到可检查文件。

## 交付物

- src/platform/debug_crt.cpp
- _CrtSetDbgFlag + _CRTDBG_LEAK_CHECK_DF
- scripts/_debug_env.bat

## 约束 / 禁止事项

- 仅 Debug，Release 无额外开销
- 不关闭 CRT 默认报告
- 不复制 Debug CRT 到发布目录

## 门禁

```bat
scripts\test.bat Debug + Debug 运行后关闭
```

## 验收证据

Debug 正常退出后 leak_report.txt 为 0 字节。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)