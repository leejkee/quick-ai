你是一位精通 Qt6 和 QML 的资深 UI 架构师。请帮我编写一个健壮的桌面应用设置界面系统，需完美兼容 Qt Design Studio 预览，并能通过 qml6 MainView.qml 直接运行测试。

# 核心工程准则 (Strict Guidelines)

请严格遵守以下 5 条准则，任何违背准则的代码都将被视为错误：

## 表单排版规范 (Form Layouts)：

- 多行表单：必须使用 GridLayout (columns: 2)。
- 左列 Label：垂直居中 (Layout.alignment: Qt.AlignVCenter)。
- 右列 Control：必须设置 Layout.fillWidth: true 以适应窗口变化。
- 单行组合：仅在“Label + Switch”等简单场景使用 RowLayout。
- 必须在中间插入 Item { Layout.fillWidth: true } (Spacer) 以实现两端对齐。

## 防溢出约束 (Overflow Protection) [极重要]：

- 容器宽度：ColumnLayout 或 GridLayout 必须绑定宽度（width: parent.width 或 anchors.left/right）。
- 禁止硬编码：严禁给输入控件（TextField, ComboBox）设置固定像素宽度（如 width: 200）。必须依靠 Layout.fillWidth: rue。
- 文本截断：长文本 Label 必须设置 elide: Text.ElideRight 或 wrapMode: Text.Wrap。

## 容器与尺寸 (Sizing)：

- 防塌陷：GroupBox 等容器高度必须由内容 (implicitHeight) 撑开，严禁内部布局使用 anchors.fill: parent。
- 页面根节点：子页面根节点推荐使用 ScrollView，并设置默认尺寸 width: 800; height: 600 以便设计器预览。

## 控件标准 (Controls)：

- 使用 QtQuick.Controls (Qt6) 标准组件。
- 利用 GroupBox (Layout.fillWidth: true) 对设置项进行逻辑分组。
- 颜色规范：确保所有 Label、Text 和 TextArea 的字体颜色与背景对比鲜明（例如：在浅色背景下强制使用深色字体 #1e1e1e），严禁出现“白底白字”的情况。

## MVVM 与 Mock 测试：

- UI 仅通过属性（Property）与逻辑层交互。
- 必须提供 Mock 实现：在 MainView.qml 中实例化一个 QtObject 作为 Mock ViewModel，包含真实的 JavaScript 逻辑来模拟 C++ 行为（例如：切换 Provider 时自动更新 Model 列表），确保 qml 命令直接运行能看到交互效果。

# 具体需求描述

## 1. 架构与文件

- MainView.qml：主窗口。
  - 布局：左侧 Sidebar (ListView) + 右侧 Content (StackLayout)。
  - 包含 Mock 数据的定义。
- General.qml：通用设置页。
- LLMConfig.qml：大模型配置页。

## 2. 页面详细规范

### 通用规范：

- 每个子页面顶部必须有一个一级标题 (Label, font size 24, bold)。
- 子页面根组件使用 ScrollView，内容使用 ColumnLayout。
- 子页面需声明对应的 ViewModel 属性：property var generalViewModel / property var llmConfigViewModel。
- 关于字，注意不要使用和背景相同的颜色，比如通常light主题是白色背景，不宜使用白色字体
### MainView.qml (主界面)：

- 侧边栏需实现：当前选中项高亮、鼠标悬停效果。
- Mock ViewModel 需模拟 C++ 信号机制。

### General.qml (子页面)：

- 标题："General Settings"
- Group 1: Appearance
  - Label + ComboBox (Theme: Light/Dark)。
  - Label + SpinBox (Font Size)。
- Group 2: Default Prompt
  - 包含一个 System Instruction 的多行文本输入框 (TextArea)，需清晰显示边框和背景，确保文字可读。

### LLMConfig.qml (子页面)：

- 标题："LLM Configuration"
- Group 1: Provider Selection
  - Label + ComboBox (Provider)。
  - 交互逻辑：选择不同的 Provider（如 DeepSeek/Qwen），下方的 Model 列表必须动态改变（由 Mock 逻辑驱动）。
- Group 2: Model Selection
  - Label + ComboBox (Model)。
- Group 3: Parameters
  - 使用 GridLayout。
  - Label + Slider (Temperature, 0.0 - 2.0, 显示当前数值)。
  - Label + SpinBox 或 Slider (Max Tokens)。

# 输出要求

请提供 3 个完整的文件代码 (MainView.qml, General.qml, LLMConfig.qml)。代码是可以直接复制并运行的。