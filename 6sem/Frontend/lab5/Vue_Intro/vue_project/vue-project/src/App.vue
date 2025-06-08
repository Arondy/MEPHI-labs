<script setup>
import {ref} from 'vue'
import Clicker from './components/Clicker.vue';

const handleClick = (index) => {
  alert(`Clicker ${index} clicked!`);
};
const array = ref(["a", "b", "c", 'd', 'e']);

function shift(){
  debugger
  const firstElem = array.value[4]
  let array2 = array.value.slice(0, array.value.length-1);
  array2.unshift(firstElem);
  array.value = array2;
}
</script>

<template>
  <div>
    <div v-for="item in array" :key="item">
      <p>{{ item }}</p>
      <input type="text">
    </div>
    <button @click="() => shift()">button</button>
  </div>
  <div id="app">
    <Clicker
        v-for="index in 100"
        :key="index"
        :amount="1"
        @button-clicked="() => handleClick(index)">
      <button class="custom-btn">
        Прибавить {{ amount }}
      </button>
    </Clicker>
  </div>
</template>

<style>
body {
  background-color: rgb(54, 54, 54);
}

#app {
  font-family: Arial, sans-serif;
  margin: 0;
  padding: 20px 40px;
  display: grid;
  grid-template-columns: repeat(8, 1fr);
  gap: 20px;
}

.custom-btn {
  padding: 12px 24px;
  font-size: 16px;
  color: white;
  background: #42b883;
  border: none;
  border-radius: 8px;
  cursor: pointer;
  transition: all 0.2s ease;
}

.custom-btn:hover {
  background: #379f74;
}

.custom-btn:active {
  transform: scale(0.98);
  box-shadow: inset 0 2px 4px rgba(0, 0, 0, 0.2);
}
</style>