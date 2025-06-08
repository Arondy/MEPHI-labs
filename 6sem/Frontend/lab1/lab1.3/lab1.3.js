function isBadInput(input){
    return input.length === 0;
}

function sortNumbers(){
    const input = prompt("Введите список натуральных чисел через запятую:");
    if (isBadInput(input)){
        alert("Неверный ввод!");
        return;
    }
    const numbers = input.split(",").map(Number);
    const sortedNumbers = numbers.sort((a, b) => a - b);
    alert(`Отсортированный список: ${sortedNumbers.join(", ")}`);
}

function f(num){
    return num % 5;
}

function getRemaindersDiv5(){
    const input = prompt("Введите массив натуральных чисел через запятую:");
    if (isBadInput(input)){
        alert("Неверный ввод!");
        return;
    }
    const numbers = input.split(",").map(Number);
    const remainders = numbers.map(function (num){
        return num % 5;
    });
    alert(`Остатки от деления на 5: ${remainders.join(", ")}`);
}

function getMedian(...args){
    const numbers = args.sort((a, b) => a - b);
    const mid = Math.floor(numbers.length / 2);

    if (numbers.length % 2 === 0){
        return (numbers[mid - 1] + numbers[mid]) / 2;
    } else {
        return numbers[mid];
    }
}

function calculateMedian(){
    const input = prompt("Введите числа через запятую:");
    if (isBadInput(input)){
        alert("Неверный ввод!");
        return;
    }
    const numbers = input.split(",").map(Number);

    // Вызов с распаковкой массива
    const result1 = getMedian(...numbers);
    alert(`Медиана: ${result1}`);

    // Вызов классическим методом
    const result2 = getMedian(1, 3, 2, 5, 4);
}

function checkBracketsString(){
    const input = prompt("Введите строку со скобками:");
    if (isBadInput(input)){
        alert("Неверный ввод!");
        return;
    }
    const stack = [];
    let isValid = true;

    for (const char of input){
        if (char === "("){
            stack.push(char);
        } else if (char === ")"){
            if (stack.length === 0){
                isValid = false;
                break;
            }
            stack.pop();
        } else {
            alert("Неверный ввод - не только скобки!");
            return
        }
    }

    if (stack.length !== 0){
        isValid = false;
    }

    alert(isValid ? "Правильная" : "Неправильная");
}

function deepCopy(obj){
    // примитив
    if (obj === null || typeof obj !== "object"){
        return obj;
    }

    // массив
    if (Array.isArray(obj)){
        return obj.map(item => deepCopy(item));
    }

    // что-то сложное (объект?)
    const copy = {};
    for (const key in obj){
        if (obj.hasOwnProperty(key)){
            copy[key] = deepCopy(obj[key]);
        }
    }
    return copy;
}

function deepCopyObjectTest(){
    const original = {
        name: "Test",
        age: 25,
        extra: {
            value: 42,
            array: [1, 2, {inner: "deep"}]
        }
    };

    const copied = deepCopy(original);
    original.extra.value = 99;

    console.log("Оригинал:", original);
    console.log("Копия:", copied);
    alert("Глубокая копия создана. Проверьте консоль.");
}