#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <iostream>
#include <algorithm>
#include <iterator>

using std::cout, std::endl;
namespace MyVec {
    /// Класс итератора для вектора
    /// \tparam T адресуемый итератором тип
    /// \tparam is_const итератор является const_iterator если true, обычным iterator в ином случае
    template <typename T, bool is_const>
    class VectorIterator {
    private:
        T *ptr;
    public:
        /// Категория итератора
        using iterator_category = std::random_access_iterator_tag;

        /// Адресуемый итератором тип
        using value_type = T;

        /// Тип, обозначающий дистанцию между двумя итераторами, т.е. результат std::distance(it1, it2)
        using difference_type = std::ptrdiff_t;

        /// Тип указателя на хранимое значение, для const_iterator - const T*, для обычного - T*
        using pointer = T *;

        /// Тип ссылки на хранимое значение, для const_iterator - const T&, для обычного - T&
        using reference = T &;

        /// Пустой конструктор
        VectorIterator() noexcept: ptr{nullptr}{};

        /// Конструктор по указателю
        /// \param p Указатель на значение
        explicit VectorIterator(T *p) : ptr(p){}

        /// Копирующий конструктор
        /// \tparam other_const Константность копируемого итератора
        /// \param other Ссылка на другой итератор
        template <bool other_const>
        explicit
        VectorIterator(const VectorIterator<T, other_const> &other) noexcept requires (is_const <= other_const): ptr{
                other.ptr}{}

        /// Перемещающий конструктор
        /// \tparam other_const Константность копируемого итератора
        /// \param other Другой итератор
        template <bool other_const>
        explicit
        VectorIterator(VectorIterator<T, other_const> &&other) noexcept requires (is_const <= other_const): ptr{
                other.ptr}{
            other.ptr = nullptr;
        }

        /// Копирующее перемещение
        /// \tparam other_const Константность копируемого итератора
        /// \param other Другой итератор
        /// \return Ссылка на переданный итератор
        template <bool other_const>
        VectorIterator &
        operator=(const VectorIterator<T, other_const> &other) noexcept requires (is_const <= other_const){
            if (this != &other){
                ptr = other.ptr;
            }
            return *this;
        }

        /// Перемещающее перемещение (-____-)
        /// \tparam other_const Константность копируемого итератора
        /// \param other Другой итератор
        /// \return Ссылка на переданный итератор
        template <bool other_const>
        VectorIterator &operator=(VectorIterator<T, other_const> &&other) noexcept requires (is_const <= other_const){
            if (this != &other){
                ptr = other.ptr;
                other.ptr = nullptr;
            }
            return *this;
        }

        /// Передвигает итератор на следующий элемент списка
        /// \return Ссылка на переданный итератор
        VectorIterator &operator++() noexcept{
            ++ptr;
            return *this;
        }

        /// Передвигает итератор на следующий элемент списка
        /// \return Состояние итератора до модификации
        VectorIterator operator++(int){
            VectorIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /// Передвигает итератор на предыдущий элемент списка
        /// \return Ссылка на переданный итератор
        VectorIterator &operator--() noexcept{
            --ptr;
            return *this;
        }

        /// Передвигает итератор на предыдущий элемент списка
        /// \return Состояние итератора до модификации
        VectorIterator operator--(int){
            VectorIterator tmp = *this;
            --(*this);
            return tmp;
        }

        /// Разыменование
        /// \return Ссылка на адресуемый элемент списка
        reference operator*() const noexcept{ return *ptr; }

        /// Обращение к полям элемента
        /// \return Указатель на адресуемый элемент списка
        pointer operator->(){ return ptr; }

        /// Перегрузка оператора "[]" для доступа к элементу по относительному смещению
        /// \param n Смещение относительно текущего положения итератора
        /// \return Ссылка на элемент списка с указанным смещением
        reference operator[](difference_type n){ return *(ptr + n); }

        /// Перегрузка оператора "[]" для доступа к элементу по относительному смещению (константная версия)
        /// \param n Смещение относительно текущего положения итератора
        /// \return Ссылка на константный элемент списка с указанным смещением
        reference operator[](difference_type n) const{ return *(ptr + n); }

        /// Перегрузка оператора "+=" для перемещения итератора на указанное смещение
        /// \param n Смещение для перемещения итератора
        /// \return Ссылка на текущий объект итератора после выполнения операции
        VectorIterator &operator+=(difference_type n){
            ptr += n;
            return *this;
        }

        /// Перегрузка оператора "-=" для перемещения итератора на указанное смещение
        /// \param n Смещение для перемещения итератора
        /// \return Ссылка на текущий объект итератора после выполнения операции
        VectorIterator &operator-=(difference_type n){
            ptr -= n;
            return *this;
        }

        /// Перегрузка оператора "+" для создания нового итератора с указанным смещением
        /// \param n Смещение для создания нового итератора
        /// \return Новый итератор, смещенный на указанное значение
        VectorIterator operator+(difference_type n) const{ return VectorIterator(ptr + n); }

        /// Перегрузка оператора "-" для создания нового итератора с указанным смещением
        /// \param n Смещение для создания нового итератора
        /// \return Новый итератор, смещенный на указанное значение
        VectorIterator operator-(difference_type n) const{ return VectorIterator(ptr - n); }

        /// Перегрузка оператора "+" для вычисления разницы между двумя итераторами
        /// \param other Другой итератор
        /// \return Разница в смещении между текущим и переданным итераторами
        difference_type operator+(const VectorIterator &other) const{ return ptr + other.ptr; }

        /// Перегрузка оператора "-" для вычисления разницы между двумя итераторами
        /// \param other Другой итератор
        /// \return Разница в смещении между текущим и переданным итераторами
        difference_type operator-(const VectorIterator &other) const{ return ptr - other.ptr; }

        /// Перегрузка оператора "+" для создания нового итератора с указанным смещением от начала списка
        /// \param n Смещение от начала списка
        /// \param it Итератор, к которому добавляется смещение
        /// \return Новый итератор, смещенный от начала списка на указанное значение
        friend VectorIterator operator+(difference_type n, const VectorIterator &it){
            return VectorIterator(n + it.ptr);
        }

        /// Перегрузка оператора "-" для создания нового итератора с указанным смещением от начала списка
        /// \param n Смещение от начала списка
        /// \param it Итератор, из которого вычитается смещение
        /// \return Новый итератор, смещенный от начала списка на указанное значение
        friend VectorIterator operator-(difference_type n, const VectorIterator &it){
            return VectorIterator(n - it.ptr);
        }

        /// Перегрузка оператора "<" для сравнения текущего итератора с другим
        /// \param other Другой итератор
        /// \return true, если текущий итератор меньше переданного, иначе false
        bool operator<(const VectorIterator &other) const{ return ptr < other.ptr; }

        /// Перегрузка оператора ">" для сравнения текущего итератора с другим
        /// \param other Другой итератор
        /// \return true, если текущий итератор больше переданного, иначе false
        bool operator>(const VectorIterator &other) const{ return ptr > other.ptr; }

        /// Перегрузка оператора "<=" для сравнения текущего итератора с другим
        /// \param other Другой итератор
        /// \return true, если текущий итератор меньше или равен переданному, иначе false
        bool operator<=(const VectorIterator &other) const{ return ptr <= other.ptr; }

        /// Перегрузка оператора ">=" для сравнения текущего итератора с другим
        /// \param other Другой итератор
        /// \return true, если текущий итератор больше или равен переданному, иначе false
        bool operator>=(const VectorIterator &other) const{ return ptr >= other.ptr; }

        /// Перегрузка оператора "=" для проверки равенства текущего итератора и другого
        /// \param other Другой итератор
        /// \return true, если текущий итератор равен переданному, иначе false
        bool operator==(const VectorIterator &other) const{ return ptr == other.ptr; }

        /// Перегрузка оператора "!=" для проверки неравенства текущего итератора и другого
        /// \param other Другой итератор
        /// \return true, если текущий итератор не равен переданному, иначе false
        bool operator!=(const VectorIterator &other) const{ return !(*this == other); }
    };

    static_assert(std::random_access_iterator<VectorIterator<std::size_t, false>>);
    static_assert(std::random_access_iterator<VectorIterator<std::size_t, true>>);

    /// Вектор
    /// \tparam T Тип хранимых значений
    template <typename T>
    class vector {
        /// Тип итератора
        using iterator = VectorIterator<T, false>;

        /// Тип константного итератора
        using const_iterator = VectorIterator<T, true>;

        /// Тип хранимых значений
        using value_type = T;

        /// Тип, обозначающий дистанцию между двумя итераторами, т.е. результат std::distance(it1, it2)
        using difference_type = std::ptrdiff_t;

        /// Тип указателя на хранимое значение, для const_iterator - const T*, для обычного - T*
        using pointer = T *;

        /// Ссылка на тип хранимых значений
        using reference = T &;

        /// Константная ссылка на тип хранимых значений
        using const_reference = const T &;

        /// Тип, используемый для преставления размера контейнера
        using size_type = std::size_t;
    private:
        size_type _size = 0;
        size_type _capacity = 0;
        T *array = nullptr;
        void erase_till_end(iterator pos) noexcept;
        void swap(vector &second) noexcept;
    public:
        /// Пустой конструктор, создаёт пустой вектор
        vector() noexcept(std::is_nothrow_default_constructible_v<T>) = default;

        /// Конструктор, заполняющий вектор n копиями T()
        /// \param capacity Количество элементов
        explicit vector(size_type capacity) requires std::copy_constructible<T>;

        /// Конструктор, заполняющий вектор n копиями el
        /// \param size Количество элементов
        /// \param el Значение элемента, которым будет заполнен вектор
        explicit vector(size_type size, const T &el) requires std::copy_constructible<T>;

        /// Конструктор, заполняющий вектор элементами из списка инициализации
        /// \param list inisializer_list, из которого будут скопированы значения
        vector(std::initializer_list<T> list) requires std::move_constructible<T>;

        /// Копирующий конструктор
        /// \param second Другой вектор
        vector(const vector &second) requires std::copy_constructible<T>;

        /// Перемещающий конструктор
        /// \param second Другой вектор
        vector(vector &&second) noexcept;

        /// Оператор присваивания копированием для вектора
        /// \param second Другой вектор
        /// \return Ссылка на текущий вектор после присваивания
        vector &operator=(const vector &second) requires std::copy_constructible<T>;

        /// Оператор присваивания перемещением для вектора
        /// \param second Другой вектор
        /// \return Ссылка на текущий вектор после присваивания
        vector &operator=(vector &&second) noexcept;

        /// Оператор присваивания из списка инициализации для вектора
        /// \param list inisializer_list, из которого будут скопированы значения
        /// \return Ссылка на текущий вектор после присваивания
        vector &operator=(std::initializer_list<T> list) requires std::move_constructible<T>;

        /// Оператор доступа к элементу вектора по индексу
        /// \param index Индекс элемента
        /// \return Ссылка на элемент вектора по указанному индексу
        [[nodiscard]] T &operator[](size_type index){ return array[index]; };

        /// Оператор доступа к элементу вектора по индексу (константная версия)
        /// \param index Индекс элемента
        /// \return Константная ссылка на элемент вектора по указанному индексу
        [[nodiscard]] const T &operator[](size_type index) const{ return array[index]; };

        /// Возвращает размер вектора (количество элементов)
        /// \return Размер вектора
        [[nodiscard]] size_type size() const noexcept{ return _size; };

        /// Возвращает текущую ёмкость вектора
        /// \return Текущая ёмкость вектора
        [[nodiscard]] size_type capacity() const noexcept{ return _capacity; };

        /// Проверяет, является ли вектор пустым
        /// \return true, если вектор пуст, иначе false
        [[nodiscard]] bool empty() const noexcept{ return _size == 0; };

        /// Увеличивает ёмкость вектора до указанной величины
        /// \param newCap Новая ёмкость вектора
        void reserve(size_type newCap);

        /// Возвращает константную ссылку на элемент вектора по указанному индексу с проверкой границ
        /// \param index Индекс элемента
        /// \return Константная ссылка на элемент вектора по указанному индексу
        [[nodiscard]] const T &at(size_type index) const;

        /// Возвращает ссылку на элемент вектора по указанному индексу с проверкой границ
        /// \param index Индекс элемента
        /// \return Ссылка на элемент вектора по указанному индексу
        [[nodiscard]] T &at(size_type index);

        /// Возвращает константную ссылку на первый элемент вектора
        /// \return Константная ссылка на первый элемент вектора
        [[nodiscard]] const T &front() const{ return array[0]; };

        /// Возвращает ссылку на первый элемент вектора
        /// \return Ссылка на первый элемент вектора
        [[nodiscard]] T &front(){ return array[0]; };

        /// Возвращает константную ссылку на последний элемент вектора
        /// \return Константная ссылка на последний элемент вектора
        [[nodiscard]] const T &back() const{ return array[_size - 1]; };

        /// Возвращает ссылку на последний элемент вектора
        /// \return Ссылка на последний элемент вектора
        [[nodiscard]] T &back(){ return array[_size - 1]; };

        /// Возвращает указатель на начало массива вектора
        /// \return Указатель на начало массива вектора
        [[nodiscard]] const T *data() const{ return array; };

        /// Возвращает указатель на начало массива вектора (константная версия)
        /// \return Указатель на начало массива вектора
        [[nodiscard]] T *data(){ return array; };

        /// Копирует элемент в конец вектора
        /// \param element Добавляемый элемент
        void push_back(const T &element) requires std::copy_constructible<T>;

        /// Перемещает элемент в конец вектора
        /// \param element Добавляемый элемент
        void push_back(T &&element) requires std::move_constructible<T>;

        /// Создает и добавляет элемент в конец вектора
        /// \tparam Args Типы аргументов для конструирования элемента
        /// \param args Аргументы для конструирования элемента
        template <typename ...Args>
        void emplace_back(Args &&... args) requires std::constructible_from<T, Args...>;

        /// Очищает вектор, вызывая деструктор хранимых объектов и устанавливая размер в 0
        void clear() noexcept;

        /// Деструктор
        ~vector();

        /// Получение итератора на начало списка
        /// \return Итератор, адресующий начало списка
        iterator begin() noexcept{ return iterator(array); }

        /// Получение итератора на конец списка
        /// \return Итератор, адресующий конец списка
        iterator end() noexcept{ return iterator(array + _size); }

        /// Получение константного итератора на начало списка
        /// \return Итератор, адресующий начало списка
        const_iterator begin() const noexcept{ return const_iterator(array); }

        /// Получение константного итератора на конец списка
        /// \return Итератор, адресующий конец списка
        const_iterator end() const noexcept{ return const_iterator(array + _size); }

        /// Получение константного итератора на начало списка
        /// \return Итератор, адресующий начало списка
        const_iterator cbegin() const noexcept{ return const_iterator(array); }

        /// Получение константного итератора на конец списка
        /// \return Итератор, адресующий конец списка
        const_iterator cend() const noexcept{ return const_iterator(array + _size); }

        /// Удаляет элемент из вектора по указанному итератору
        /// \param it Итератор на удаляемый элемент
        /// \return Итератор на элемент, следующий за удаленным элементом
        iterator erase(iterator it) noexcept;

        /// Удаляет элементы в диапазоне [first, last) из вектора
        /// \param first Итератор на начало диапазона
        /// \param last Итератор на конец диапазона (не включая)
        /// \return Итератор на элемент, следующий за последним удаленным элементом
        iterator erase(iterator first, iterator last) noexcept;
    };
}

namespace MyVec {
    template <typename T>
    using iterator = VectorIterator<T, false>;
    template <typename T>
    using const_iterator = VectorIterator<T, true>;

    template <typename T>
    vector<T>::vector(size_type capacity) requires std::copy_constructible<T>{
        reserve(capacity);
        for (size_type i = 0; i < capacity; i++){
            new(&array[i]) T();
        }
        _size = capacity;
    }

    template <typename T>
    vector<T>::vector(size_type size, const T &el) requires std::copy_constructible<T>{
        reserve(size);
        for (size_type i = 0; i < size; i++){
            new(&array[i]) T(el);
        }
        _size = size;
    }

    template <typename T>
    vector<T>::vector(std::initializer_list<T> list) requires std::move_constructible<T>{
        reserve(list.size());

        size_type i = 0;
        for (const T &element: list){
            new(&array[i++]) T(element);
        }
        _size = list.size();
    }

    template <typename T>
    vector<T>::vector(const vector &second) requires std::copy_constructible<T>{
        reserve(second.size());

        for (size_type i = 0; i < second._size; i++){
            new(&array[i]) T(second.array[i]);
        }
        _size = second.size();
    }

    template <typename T>
    vector<T>::vector(vector &&second) noexcept{
        swap(second);
    }

    template <typename T>
    vector<T> &vector<T>::operator=(const vector<T> &second) requires std::copy_constructible<T>{
        if (this != &second){
            clear();
            reserve(second._capacity);

            for (size_type i = 0; i < second._size; i++){
                new(&array[i]) T(second.array[i]);
            }
            _size = second._size;
        }
        return *this;
    }

    template <typename T>
    vector<T> &vector<T>::operator=(vector<T> &&second) noexcept{
        if (this != &second){
            swap(second);
        }
        return *this;
    }

    template <typename T>
    vector<T> &vector<T>::operator=(std::initializer_list<T> list) requires std::move_constructible<T>{
        reserve(list.size());

        size_type i = 0;
        for (const T &element: list){
            new(&array[i++]) T(element);
        }
        _size = list.size();
        return *this;
    }

    template <typename T>
    void vector<T>::reserve(size_type newCap){
        if (newCap == 0){
            newCap = 1;
        } else if (newCap < _capacity){
            return;
        }
        size_type temp = _size;

        T *buffer = static_cast<T *>(::operator new(newCap * sizeof(T)));
        for (size_type i = 0; i < _size; i++){
            new(&buffer[i]) T(std::move(array[i]));
        }
        clear();
        ::operator delete(array, _capacity * sizeof(T));
        array = std::move(buffer);
        _capacity = newCap;
        _size = temp;
    }

    template <typename T>
    const T &vector<T>::at(size_type index) const{
        if (index >= _size){
            throw std::out_of_range("Index is out of range");
        }
        return array[index];
    }

    template <typename T>
    T &vector<T>::at(size_type index){
        if (index >= _size){
            throw std::out_of_range("Index is out of range");
        }
        return array[index];
    }

    template <typename T>
    void vector<T>::push_back(const T &element) requires std::copy_constructible<T>{
        if (_size == _capacity){
            reserve(_capacity * 2);
        }
        new(&array[_size++]) T(std::move(element));
    }

    template <typename T>
    void vector<T>::push_back(T &&element) requires std::move_constructible<T>{
        if (_size == _capacity){
            reserve(_capacity * 2);
        }
        new(&array[_size++]) T(std::move(element));
    }

    template <typename T>
    template <typename ...Args>
    void vector<T>::emplace_back(Args &&... args) requires std::constructible_from<T, Args...>{
        if (_size == _capacity){
            reserve(_capacity * 2);
        }
        new(&array[_size]) T{std::forward<Args>(args)...};
        _size++;
    }

    template <typename T>
    void vector<T>::clear() noexcept{
        for (int i = 0; i < _size; ++i){
            array[i].~T();
        }
        _size = 0;
    }

    template <typename T>
    vector<T>::~vector(){
        clear();
        ::operator delete(array, _capacity * sizeof(T));
    }

    template <typename T>
    iterator<T> vector<T>::erase(iterator it) noexcept{
        if (it >= end()){
            return end();
        }

        if (it + 1 != end()){
            std::move(it + 1, end(), it);
        }

        array[--_size].~T();
        return it;
    }

    template <typename T>
    iterator<T> vector<T>::erase(iterator first, iterator last) noexcept{
        if (first != last){
            if (last != end()){
                std::move(last, end(), first);
            }
            erase_till_end(first + (end() - last));
        }
        return first;
    }

    template <typename T>
    void vector<T>::erase_till_end(iterator pos) noexcept{
        size_type num = end() - pos;
        if (num){
            for (iterator it = pos; it != end(); ++it){
                it->~T();
            }
            _size -= num;
        }
    }

    template <typename T>
    void vector<T>::swap(vector &second) noexcept{
        std::swap(array, second.array);
        std::swap(_size, second._size);
        std::swap(_capacity, second._capacity);
    }
}

#endif