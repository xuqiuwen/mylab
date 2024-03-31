<template>
    <div class="message-history">
        <MessageItem v-for="message in messages" :key="message.id" :message="message" />
    </div>
</template>

<script setup>
import MessageItem from './MessageItem.vue';
import { ref, onMounted, onUnmounted } from 'vue';
import axios from 'axios';

const messages = ref([]);

let ws;

function onWebSocketMessage(event) {
    console.log(event.data);
    const newMessages = JSON.parse(event.data); // 假设服务器发送的是JSON格式的数据
    messages.value.push(newMessages);
}

function onWebSocketOpen() {
    console.log('WebSocket connected');
    // 可以发送初始化消息或请求
}

function onWebSocketClose() {
    console.log('WebSocket disconnected');
    // 可以在这里尝试重连
}
// 从后端获取消息列表的函数
async function fetchMessages() {
    try {
        const response = await axios.get('/messages'); // Make sure this URL is correct
        messages.value = response.data;
        console.log(response.data);
        console.log(messages.value);
    } catch (error) {
        // Handle network errors
        console.error('Network error:', error);
    }
}

onMounted(() => {
    fetchMessages();//初始化数据
    ws = new WebSocket('ws://localhost:8080/api/v1/ws'); // 根据你的服务器地址修改
    ws.onmessage = onWebSocketMessage;
    ws.onopen = onWebSocketOpen;
    ws.onclose = onWebSocketClose;
});

onUnmounted(() => {
    if (ws) {
        ws.close();
    }
});
</script>
