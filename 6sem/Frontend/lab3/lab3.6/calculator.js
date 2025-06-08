const number1 = document.getElementById('number1');
const number2 = document.getElementById('number2');
const buttons = document.querySelectorAll('.button');

let currentInput = '';
let operator = '';
let isNumber2 = false;
let activeOperatorButton = null;

function updateDisplay(value){
    if (!isNumber2){
        number1.value = value;
    } else {
        number2.value = value;
    }
}

function concatWithCurrentInput(value){
    if (!isNumber2){
        currentInput = number1.value + value;
        number1.value = currentInput;
    } else {
        currentInput = number2.value + value;
        number2.value = currentInput;
    }
}

function resetActiveOperatorButton(){
    if (activeOperatorButton){
        activeOperatorButton.classList.remove('active');
    }
}

function shiftToSecondNumber(button, value){
    resetActiveOperatorButton();
    activeOperatorButton = button;
    activeOperatorButton.classList.add('active');
    operator = value;
    isNumber2 = true;
    currentInput = '';
}

function calculateOperation(a, b, operator){
    switch (operator){
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b === 0){
                alert("Ошибка: Деление на ноль!");
                return null;
            }
            return a / b;
        default:
            console.warn(`Неизвестный оператор: ${operator}`)
            return null;
    }
}

function calculateResult(){
    const result = calculateOperation(
        parseFloat(number1.value),
        parseFloat(number2.value),
        operator
    );

    if (result !== null){
        updateDisplay(result);
        number1.value = result.toString();
    }

    resetActiveOperatorButton();
    number2.value = '';
    operator = '';
    isNumber2 = false;
}

function resetCalculator(){
    currentInput = '';
    operator = '';
    isNumber2 = false;
    number1.value = '';
    number2.value = '';
    resetActiveOperatorButton();
}

buttons.forEach(button => {
    button.addEventListener('click', () => {
        const value = button.getAttribute('data-value');

        if (value === 'C'){
            resetCalculator();
        } else if (['+', '-', '*', '/'].includes(value)){
            if (number1.value === ''){
                return;
            }

            shiftToSecondNumber(button, value)
        } else if (value === '='){
            if (number1.value === '' || number2.value === '' || operator === ''){
                return;
            }

            calculateResult();
        } else {
            concatWithCurrentInput(value);
        }
    });
});
