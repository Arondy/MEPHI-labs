<script setup>
import {defineProps, defineEmits} from 'vue';

const props = defineProps({
  visible: Boolean,
});

const emit = defineEmits(['close', 'confirm']);

const close = () => emit('close');
const confirm = () => emit('confirm');
</script>

<template>
  <Teleport to="body">
    <div v-if="visible" class="popup-overlay" @click.self="close">
      <div class="popup-content">
        <slot/>
        <div class="popup-actions">
          <button @click="confirm">Да</button>
          <button @click="close">Отмена</button>
        </div>
      </div>
    </div>
  </Teleport>
</template>

<style scoped>
.popup-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(0, 0, 0, 0.5);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 1000;
}

.popup-content {
  background: white;
  padding: 1.5rem;
  border-radius: 10px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15);
  max-width: 90%;
  width: 320px;
  text-align: center;
  animation: popup-appear .2s ease-out;
}

.popup-actions {
  margin-top: 1.5rem;
  display: flex;
  justify-content: space-around;
}

.popup-actions button {
  padding: .5rem 1.2rem;
  border: none;
  border-radius: 6px;
  cursor: pointer;
  color: white;
  font-weight: bold;
  font-size: 14px;
}

.popup-actions button:first-child {
  background: #2ecc71; /* зелёная */
}

.popup-actions button:first-child:hover {
  background: #27ae60;
}

.popup-actions button:last-child {
  background: #e74c3c; /* красная */
}

.popup-actions button:last-child:hover {
  background: #c0392b;
}

@keyframes popup-appear {
  from {
    transform: scale(0.9);
    opacity: 0;
  }
  to {
    transform: scale(1);
    opacity: 1;
  }
}
</style>
