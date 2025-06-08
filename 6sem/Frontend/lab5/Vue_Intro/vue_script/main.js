const {createApp, ref} = Vue;

const app = createApp({
    setup(){
        const count = ref(0);

        const onClick = () => {
            count.value++;
        };

        return {
            count,
            onClick,
        };
    },
});

app.mount('#app');