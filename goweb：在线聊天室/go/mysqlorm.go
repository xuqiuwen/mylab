package main

import (
	"errors"
	"log"
	"time"

	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

type Orm struct {
	db *gorm.DB
}

type User struct { // 默认表名是 `users`
	Id       uint   `gorm:"primaryKey"`
	Nickname string `gorm:"unique"`
	Gender   string
}

type Message struct {
	Id        uint `gorm:"primaryKey"` // 消息ID，主键
	SenderId  uint `gorm:"index"`      // 发送者ID，建立索引以便快速查询
	Sender    string
	Content   string    // 消息内容
	CreatedAt time.Time // 消息发送时间
}

func MysqlConnect() *Orm {
	dsn := "root:254396@tcp(127.0.0.1:3306)/onlineChat?charset=utf8mb4&parseTime=True&loc=Local"
	db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	if err != nil {
		log.Fatal("failed to connect database", err)
	}
	db.AutoMigrate(&User{})
	db.AutoMigrate(&Message{})
	return &Orm{db: db}
}

func (orm *Orm) AddOrAlterUser(nickname, gender string) (User, error) {
    var user User

    // 用户是否存在
    result := orm.db.First(&user, "nickname = ?", nickname)

    if result.Error != nil {
        // 用户不存在，新建一个
        if errors.Is(result.Error, gorm.ErrRecordNotFound) {
            newUser := User{Nickname: nickname, Gender: gender}
            createResult := orm.db.Create(&newUser)
            if createResult.Error != nil {
                return User{}, createResult.Error
            }
            return newUser, nil
        } else {
            return User{}, result.Error
        }
    }

     // 用户存在，修改信息
    user.Gender = gender
    updateResult := orm.db.Save(&user)
    if updateResult.Error != nil {
        return User{}, updateResult.Error
    }

    return user, nil
}


func (orm *Orm) AddMessage(senderId uint, sender string,content string, createdAt time.Time) (Message, error) {
	message := Message{SenderId: senderId, Sender:sender,Content: content, CreatedAt: createdAt}
	result := orm.db.Create(&message)
	if result.Error != nil {
		return Message{}, result.Error
	}
	return message, nil
}

func (orm *Orm) GetUsers() ([]User, error) {
	var users []User
	result := orm.db.Find(&users) // 使用GORM的Find方法检索所有记录
	if result.Error != nil {
		return nil, result.Error // 如果有错误发生，返回空切片和错误信息
	}
	return users, nil // 没有错误发生，返回用户切片
}

func (orm *Orm) GetMessages() ([]Message, error) {
	var messages []Message
	result := orm.db.Find(&messages) // 使用GORM的Find方法检索所有记录
	if result.Error != nil {
		return nil, result.Error // 如果有错误发生，返回空切片和错误信息
	}
	return messages, nil // 没有错误发生，返回用户切片
}