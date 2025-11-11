## UI

### InputView

- (Icon) (Input: Text) (Button: Enter)

> 按下enter键，或者点击Enter，读取输入Text为一个string，为空则不会进行该次请求，不为空则唤起一次请求，请求成功则将内容输出到OutputView

### StatusView

- (Status: "Input Prompt" "Waiting for response" "Successfully" "Failed")
- (Total tokens: ) if Status == "Successfully"

### OutputView
- (Vector<Message{Role, Text}>)
- extend: parser markdown
