import { createApp } from 'vue'
import App from './App.vue'
import axios from 'axios'

axios.defaults.withCredentials = true;
axios.defaults.baseURL = 'http://localhost:8080/api/v1';
createApp(App).mount('#app');
