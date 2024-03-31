package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/http"
	"time"

	"github.com/gin-gonic/gin"
)

type LoginInfo struct {
	Nickname string `json:"nickname"`
	Gender   string `json:"gender"`
}

var sessionStore = make(map[string]LoginInfo)

func printhttp(c *gin.Context) {
	// 打印请求的方法和路径
	fmt.Printf("Method: %s, Path: %s\n", c.Request.Method, c.Request.URL.Path)

	// 打印请求头
	for name, headers := range c.Request.Header {
		for _, h := range headers {
			fmt.Printf("Header: %v: %v\n", name, h)
		}
	}

	// 如果需要，打印请求体（对于流式请求体，这会消耗请求体）
	if bodyBytes, err := io.ReadAll(c.Request.Body); err == nil {
		// 将请求体读取为字符串
		bodyString := string(bodyBytes)
		fmt.Printf("Body: %s\n", bodyString)

		// 阅读后需要重新设置请求体
		c.Request.Body = io.NopCloser(bytes.NewBuffer(bodyBytes))
	}
}

func LoginEndpoint(c *gin.Context) {

    // 处理新的登录请求
    var loginInfo LoginInfo
    if err := c.ShouldBindJSON(&loginInfo); err != nil {
        c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
        return
    }

    user, err := orm.AddOrAlterUser(loginInfo.Nickname, loginInfo.Gender)
    if err != nil {
        c.JSON(http.StatusInternalServerError, gin.H{"error": "无法添加用户"})
        return
    }


    c.JSON(http.StatusOK, gin.H{"message": "登录成功", "nickname": user.Nickname, "gender": user.Gender, "userid": user.Id})
}



// func GetMessagesEndpoint(c *gin.Context) {

// 	c.JSON(http.StatusOK, gin.H{"messages": []string{"Hello World", "Hi there!"}})
// }

func PostMessageEndpoint(c *gin.Context) {
	var newMessage struct {
		SenderId uint   `json:"senderid"`
		Sender   string `json:"sender"`
		Message  string `json:"message"`
	}
	//printhttp(c)

	// 尝试绑定JSON到newMessage结构体
	if err := c.ShouldBindJSON(&newMessage); err != nil {
		log.Printf("%v", err)
		c.JSON(http.StatusBadRequest, gin.H{"error": err.Error()})
		return
	}
	fmt.Println(newMessage)

	timenow :=time.Now()
	message, err := orm.AddMessage(newMessage.SenderId, newMessage.Sender, newMessage.Message, timenow)
	if err != nil {
		log.Printf("%v", err)
		c.JSON(http.StatusInternalServerError, gin.H{"error": "无法添加消息"})
		return
	}

	formattedMessage := struct {
		Sender    string `json:"Sender"`
		CreatedAt string `json:"CreatedAt"` // 假设这里是字符串形式的时间戳
		Content   string `json:"Content"`
	}{
		Sender:    newMessage.Sender,
		CreatedAt: time.Now().Format(time.RFC3339), // 格式化当前时间
		Content:   newMessage.Message,
	}
	// 将消息格式化为 JSON
	jsonMsg, err := json.Marshal(formattedMessage)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "无法序列化消息"})
		return
	}
	// 群发 JSON 消息
	broadcastMessage(jsonMsg)

	// 如果一切顺利，返回成功的响应
	c.JSON(http.StatusOK, gin.H{"message": "消息已发送", "message_id": message.Id})
}

func GetUsersEndpoint(c *gin.Context) {
	users, err := orm.GetUsers() // 获取用户列表
	if err != nil {
		// 如果有错误发生，则返回HTTP状态码500和错误信息
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	// 如果成功，返回用户列表
	c.JSON(http.StatusOK, users)
}

func GetMessagesEndpoint(c *gin.Context) {
	users, err := orm.GetMessages()
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}
	c.JSON(http.StatusOK, users)
}
