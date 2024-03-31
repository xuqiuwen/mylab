<template>
    <div class="user-list-container">
        <h2 class="user-list-title">用户列表</h2>
        <div class="user-list">
            <UserItem v-for="user in users" :key="user.Id" :user="user" />
        </div>
    </div>
</template>
  

  
<script setup>
import UserItem from './UserItem.vue';
import { ref, onMounted, onUnmounted } from 'vue';
import axios from 'axios';
const users = ref([]); // 初始化为空数组

// 从后端获取用户列表的函数
async function fetchUsers() {
    try {
        const response = await axios.get('/users'); // Make sure this URL is correct
        users.value = response.data;

    } catch (error) {
        // Handle network errors
        console.error('Network error:', error);
    }
}

// 轮询逻辑
const pollInterval = 500; // 0.5秒轮询一次
let intervalId;

onMounted(() => {
    fetchUsers(); // 首次挂载组件时获取用户
    intervalId = setInterval(fetchUsers, pollInterval); // 设置轮询
});

onUnmounted(() => {
    clearInterval(intervalId); // 清理轮询
});
</script>

<style scoped>
.user-list-container {
    width: 100%;
    height: 100vh;
    overflow-y: auto;
    background-color: #f5f5f5;
    box-shadow: 2px 0 5px rgba(0, 0, 0, 0.1);
}

.user-list-title {
    margin: 0;
    padding: 20px;
    background-color: #333;
    color: #fff;
    text-align: center;
}

.user-list {
    display: flex;
    flex-direction: column;
}
</style>
  