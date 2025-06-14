# Лабораторные работы по программированию на C (2 семестр)

В этом репозитории собраны лабораторные работы, выполненные в течение второго семестра по программированию на языке C.

## Структура репозитория

- `lab1/` - Лабораторная работа 1: Работа с матрицами
- `lab2/` - Лабораторная работа 2: Списки и очереди
- `lab3a/` - Лабораторная работа 3a: Хеш-таблицы (открытая адресация)
- `lab3b/` - Лабораторная работа 3b: Хеш-таблицы (метод цепочек)
- `lab3c/` - Лабораторная работа 3c: Деревья поиска
- `lab4a/` - Лабораторная работа 4a: АВЛ-деревья
- `lab4b/` - Лабораторная работа 4b: Красно-черные деревья
- `lab5/` - Лабораторная работа 5: Графы
- `ekz/` - Подготовка к экзамену

## Подробное описание лабораторных работ

### Лабораторная работа 1: Работа с матрицами
- **Файлы:**
  - `main.c` - основная программа
  - `matrix.c`, `matrix.h` - работа с матрицами
- **Описание:** Программа для работы с рваными матрицами (матрицами с разной длиной строк)
- **Функции:**
  - Ввод и вывод матрицы
  - Поиск строки с максимальным количеством повторяющихся элементов
  - Обработка матрицы по заданному алгоритму

### Лабораторная работа 2: Списки и очереди
- **Файлы:**
  - `main.c` - основная программа
  - `list.c` - реализация списка
  - `vector.c` - реализация вектора
  - `queue.h` - интерфейс очереди
  - `input.c`, `input.h` - ввод данных
- **Описание:** Сравнительный анализ эффективности операций над разными структурами данных
- **Структуры данных:**
  - Односвязный список
  - Вектор (динамический массив)
  - Очередь

### Лабораторная работа 3: Хеш-таблицы и деревья поиска

#### Вариант 3a: Хеш-таблица с открытой адресацией
- **Файлы:**
  - `table.c`, `table.h` - реализация хеш-таблицы
  - `menu.c`, `menu.h` - пользовательский интерфейс
  - `input.c`, `input.h` - ввод данных
- **Особенности:**
  - Разрешение коллизий методом открытой адресации
  - Рехеширование при достижении коэффициента заполнения

#### Вариант 3b: Хеш-таблица с методом цепочек
- **Файлы:**
  - `table.c`, `table.h` - реализация хеш-таблицы
  - `menu.c`, `menu.h` - пользовательский интерфейс
  - `input.c`, `input.h` - ввод данных
- **Особенности:**
  - Разрешение коллизий методом цепочек
  - Каждая ячейка таблицы содержит указатель на список элементов

#### Вариант 3c: Деревья поиска
- **Файлы:**
  - `hashtable.c`, `hashtable.h` - реализация бинарного дерева поиска
  - `menu.c`, `menu.h` - пользовательский интерфейс
  - `input.c`, `input.h` - ввод данных
- **Операции:**
  - Вставка, удаление, поиск элементов
  - Балансировка дерева
  - Обходы дерева

### Лабораторная работа 4: Сбалансированные деревья

#### Вариант 4a: АВЛ-деревья
- **Файлы:**
  - `binary_tree.c`, `binary_tree.h` - реализация АВЛ-дерева
  - `tree_time.c`, `tree_time.h` - замеры времени выполнения операций
  - `menu.c`, `menu.h` - пользовательский интерфейс
- **Особенности:**
  - Самобалансирующееся бинарное дерево поиска
  - Высота поддеревьев отличается не более чем на 1

#### Вариант 4b: Splay-деревья
- **Файлы:**
  - `splay_tree.c`, `splay_tree.h` - реализация Splay-дерева
  - `tree_time.c`, `tree_time.h` - замеры времени выполнения операций
  - `menu.c`, `menu.h` - пользовательский интерфейс
  - `input.c`, `input.h` - ввод данных
- **Особенности:**
  - Самобалансирующаяся структура данных
  - Недавно использованные элементы быстрее доступны

### Лабораторная работа 5: Графы
- **Файлы:**
  - `graph.c`, `graph.h` - реализация графа
  - `queue.c`, `queue.h` - очередь для обхода графа
  - `menu.c`, `menu.h` - пользовательский интерфейс
  - `input.c`, `input.h` - ввод данных
  - `lw/input.c`, `lw/input.h` - дополнительные функции ввода
- **Функциональность:**
  - Представление графа (матрица смежности, список смежности)
  - Алгоритмы обхода (в глубину, в ширину)
  - Поиск кратчайшего пути
  - Поиск минимального остовного дерева

## Требования

- Компилятор C (gcc, clang, MSVC)
- CMake 3.XX

## Сборка и запуск

Каждая лабораторная работа содержит файл CMakeLists.txt для сборки с помощью CMake.

```bash
mkdir build
cd build
cmake ..
make
```