# T01 · 项目骨架 + CMake

状态：**DONE**（已验收）

## 目标

建立可编译的 C/C++ 工程骨架，固定 x64 + MSVC 默认工具链，提供 harness 可调用的构建脚本。

## 交付物

- CMakeLists.txt：sp_core / SnowParkour / sp_tests 三个目标
- src/main.c：版本入口，暂时只验证构建
- scripts/build.bat、test.bat、run.bat 初版
- .gitignore 与目录结构

## 约束 / 禁止事项

- 必须使用 vendored EasyX，不写系统目录
- MSVC x64；后续保留 MinGW 分支
- 构建脚本在中文路径下可用

## 门禁

```bat
scripts\build.bat Release
```

## 验收证据

全新构建退出码 0；生成 build\Release\SnowParkour.exe。

---

上级门禁：[harness/gates.md](../../gates.md) · 需求：[docs/requirements.md](../../../docs/requirements.md)