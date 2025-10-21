## Quick-Ai-Assistant
一个桌面大语言模型助手，快捷键唤起，随时随地问答

## 使用

### 配置API_KEY
安全起见，推荐将`api_key`配置到环境变量，cli工具会读取环境变量中的`DEEPSEEK_API_KEY`
### CLI Tool

* `qacli-nostreamingclient`  
返回非流式数据，回复内容一次性呈现但是需要等待
* `qacli-streamingclient`  
返回流式数据chunk，回复多少显示多少，体验流畅


## Todo
- [x] LLM(Deepseek) client cli
- [x] Conversation management
- [ ] Gui based QtQuick
- [x] Multi-model switching capability 