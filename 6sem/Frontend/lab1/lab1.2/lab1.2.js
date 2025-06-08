function isBadInput(input){
    return input.length === 0;
}

function getMonthName(){
    const input = prompt("Введите номер месяца (от 1 до 12):");
    if (isBadInput(input)){
        alert("Неверный ввод!");
        return;
    }
    const monthNumber = parseInt(input, 10);
    const monthNumberFloat = parseFloat(input);
    if (monthNumber !== monthNumberFloat){
        alert("Неверный ввод!");
        return;
    }
    const months = [
        "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"
    ];

    if (!isNaN(monthNumber) && monthNumber >= 1 && monthNumber <= 12){
        alert(months[monthNumber - 1]);
    } else {
        alert("Некорректный номер месяца");
    }
}

function isPrime(num){
    if (num < 2){
        return false;
    }

    for (let i = 2; i <= Math.sqrt(num); i++){
        if (num % i === 0) return false;
    }

    return true;
}

function getNPrimeNumbers(){
    const input = prompt("Введите натуральное число n:");
    if (isBadInput(input)){
        alert("Неверный ввод!");
        return;
    }
    const n = parseInt(input, 10);

    if (isNaN(n)){
        alert("Неверный ввод!");
        return;
    }

    if (n <= 0){
        alert("Число должно быть натуральным!");
        return;
    }

    const primes = [];
    let num = 2;

    while (primes.length < n){
        if (isPrime(num)){
            primes.push(num);
        }
        num++;
    }
    alert(primes.join(" "));
}

// Для з.4
const Counter = {
    count: 0,
    add(value){
        this.count += value;
    },
    sub(value){
        this.count -= value;
    }
};

function replaceCommasWithDots(){
    const input = prompt("Введите список слов, разделенных запятыми:");
    if (isBadInput(input)){
        alert("Неверный ввод!");
        return;
    }
    const result = input.replaceAll(",", ".");
    alert(result);
}

function isPalindrome(){
    const input = prompt("Введите строку:");
    if (isBadInput(input)){
        alert("Неверный ввод!");
        return;
    }
    const cleanedInput = input.toLowerCase().replace(/[^a-zа-яё]/g, "");
    const reversedInput = cleanedInput.split("").reverse().join("");

    if (cleanedInput === reversedInput){
        alert("Да");
    } else {
        alert("Нет");
    }
}