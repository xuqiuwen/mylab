<!-- ChatHall.vue -->
<template>
    <div class="chat-hall">
        <div class="header">
            <h1 class="chat-title">Chat Hall</h1>
            <button class="login-button" @click="toggleLogin">{{ isLogged ? user.nickname : '登录' }}</button>
        </div>
        <div class="message-list-container">
            <MessageList />
        </div>
        <div class="input-bar-container">
            <MessageInputBar />
        </div>
        <UserLogin v-if="showLogin" @login-success="toggleLogin" />
    </div>
</template>

  
<script setup>
import MessageList from './MessageHistory.vue';
import MessageInputBar from './MessageInputBar.vue';
import UserLogin from './UserLogin.vue';
import { ref } from 'vue';
import { user, isLogged } from '/src/composables/useUser.js';



const showLogin = ref(false);
const toggleLogin = () => {
    showLogin.value = !showLogin.value;
};
</script>
<style scoped>
.chat-hall {
    flex-direction: column;
    height: 100vh;
    /* Full viewport height */
    padding-top: 60px;
    /* Space for header */
    padding-bottom: 70px;
    /* Space for input bar */
}

.header {
    position: fixed;
    /* Fixed to the top */
    top: 0;
    /* Align to the left */
    right: 10px;
    /* Stretch across the width */
    width: 76%;
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 10px 20px;
    background-color: #333;
    color: #fff;
    z-index: 100;
    /* Ensure it's above other content */
}

.chat-title {
    text-align: center;
    margin: 0;
    padding: 10px;
    background-color: #333;
    color: #fff;
}

.login-button {
    padding: 10px 20px;
    background-color: #007bff;
    color: #fff;
    border: none;
    border-radius: 5px;
    cursor: pointer;
}

.input-bar-container {
    position: fixed;
    bottom: 10px;
    right: 10px;
    width: auto;
    /* Adjust width as needed */
    width: 40%;
    /* Maximum width */
    padding: 10px;
    background-color: #f3f3f3;
    border-radius: 5px;
    box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.1);
}

/* Add a class for message list container with appropriate padding */
.message-list-container {
    flex-grow: 1;
    overflow-y: auto;
    /* Scroll for overflow */
    margin-top: 10px;
    /* Space from header */
    margin-bottom: 10px;
    /* Space above input bar */
}

/* Rest of your styles */
</style>
