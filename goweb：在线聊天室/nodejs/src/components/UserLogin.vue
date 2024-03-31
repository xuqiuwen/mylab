<template>
    <div class="modal-overlay">
        <div class="modal">
            <div class="modal-header">
                <h3>登录</h3>
            </div>
            <div class="modal-body">
                <div class="input-group">
                    <label for="loginForm.nickname">昵称：</label>
                    <input type="text" id="loginForm.nickname" v-model="loginForm.nickname" placeholder="请输入昵称" />
                </div>
                <div class="input-group">
                    <label for="loginForm.gender">性别：</label>
                    <select id="loginForm.gender" v-model="loginForm.gender">
                        <option value="">请选择</option>
                        <option value="male">male</option>
                        <option value="female">female</option>
                        <option value="other">other</option>
                    </select>
                </div>
            </div>
            <div class="modal-footer">
                <button @click="handleLogin">登录</button>
            </div>
        </div>
    </div>
</template>


  
<script setup>
import { ref } from 'vue';
import axios from 'axios';
import { setUser } from '../composables/useUser.js'

const loginForm = ref({
    nickname: '',
    gender: ''
});


const emit = defineEmits(['login-success'])

async function handleLogin() {
    try {
        const response = await axios.post('/login', loginForm.value);
        // 登录成功处理，例如设置用户数据
        //console.log(loginForm);

        setUser(response.data)//设置登录用户
        //console.log(user.value);
        //console.log(response.data);
        emit('login-success');

    } catch (error) {
        // 登录失败处理
        console.error(error);
    }
};
</script>
  
<style scoped>
.modal-overlay {
    display: flex;
    justify-content: center;
    align-items: center;
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background-color: rgba(0, 0, 0, 0.6);
}

.modal {
    background-color: #fff;
    padding: 25px;
    border-radius: 15px;
    box-shadow: 0 6px 20px rgba(0, 0, 0, 0.15);
    width: 350px;
}

.modal-header h3 {
    margin: 0 0 20px;
    color: #333;
    font-size: 24px;
}

.input-group {
    margin-bottom: 15px;
}

.input-group label {
    display: block;
    margin-bottom: 5px;
    color: #666;
    font-size: 14px;
}

.input-group input,
.input-group select {
    width: 85%;
    padding: 12px 15px;
    border: 1px solid #ddd;
    border-radius: 8px;
    font-size: 14px;
}

.modal-footer {
    margin-top: 20px;
}

.modal button {
    width: 100%;
    padding: 12px 15px;
    border: none;
    border-radius: 8px;
    background-color: #3498db;
    color: white;
    font-size: 16px;
    cursor: pointer;
    transition: background-color 0.2s;
}

.modal button:hover {
    background-color: #2980b9;
}
</style>
