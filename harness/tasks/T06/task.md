# T06 · 玩家矩形 + 地面渲染

状态：**DONE**（已验收）

## 目标

用几何图元绘制雪原、地面、跑者、HUD，形成可辨识的 MVP 画面。

## 交付物

- src/platform/render_easyx.cpp：天空/山/松林/地面/角色/HUD
- src/core/config.h：尺寸与配色无关的玩法常量
- 应用图标 assets/icon.ico + app.rc

## 约束 / 禁止事项

- 只读 GameState，不在渲染中改状态
- 不使用外部图片资源
- 字符使用 Unicode EasyXw

## 门禁

```bat
scripts\run.bat
```

## 验收证据

docs/screenshot.png；角色、雪地、山脉、HUD 正常。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)