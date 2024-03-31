<template>
    <div class="user-item">
        <div class="user-name">{{ user.Nickname }}</div>
        <div class="user-gender">
            <!-- 条件渲染性别图标 -->
            <span v-if="user.Gender === 'male'" class="gender-icon male"></span>
            <span v-else-if="user.Gender === 'female'" class="gender-icon female"></span>
            <!-- 如果有必要，可以增加更多的性别条件或者图标 -->
        </div>
    </div>
</template>

<script setup>
import { defineProps } from 'vue';

const props = defineProps({
    user: Object
});
</script>

<style scoped>
.user-item {
    display: flex;
    align-items: center;
    padding: 10px 15px;
    border-bottom: 1px solid #ddd;
    /* 添加一个底部边框 */
    transition: background-color 0.3s;
    /* 添加一个简单的鼠标悬停效果 */
}

.user-item:hover {
    background-color: #e8e8e8;
}

.user-name {
    font-size: 1.1em;
    /* 增加字体大小 */
    font-weight: 500;
    /* 字体加粗 */
    flex-grow: 1;
    margin-right: 15px;
    /* 调整间隔 */
    color: #333;
    /* 字体颜色 */
}

.user-gender {
    min-width: 20px;
    /* 调整性别图标的最小宽度 */
    min-height: 20px;
    /* 调整性别图标的最小高度 */
    display: flex;
    /* 使用 flex 来居中图标 */
    align-items: center;
    /* 垂直居中 */
    justify-content: center;
    /* 水平居中 */
}

.gender-icon {
    display: inline-block;
    /* 改为 inline-block 以允许设置宽度和高度 */
    border-radius: 50%;
    /* 圆形图标 */
    width: 20px;
    /* 调整性别图标的宽度 */
    height: 20px;
    /* 调整性别图标的高度 */
}

.male {
    background-color: #0000FF;
    /* 男性的颜色，通常为蓝色 */
}

.female {
    background-color: #FFC0CB;
    /* 女性的颜色，通常为粉色 */
}
</style>
