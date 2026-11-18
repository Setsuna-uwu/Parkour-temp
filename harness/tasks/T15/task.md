# T15 · MVP 验收

状态：**DONE**（已验收）

## 目标

完成可玩闭环：连续跑 2 分钟、难度递增、失败/重开/最高分正常、全部测试绿。

## 交付物

- Release 可执行文件
- 截图 3 张
- README/docs/harness 交付文档

## 约束 / 禁止事项

- 不加入 MVP 明确不做的音频/粒子/精灵动画
- 保持 core/platform 边界
- 验收命令必须可复现

## 门禁

```bat
scripts\test.bat + scripts\build.bat Release
```

## 验收证据

765 checks；120 秒自动局存活；实机截图；Debug 0 泄漏；最高分 20 字节。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)