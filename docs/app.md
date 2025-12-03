## lib
* client for chat
* r/w config file
* runtime settings

### init.json
```json
{
  "providers": [
    {
      "id": "DeepSeek",
      "baseUrl": "https://api.deepseek.com",
      "apiKey": "sk-xxxx", 
      "standard": "openai", 
      "models": [
        {
          "name": "deepseek-chat",
          "endpoint": "/chat/completions"
        },
        {
          "name": "deepseek-reasoner",
          "endpoint": "/chat/completions"
        }
      ]
    },
    {
      "id": "Qwen",
      "baseUrl": "https://dashscope.aliyuncs.com/compatible-mode/v1",
      "apiKey": "sk-xxxx",
      "standard": "openai",
      "models": [
        {
          "name": "qwen-plus",
          "endpoint": "/chat/completions"
        }
      ]
    }
  ],
  "activeConfig": {
    "provider": "DeepSeek",
    "modelName": "deepseek-chat"
  },
  "modelParams": {
    "max_tokens": 4096,
    "stream": true,
    "temperature": 0.5,
    "top_p": 0.95
  }
}
```

### Settings对象


## cli
维护有状态的settings对象，通过命令进入settings交互，更改配置，退出settings回到交互模式，此时chat使用settings保存的参数
- 模仿iwctl这类工具

### settings
- 从json对象中收集全部的model

## gui with qml

### settings service

- 接口
  - 读取配置文件
  - 写配置文件
  - 维护运行时配置

