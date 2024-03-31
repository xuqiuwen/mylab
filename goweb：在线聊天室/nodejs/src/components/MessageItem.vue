<template>
    <div class="message-item">
        <div class="message-author">{{ message.Sender }}</div>
        <div class="message-content">
            <div class="message-time">{{ message.CreatedAt }}</div>
            <div class="message-text">{{ message.Content }}</div>
        </div>
    </div>
</template>
  
<script setup>
import { defineProps } from 'vue';

const props = defineProps({
    message: Object
});
</script>
  
<style scoped>
.message-item {
    display: flex;
    align-items: flex-start;
    /* 对齐到顶部 */
    margin-bottom: 10px;
    /* 或根据你的设计调整 */
    padding: 10px;
}

.message-author {
    min-width: 50px;
    /* 根据你的设计调整姓名的宽度 */
    font-weight: bold;
    color: rgb(22, 171, 171);
    margin-right: 10px;
    /* 与时间和消息文本之间的间距 */
    align-self: center;
    /* 垂直居中 */
}

.message-content {
    flex: 1;
    display: flex;
    flex-direction: column;
}

.message-time {
    font-size: 0.75em;
    color: #555;
    /* 调整时间的字体颜色 */
    margin-bottom: 5px;
    /* 时间与消息文本之间的间距 */
}

.message-text {
    background-color: #EFEFEF;
    border-radius: 10px;
    padding: 5px 10px;
    display: inline-block;
    max-width: fit-content;
    word-wrap: break-word;
}
</style>
