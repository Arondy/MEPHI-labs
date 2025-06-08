import axios from "axios";

const url = "https://vk.com";
// const url = "https://json.geoiplookup.io/46.138.52.227";

axios.get(url)
    .then(response => {
        console.log('Успешный ответ в Node.js!');
        // console.log(response.data);

        if (typeof window !== 'undefined'){
            document.querySelector('#line').innerHTML = 'Успешный ответ!';
        }
    })
    .catch(error => {
        console.error('Ошибка в Node.js:', error.message);
    });
