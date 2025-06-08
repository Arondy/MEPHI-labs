document.onselectstart = function (){
    return false;
}

document.addEventListener("contextmenu", (event) => {
    event.preventDefault();
});

const counterElement = document.getElementById('counter');
const avgClicksElement = document.getElementById('avgClicks');
const button = document.getElementById('clickButton');

let counter = parseInt(localStorage.getItem('counter')) || 0;
let startTime = localStorage.getItem('startTime') ? parseInt(localStorage.getItem('startTime')) : null;

function updateCounter(){
    counterElement.textContent = `Счетчик: ${counter}`;
}

function updateAvgClicks(){
    if (counter === 0 || !startTime){
        avgClicksElement.textContent = 'Средняя скорость: 0 кликов/сек';
        return;
    }

    const currentTime = Date.now();
    const elapsedTimeInSeconds = (currentTime - startTime) / 1000;
    const avgClicksPerSecond = (counter / elapsedTimeInSeconds).toFixed(2);
    avgClicksElement.textContent = `Средняя скорость: ${avgClicksPerSecond} кликов/сек`;
}

function incrementCounter(){
    if (!startTime){
        startTime = Date.now();
        localStorage.setItem('startTime', startTime);
    }

    counter++;
    localStorage.setItem('counter', counter);
    updateCounter();
    updateAvgClicks();
}

button.addEventListener('click', incrementCounter);
updateCounter();
updateAvgClicks();

const resetButton = document.getElementById('resetButton');
resetButton.addEventListener('click', () => {
    counter = 0;
    startTime = null;
    localStorage.removeItem('counter');
    localStorage.removeItem('startTime');
    updateCounter();
    updateAvgClicks();
});
