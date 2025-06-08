<script setup>
import {ref, defineProps, defineEmits} from 'vue';

const props = defineProps({
  amount: {
    type: Number,
    required: true,
    default: 1
  }
});

const emit = defineEmits(['button-clicked']);

const count = ref(0);

const increment = () => {
  count.value += props.amount;
  emit('button-clicked');
};
</script>

<template>
  <div class="counter">
    <div class="count-display">
      Счётчик: {{ count }} <br>
      <small>Прибавляет: {{ amount }}</small>
    </div>
    <div class="slot-container" @click="increment">
      <slot />
    </div>
  </div>
</template>

<style scoped>
.counter {
  border: 1px solid #e0e0e0;
  border-radius: 12px;
  padding: 20px;
  text-align: center;
  box-shadow: 0 2px 8px rgba(255, 255, 255, 0.1);
}

.count-display {
  font-size: 20px;
  margin-bottom: 15px;
  color: #b0cae8;
}

</style>