function isBadInput(input){
    return input.length === 0;
}

function getStringsFromPrompt(stringsNumber = 5, promptMessage = `⏳ Введите ${stringsNumber} строк, разделенные пробелом`){
    const input = prompt(promptMessage);

    if (isBadInput(input)){
        return;
    }

    const strings = input.split(' ');

    if (strings.length !== stringsNumber){
        const errorMessage = `Неверное количество строк: ${strings.length}`;
        alert(errorMessage);
        throw new Error(errorMessage);
    }

    return strings;
}

// 1
function getUserRestartsCount(){
    let userRestartsCount = localStorage.getItem('userRestartsCount');
    if (userRestartsCount){
        userRestartsCount = Number(userRestartsCount);
    } else {
        localStorage.setItem('userRestartsCount', '0');
        userRestartsCount = 0;
    }
    return userRestartsCount;
}

function updateRestartsCount(){
    const count = getUserRestartsCount();
    document.getElementById('restarts-count').textContent = count;
}

updateRestartsCount();

window.addEventListener('beforeunload', function (event){
    let userRestartsCount = getUserRestartsCount();
    userRestartsCount++;
    localStorage.setItem('userRestartsCount', userRestartsCount.toString());
});

// 2
function clearImages(){
    const imageContainer = document.getElementById('image-container');
    imageContainer.replaceChildren();
    console.log('Очистка страницы от изображений завершена!');
}

const urlsNumber = 5;
const urlsPrompt = `⏳ Введите ${urlsNumber} URL изображений, разделенные пробелом`;

async function loadImageFromUrl(imageUrl, imageContainer){
    const response = await fetch(imageUrl);

    if (!response.ok){
        console.warn(`Ошибка загрузки изображения: ${response.status}`);
        const brokenImageText = document.createElement('p');
        brokenImageText.textContent = 'Can’t load image';
        imageContainer.appendChild(brokenImageText);
        return;
    }

    const blob = await response.blob();
    const objectUrl = URL.createObjectURL(blob);
    const img = document.createElement('img');
    img.src = objectUrl;

    imageContainer.appendChild(img);
}

async function loadImages(){
    const imageUrls = getStringsFromPrompt(urlsNumber, urlsPrompt);
    const imageContainer = document.getElementById('image-container');

    for (const imgUrl of imageUrls){
        await loadImageFromUrl(imgUrl, imageContainer);
    }

    console.log('Загрузка изображений завершена!');
}

// 3
async function loadImagesAsync(){
    const imageUrls = getStringsFromPrompt(urlsNumber, urlsPrompt);
    const imageContainer = document.getElementById('image-container');
    const tasks = [];

    for (const imgUrl of imageUrls){
        tasks.push(loadImageFromUrl(imgUrl, imageContainer));
    }

    for (const task of tasks){
        await task;
    }

    console.log('Загрузка изображений завершена!');
}

// 4
function loadImageFromUrlByPromises(imageUrl, imageContainer){
    return fetch(imageUrl)
        .then(response => {
            if (!response.ok){
                throw new Error(`Ошибка загрузки изображения: ${response.status}`);
            }
            return response.blob();
        })
        .then(blob => {
            const objectUrl = URL.createObjectURL(blob);
            const img = document.createElement('img');
            img.src = objectUrl;
            imageContainer.appendChild(img);

            return Promise.resolve(img);
        })
        .catch(error => {
            console.warn(error.message);
            const brokenImageText = document.createElement('p');
            brokenImageText.textContent = 'Can’t load image';
            imageContainer.appendChild(brokenImageText);

            return Promise.resolve(brokenImageText);
        });
}

function loadImagesByPromises(){
    const imageUrls = getStringsFromPrompt(urlsNumber, urlsPrompt);
    const imageContainer = document.getElementById('image-container');
    let promise = Promise.resolve(); // пустой промис

    for (const imgUrl of imageUrls){
        promise = promise.then(() => {
            return loadImageFromUrlByPromises(imgUrl, imageContainer);
        });
    }

    promise.then(() => {
        console.log('Загрузка изображений завершена!');
    }).catch(error => {
        console.error('Произошла ошибка при загрузке изображений:', error);
    });

    console.log('Загрузка изображений завершена!');
}

function loadImagesAsyncByPromises(){
    const imageUrls = getStringsFromPrompt(urlsNumber, urlsPrompt);
    const imageContainer = document.getElementById('image-container');
    const promises = [];

    for (const imgUrl of imageUrls){
        promises.push(new Promise((resolve, reject) => {
            loadImageFromUrlByPromises(imgUrl, imageContainer)
                .then(() => resolve())
                .catch(error => reject(error));
        }));
    }

    Promise.allSettled(promises)
        .then(() => {
            console.log('Загрузка изображений завершена!');
        })
        .catch(error => {
            console.error('Произошла ошибка при загрузке изображений:', error);
        });

    console.log('Загрузка изображений завершена!');
}

// 5
function checkIpAddressString(ip){
    const ipv4Pattern = /^(\d{1,3}\.){3}\d{1,3}$/;
    return ipv4Pattern.test(ip);
}

async function checkIps(){
    const ipsNumber = 5;
    const ips = getStringsFromPrompt(ipsNumber, `⏳ Введите ${ipsNumber} IP, разделенные пробелом`);

    if (ips.some((ip) => !checkIpAddressString(ip))){
        const errorMessage = 'Неверные IP!';
        alert(errorMessage);
        throw new Error(errorMessage);
    }

    const geoApi = 'https://json.geoiplookup.io/';
    const bannedCountriesCodes = ['RU', 'BY', 'AF', 'CN', 'VE', 'IR']
    const tasks = [];

    for (const ip of ips){
        tasks.push(fetch(`${geoApi}${ip}`));
    }

    for (const task of tasks){
        const result = await task;
        const json = await result.json();

        if (bannedCountriesCodes.includes(json['country_code'])){
            alert('Our services are not available in your country');
            console.log('BANNED: ', json['ip'], json['country_code']);
            return;
        } else {
            console.log(json['ip'], json['country_code']);
        }
    }

    alert('Welcome to our website!')
}