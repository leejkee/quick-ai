<p align="center">
  <img src="docs/screenshot/logo.svg" alt="Quick-AI Logo" width="120">
</p>

<h1 align="center">Quick-AI</h1>

<p align="center">
  <strong>由 C++17 与 QtQuick 构建的轻量、跨平台 LLM GUI 客户端</strong>
  <br />
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-blue?style=flat-square&logo=linux" alt="Platform">
  <img src="https://img.shields.io/badge/Qt-6.8%2B-green?style=flat-square&logo=qt" alt="Qt Version">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=c%2B%2B" alt="C++ Standard">
  <a href="https://github.com/leejkee/quick-ai/stargazers">
    <img src="https://img.shields.io/github/stars/leejkee/quick-ai?style=flat-square&logo=github" alt="Stars">
  </a>
</p>

<p align="center">
  <a href="#项目简介">项目简介</a> •
  <a href="#使用方法">使用方法</a> •
  <a href="#编译">编译指南</a> •
  <a href="#todo">开发计划</a>
</p>

---
# Quick-AI-Assistant

一款支持多模型切换的智能对话助手，提供 CLI 和 Qt Quick GUI 两种交互方式。基于 C++17 和 Qt 6 构建，采用 DeepSeek API 实现流式和非流式对话。

---

## 项目简介

Quick-AI-Assistant 是一个轻量级、跨平台的 AI 对话客户端，支持以下特性：

- **多模型支持**：支持 DeepSeek 及其他 OpenAI 兼容的 Provider
- **对话管理**：支持多会话管理和历史记录
- **配置灵活**：通过环境变量或 GUI 设置 API 密钥和参数
- **跨平台**：支持 Windows 和 Linux

---

## 界面演示(Arch linux/Wayland/COSMIC)

### Chat界面
![Chat Interface](docs/screenshot/chat.png)

### Settings界面
![Settings Interface](docs/screenshot/settings.png)

---

## 使用

### cli tool 
- 需要将api key配置到env


```bash
# Windows PowerShell
$env:DEEPSEEK_API_KEY="your-api-key-here"
```

### quick-ai app
- 第一次运行时需要在设置页面配置provider
- chat窗口由快捷键ctrl+alt+enter启动


---

## 编译

### 环境

- **C++ Standard**: C++17
- **Qt Version**: Qt 6.8+
- **Build System**: CMake 3.23+
- **Generator**: Ninja (推荐)

### 使用 CMakePresets.json（推荐）

本项目提供了 `CMakePresets.json` 配置文件，支持一键配置和构建：

#### Windows

```bash
# 配置 Debug 版本
cmake --preset=windows-debug

# 构建
cmake --build build/windows-debug

# 配置 Release 版本
cmake --preset=windows-release
cmake --build build/windows-release
```

#### Linux


### 编译选项

| 选项 | 说明 | 默认值 |
| :--- | :--- | :--- |
| `QA_BUILD_UI` | 构建 Qt Quick GUI | ON |
| `QA_BUILD_TESTS` | 构建测试 | OFF |
| `CMAKE_BUILD_TYPE` | 构建类型 (Debug/Release) | Release |

---

## Development Guidelines

[Development Guidelines](docs/dev_guide.md)

---

## TODO

- [x] LLM (DeepSeek) client CLI
- [x] Conversation management
- [x] Multi-model switching capability
- [x] Documentation
- [ ] Streaming-chat
- [x] GUI based on Qt Quick
  - [x] UI - Session (聊天界面)
  - [x] UI - Settings (设置界面)
  - [x] User Configuration (用户配置)
  - [ ] Style switch ()
- [x] Application icon
---
