## Quick-Ai-Assistant
实现两种方式访问 `deepseek api`: `streaming` 和 `non-streaming`

## 使用
### 配置API_KEY
安全起见，推荐将`api_key`配置到环境变量，cli工具会读取环境变量中的`DEEPSEEK_API_KEY`


## 🛠️ Development Guidelines (开发规范)

本项目采用 **Qt 官方代码风格** 和 **Modern CMake** 最佳实践。为了保持代码的一致性和可维护性，请遵循以下规范。

### 1. 技术栈要求 (Tech Stack)
* **C++ Standard**: C++17
* **Qt Version**: Qt 6.10+
* **Build System**: CMake 3.23+

### 2. 命名规范 (Naming Conventions)

#### C++ 代码风格
遵循 Qt 官方 API 设计风格：

| 类型 (Type) | 风格 (Style) | 示例 (Example) | 备注 |
| :--- | :--- | :--- | :--- |
| **类 / 结构体** | PascalCase (大驼峰) | `LLMConversation`, `ModelParams` | 与文件名保持一致 |
| **函数 / 方法** | camelCase (小驼峰) | `pushMessage()`, `getStartTime()` | 禁止使用 snake_case |
| **局部变量** | camelCase (小驼峰) | `currentIndex`, `responseBody` | |
| **成员变量** | `m_` + camelCase | `m_messagesHistory`, `m_apiKey` | Qt 源码标准，易于区分 |
| **枚举 / 常量** | PascalCase 或 ALL_CAPS | `MaxContextWindow`, `TIMEOUT_MS` | |
| **命名空间** | PascalCase | `QA::Core` | |

> **注意**: 对于 API 交互的数据结构 (DTO)，即使 JSON 字段是 `snake_case` (如 `frequency_penalty`)，C++ 成员变量仍应保持 **camelCase** (`frequencyPenalty`)，在序列化层进行映射。

#### 文件与目录 (Files & Directories)

| 实体 | 风格 | 示例 | 理由 |
| :--- | :--- | :--- | :--- |
| **文件夹** | lowercase (全小写) | `src/core`, `src/ui/widgets` | 避免跨平台(Windows/Linux)大小写路径问题 |
| **C++ 类文件** | PascalCase | `LLMConversation.h`, `MainWindow.cpp` | 文件名与类名完全一致，方便 IDE 索引 |
| **QML 文件** | PascalCase | `ChatBubble.qml` | **强制要求**，否则无法作为组件实例化 |
| **入口/工具文件** | lowercase | `main.cpp`, `utils.h` | 非类定义的源文件 |

### 3. 构建系统规范 (CMake)

* **Target 命名**: 内部 Target 使用 `snake_case`，对外别名使用 `Namespace::PascalCase`。
* **示例**:

```cmake
# 1. 定义库 (Internal Target)
add_library(app_core STATIC src/core/LLMConversation.cpp)

# 2. 定义别名 (Usage Target) - 链接时使用这个
add_library(QA::Core ALIAS app_core)

# 3. 链接
target_link_libraries(my_app PRIVATE QA::Core)
```

## Todo
- [x] LLM(DeepSeek) client cli
- [x] Conversation management
- [x] Multi-model switching capability
- [x] Docs
- [x] Gui based on QtQuick
  - [x] ui-session
  - [x] ui-settings
  - [ ] user-config
