// 1
class User {
    constructor(name, age){
        this.name = name;
        this.age = age;
    }

    hello(){
        console.log(`Hi! My name is ${this.name}. And I am ${this.age} years old.`);
    }
}

// 2
// .call(this, ...) = super() для наследования
function User2(name, age){
    this.name = name;
    this.age = age;
}

User2.prototype.hello = function (){
    console.log(`Hi! My name is ${this.name}. And I am ${this.age} years old.`);
}

// 3, 4
class User3 {
    #age;
    _tel;

    constructor(name){
        this.name = name;
    }

    get tel(){
        return this._tel;
    }

    set tel(value){
        const phoneRegex = /^\+7\d{10}$/;

        if (phoneRegex.test(value)){
            this._tel = value;
        } else {
            throw new Error("Некорректный формат телефона!");
        }
    }

    get age(){
        return this.#age;
    }

    set age(value){
        if (Number.isInteger(value) && 1 <= value <= 100){
            this.#age = value;
        } else {
            throw new Error("Некорректный возраст!")
        }
    }

    hello(){
        console.log(`Hi! My name is ${this.name}. And I am ${this.#age} years old.`);
    }
}

// 5
class Student extends User {
    #knowledge = 0;

    constructor(name){
        super(name);
    }

    hello(){
        console.log(
            `Hi! My name is ${this.name}. I am ${this.age} years old. And I am a student!`
        );
    }

    learn(){
        this.#knowledge += 1;
        console.log(`Knowledge: ${this.#knowledge}`);
    }
}

// 6
Array.prototype.reverse = function (){
    return this.concat(this.slice());
}
