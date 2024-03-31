package main

import (
	"time"

	"github.com/gin-contrib/cors"
	"github.com/gin-contrib/sessions"
	"github.com/gin-contrib/sessions/cookie"
	"github.com/gin-gonic/gin"
	_ "github.com/go-sql-driver/mysql"
)

var orm *Orm //数据库操作

func main() {
	orm = MysqlConnect() //连接数据库

	ses := sessions.Sessions("mysession", cookie.NewStore([]byte("secret"))) //创建session

	config := cors.Config{
		AllowOrigins:     []string{"http://localhost:5173"},
		AllowMethods:     []string{"GET", "POST", "PUT", "PATCH", "DELETE", "OPTIONS"},
		AllowHeaders:     []string{"Origin", "Content-Type", "Accept", "Authorization", "X-Requested-With", "X-CSRF-Token"},
		ExposeHeaders:    []string{"Content-Length", "Set-Cookie"},
		AllowCredentials: true,
		AllowOriginFunc: func(origin string) bool {
			return origin == "http://localhost:5173"
		},
		MaxAge: 12 * time.Hour,
	}
	
	router := gin.Default()
	router.Use(cors.New(config), ses)
	v1 := router.Group("api/v1")
	{
		v1.POST("/login", LoginEndpoint)
		v1.POST("/send", PostMessageEndpoint)
		v1.GET("/users", GetUsersEndpoint)
		v1.GET("/messages", GetMessagesEndpoint)
		v1.GET("/ws", wsHandler)
	}
	router.Run(":8080")

}
