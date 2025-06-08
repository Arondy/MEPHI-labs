/*
 Дан следующий фрагмент кода:
class S {
private:
    int x, y;
public:
    S():x(0), y(0){ }
    S(int a):x(a), y(a){ }
    S(int x0, int y0):x(x0), y(y0){ }
    // прототипы перегруженного оператора сложения
};
int main(){
    S a1, a2, a3;
    int k;
    a1 = a2 + a3;
    a1 = k + a3;
    a1 = a2 + k;
    return 1;
}
Руководствуясь приведенным кодом функции main(  ), укажите, какое минимальное количество прототипов
перегруженного оператора сложения должно быть включено в определение класса S.
Приведите необходимые прототипы.
Ответ обоснуйте.
 */

class S {
private:
    int x, y;
public:
    S() : x(0), y(0){}
    S(int a) : x(a), y(a){}
    S(int x0, int y0) : x(x0), y(y0){}
    // прототипы перегруженного оператора сложения
    friend S operator+(const S &first, const S &second){
        return S{first.x + second.x, first.y + second.y};
    }
    friend S operator+(S s, const int num){
        return S{s.x + num, s.y + num};
    }
    friend S operator+(const int num, S s){
        return S{s.x + num, s.y + num};
    }
};

int main(){
    S a1{1, 2}, a2{4, 6}, a3{10, 10};
    int k{2};
    a1 = a2 + a3;
    a1 = k + a3;
    a1 = a2 + k;
    return 1;
}