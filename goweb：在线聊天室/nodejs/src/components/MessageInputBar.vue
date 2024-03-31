<template>
    <div class="message-input-bar">
        <input class="message-input" type="text" v-model="messageForm.message" @keyup.enter="sendMessage"
            placeholder="输入消息..." />
        <button class="send-button" @click="sendMessage">Send</button>
    </div>
</template>

<script setup>
import { ref, defineEmits } from 'vue';
import { user } from '/src/composables/useUser.js';
import axios from 'axios';

const emit = defineEmits(['send-success']);

const messageForm = ref({
    senderid: 0,
    sender: '',
    message: '',
});

async function sendMessage() {
    try {
        messageForm.value.senderid = user.value.userid
        messageForm.value.sender = user.value.nickname
        const response = await axios.post('/send', messageForm.value);

        messageForm.value.message = ''
        emit('send-success');

    } catch (error) {
        console.error(error);
    }
}

</script>

<style scoped>
.message-input-bar {
    display: flex;
    justify-content: space-between;
    padding: 10px;
    background-color: #f4f4f4;
}

.message-input {
    flex-grow: 1;
    margin-right: 10px;
    padding: 10px;
    border: 1px solid #ddd;
    border-radius: 5px;
    font-size: 1em;
}

.send-button {
    padding: 10px 20px;
    background-color: #5cb85c;
    border: none;
    border-radius: 5px;
    color: white;
    font-size: 1em;
    cursor: pointer;
    transition: background-color 0.3s;
}

.send-button:hover {
    background-color: #4cae4c;
}

/* 适应不同设备尺寸 */
@media (max-width: 768px) {
    .send-button {
        padding: 10px;
    }
}
</style>
