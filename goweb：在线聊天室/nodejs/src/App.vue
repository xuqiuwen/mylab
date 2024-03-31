<template>
  <div class="app-container">
    <div class="user-list-container">
      <!-- 用户列表组件 -->
      <UserList />
    </div>
    <div class="hmessage-chathall-container">
      <!-- 消息历史组件 -->
      <ChatHall />
    </div>
  </div>
</template>

<script setup>
// 引入UserList和MessageHistory组件
import UserList from './components/UserList.vue';
import ChatHall from './components/ChatHall.vue';
</script>

<style>
.app-container {
  display: flex;
  height: 98vh;
  /* 使应用占据全部视口高度 */
}

.user-list-container {
  flex: 1;
  /* 用户列表占1份 */
  width: 20%;
  /* 占总宽度的1/5 */
  overflow-y: auto;
  /* 如果内容过多，出现滚动条 */
  /* 根据需要添加边框、间距、背景等样式 */
}

.hmessage-chathall-container {
  flex: 4;
  /* 历史消息占4份 */
  width: 80%;
  /* 占总宽度的4/5 */
  overflow-y: auto;
  /* 如果内容过多，出现滚动条 */
  /* 根据需要添加边框、间距、背景等样式 */
}
</style>
