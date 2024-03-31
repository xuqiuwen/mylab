// useUser.js
import { ref } from 'vue';

export const user = ref({
    userid: 0,
    nickname: '',
    gender: ''
})
export const isLogged = ref(false);

export function setUser(userData) {
    user.value.userid = userData.userid;
    user.value.nickname = userData.nickname;
    user.value.message = userData.message;
    isLogged.value = true;
}
