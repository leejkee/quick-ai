# 🛠️ Development Guidelines (开发规范)

本项目采用 **Qt 官方代码风格** 和 **Modern CMake** 最佳实践。为了保持代码的一致性和可维护性，请遵循以下规范。

---

## 1. 技术栈要求 (Tech Stack)
* **C++ Standard**: C++17
* **Qt Version**: Qt 6.10+
* **Build System**: CMake 3.23+

---

## 2. 命名规范 (Naming Conventions)

### C++ 代码风格
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

### 文件与目录 (Files & Directories)

| 实体 | 风格 | 示例 | 理由 |
| :--- | :--- | :--- | :--- |
| **文件夹** | lowercase (全小写) | `src/core`, `src/ui/widgets` | 避免跨平台(Windows/Linux)大小写路径问题 |
| **C++ 类文件** | PascalCase | `LLMConversation.h`, `MainWindow.cpp` | 文件名与类名完全一致，方便 IDE 索引 |
| **QML 文件** | PascalCase | `ChatBubble.qml` | **强制要求**，否则无法作为组件实例化 |
| **入口/工具文件** | lowercase | `main.cpp`, `utils.h` | 非类定义的源文件 |

---

## 3. QML 代码规范

### 风格导入
* 使用 `QtQuick.Controls.Fusion` 作为默认控件风格
* 避免使用 Material 或其他特定风格

### 组织方式
* 使用 `// ===` 分隔符将属性分为 Colors 和 Dimensions 区域
* 颜色变量应使用 `palette` 属性实现主题适配
* 尺寸数值应使用局部变量统一管理
* 创建 `fusionMetrics` QtObject 统一样式度量（如 cornerRadius, borderWidth）

### 示例
```qml
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Fusion

Rectangle {
    id: root
    
    // ============================================
    // Colors - Fusion Style Palette
    // ============================================
    readonly property color backgroundColor: palette.window
    readonly property color borderColor: palette.mid
    
    // ============================================
    // Dimensions
    // ============================================
    readonly property int layoutMargin: 10
    readonly property int spacing: 8
    
    // ============================================
    // Fusion Style Metrics
    // ============================================
    QtObject {
        id: fusionMetrics
        readonly property int cornerRadius: 6
        readonly property int borderWidth: 1
    }
    
    radius: fusionMetrics.cornerRadius
    border.width: fusionMetrics.borderWidth
    border.color: borderColor
}
```

---

## 4. 提交规范

* 使用清晰的提交信息，描述做了什么以及为什么
* 遵循单一职责原则，每个提交只完成一个任务
* 提交前确保代码通过编译和基本测试

---

## 5. 文档规范

* 公共 API 必须添加文档注释
* 复杂逻辑应添加行内注释说明
* 修改文档时保持 Markdown 格式一致性
