# T09 · 碰撞检测 + 失败条件

状态：**DONE**（已验收）

## 目标

实现 AABB 碰撞，玩家撞障碍后进入 GAME OVER，并可在调试模式看到碰撞盒。

## 交付物

- src/core/collision.c/.h
- game.c 的失败转换与 collision 索引
- render_easyx.cpp 的 AABB 线框

## 约束 / 禁止事项

- 接触不算碰撞
- 冰锥判定框内缩；宽障碍左右内缩
- 碰撞后冻结物理并记录 last_hit_type

## 门禁

```bat
scripts\test.bat --filter collision
```

## 验收证据

collision 套件 18 checks；F1 截图显示玩家红框/障碍黄框。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)