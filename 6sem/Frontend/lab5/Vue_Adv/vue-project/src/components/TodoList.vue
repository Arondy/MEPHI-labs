<script setup>
import {ref} from 'vue';
import {useTodos} from '../composables/useTodos';
import Popup from './Popup.vue';

const {todos, addTodo, toggleTodo, deleteTodo} = useTodos();
const newTitle = ref('');
const loadingIds = ref(new Set());
const popupVisible = ref(false);
const selectedTodo = ref(null);

const handleAdd = async () => {
  const title = newTitle.value.trim();
  if (!title) return;
  await addTodo(title);
  newTitle.value = '';
};

const handleToggle = async (todo) => {
  loadingIds.value.add(todo.id);
  await toggleTodo(todo);
  loadingIds.value.delete(todo.id);
};

const confirmDelete = (todo) => {
  selectedTodo.value = todo;
  popupVisible.value = true;
};

const cancelDelete = () => {
  popupVisible.value = false;
  selectedTodo.value = null;
};

const proceedDelete = async () => {
  if (selectedTodo.value){
    await deleteTodo(selectedTodo.value);
  }
  cancelDelete();
};
</script>

<template>
  <div>
    <div class="input-group">
      <input
          v-model="newTitle"
          @keyup.enter="handleAdd"
          placeholder="Добавить новое задание"
      />
      <button @click="handleAdd">Добавить</button>
    </div>

    <ul>
      <li v-for="todo in todos" :key="todo.id">
        <label :class="{ completed: todo.completed }">
          <input
              type="checkbox"
              :checked="todo.completed"
              :disabled="loadingIds.has(todo.id)"
              @click.prevent="handleToggle(todo)"/>
          <!-- .prevent чтобы галка не выставлялась сразу, а ждала API вместе с перечеркиванием текста-->
          {{ todo.title }}
        </label>
        <button @click="confirmDelete(todo)">Удалить</button>
      </li>
    </ul>

    <Popup :visible="popupVisible" @confirm="proceedDelete" @close="cancelDelete">
      <p>Вы уверены, что хотите удалить задачу?</p>
    </Popup>
  </div>
</template>

<style scoped>
ul {
  list-style: none;
  margin: 1rem 0;
  padding: 0;
  border: 1px solid #ddd;
  border-radius: 8px;
  overflow: hidden;
}

li {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 0.75rem 1rem;
  background: #fafafa;
  border-bottom: 1px solid #eee;
}

li:hover {
  background: #f0f0f0;
}

/* чекбокс */
label {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  cursor: pointer;
}

input[type="checkbox"] {
  appearance: none;
  width: 18px;
  height: 18px;
  border: 2px solid #42b983;
  border-radius: 4px;
  position: relative;
  cursor: pointer;
  transition: background 0.2s ease;
}

input[type="checkbox"]:checked {
  background-color: #42b983;
  border-color: #42b983;
}

input[type="checkbox"]:checked::after {
  content: '✓';
  color: white;
  position: absolute;
  left: 2px;
  top: -3px;
  font-size: 14px;
}

.completed {
  text-decoration: line-through;
  color: #999;
}

.input-group {
  display: flex;
  margin-bottom: 1rem;
}

.input-group input {
  flex: 1;
  padding: .5rem;
  border: 1px solid #ccc;
  border-radius: 4px;
}

.input-group button {
  margin-left: .5rem;
  padding: .5rem 1rem;
  border: none;
  border-radius: 4px;
  background: #42b983;
  color: white;
  cursor: pointer;
}

.input-group button:hover {
  background: #369b6e;
}

/* СТИЛИ ДЛЯ КНОПКИ УДАЛЕНИЯ */
li button {
  background: #e74c3c;
  border: none;
  padding: 0.4rem 0.8rem;
  border-radius: 6px;
  color: white;
  font-size: 14px;
  cursor: pointer;
  transition: background 0.2s;
}

li button:hover {
  background: #c0392b;
}
</style>
