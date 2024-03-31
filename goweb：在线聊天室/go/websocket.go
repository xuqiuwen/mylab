package main

import (
	"net/http"
	"sync"

	"github.com/gin-gonic/gin"
	"github.com/gorilla/websocket"
)

var upgrader = websocket.Upgrader{
    CheckOrigin: func(r *http.Request) bool {
        return true
    },
}

// 定义一个全局的连接池
var (
    activeConnections = make(map[*websocket.Conn]bool)
    connMutex = sync.Mutex{}
)

func wsHandler(c *gin.Context) {
    conn, err := upgrader.Upgrade(c.Writer, c.Request, nil)
    if err != nil {
        c.JSON(http.StatusInternalServerError, gin.H{"error": "Failed to upgrade to websocket"})
        return
    }
    defer conn.Close()

    // 将新的连接添加到连接池
    connMutex.Lock()
    activeConnections[conn] = true
    connMutex.Unlock()

    // 这里可以保持连接打开状态，但不需要读取消息
    for {
        _, _, err := conn.NextReader()
        if err != nil {
            break
        }
    }

    // 从连接池中移除断开的连接
    connMutex.Lock()
    delete(activeConnections, conn)
    connMutex.Unlock()
}

func broadcastMessage(message []byte) {
    connMutex.Lock()
    defer connMutex.Unlock()

    for conn := range activeConnections {
        if err := conn.WriteMessage(websocket.TextMessage, message); err != nil {
            delete(activeConnections, conn)
            conn.Close()
        }
    }
}
