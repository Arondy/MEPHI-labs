import {ref, onMounted, watch} from 'vue';
import axios from 'axios';

const API_URL = 'https://jsonplaceholder.typicode.com/todos';
let task_id = 1000;

export function useTodos(){
    const todos = ref([]);

    const loadLocal = () => {
        const saved = localStorage.getItem('todos');
        if (saved) todos.value = JSON.parse(saved);
    };

    const saveLocal = () => {
        localStorage.setItem('todos', JSON.stringify(todos.value));
    };

    const fetchTodos = async () => {
        try {
            const response = await axios.get(`${API_URL}?userId=1&_limit=5`);
            todos.value = response.data;
            // saveLocal();
        } catch (err){
            console.error('Fetch error', err);
        }
    };

    const addTodo = async (title) => {
        try {
            const newTodo = {
                title,
                completed: false,
                userId: 1,
            };
            const response = await axios.post(API_URL, newTodo);

            todos.value.push({
                ...response.data,
                id: task_id++,
            });

            // saveLocal();
        } catch (err){
            console.error('Add error', err);
        }
    };

    const toggleTodo = async (todo) => {
        try {
            const response = await axios.patch(`${API_URL}/${todo.id}`, {
                completed: !todo.completed,
            });
            todo.completed = response.data.completed;
            // saveLocal();
        } catch (err){
            console.error('Toggle error', err);
        }
    };

    const deleteTodo = async (todo) => {
        try {
            await axios.delete(`${API_URL}/${todo.id}`);
            todos.value = todos.value.filter((t) => t.id !== todo.id);
            // saveLocal();
        } catch (err){
            console.error('Delete error', err);
        }
    };

    onMounted(() => {
        loadLocal();
        if (todos.value.length === 0) fetchTodos();
    });

    watch(todos, saveLocal, {deep: true});

    return {todos, addTodo, toggleTodo, deleteTodo};
}
