#include <iostream>
#include <string>
#include <vector>

void print_vector(std::vector<int> vec){
    for (int i : vec){
        std::cout << i;
    }
    std::cout << std::endl;
}

//Префикс функция
std::vector<int> prefix(const std::string& data){
    // Получаем длину строки
    std::cout << "Префикс функция получила на вход строку: " << data << std::endl;
    int n = data.length();
    //Инициализация вектора pi
    std::vector<int> pi(data.length(), 0);
    std::cout << "Вектор pi равен: ";
    print_vector(pi);
    // Проход по всему вектору с 1 элемента
    for(int i = 1; i < n; i++){
        std::cout << "Шаг: " << i << std::endl;
        int j = pi[i - 1];
        std::cout << "j = " << j << std::endl;
        //Выбор индекса из начала строки (строки которую ищем в другой)
        while(j > 0 && data[i] != data[j]){
            j = pi[j - 1];
            std::cout << "j = " << j << std::endl;
        }
        std::cout << "Сравнение элементов " << data[i] << " и " << data[j] << std::endl;
        if(data[i] == data[j]){
            std::cout << "pi[" << i <<"] = " << j+1 << std::endl;
            //Добавляем номер повторенной буквы в строке где ищем в вектор префикс функции
            pi[i] = j + 1;
        }else{
            std::cout << "pi[" << i <<"] = " << j << std::endl;
            //Добавляем номер повторенной буквы в строке где ищем в вектор префикс функции
            pi[i] = j;
        }
        std::cout << "Вектор pi на " << i << " шаге равен: ";
        print_vector(pi);
        std::cout << std::endl;
    }
    return pi;
}

std::vector<int> kmp(std::string& A, std::string& B){
    std::vector<int> res;
    //Составление строки для префикс функции
    std::string tmp = B + "#" + A + A;
    //Вызов префикс функции
    std::cout << "Вызов префикс функции" << std::endl;
    std::vector<int> pi = prefix(tmp);
    std::cout << "Префикс функция вернула: ";
    print_vector(pi);
    //Получает длину строки A
    int aLen = A.size();
    std::cout << "Длина строки A равна: " << aLen << std::endl;
    //Получает длину строки B
    int bLen = B.size();
    std::cout << "Длина строки B равна: " << bLen << std::endl;
    for(int i = 0; i < 2 * bLen; i++){
        std::cout << "Сравнение элементов " << "pi[aLen + 1 + i] ( = " << pi[aLen + 1 + i] << ") и aLen ( =" << aLen << ")" << std::endl;
        if(pi[aLen + 1 + i] == aLen){
            res.push_back(i - aLen + 1);
            //Если значение в векторе префикс функции совпало с длиной строки, то заносим в итоговый вектор элемент откуда началась строка
            std::cout << "Добавление в результирующий вектор индекса " << (i - aLen +1) << std::endl;
        }
    }
    return res;
}

int main(){
    //Объявление переменных, где будут храниться входдные данные
    std::string A, B;
    //Считывание данных
    std::cin >> A >> B;
    std::cout << "Вы ввели:\n"
              << A << "\n"
              << "и\n"
              << B << "\n";
    //Вызов алгоритма КМП
    std::vector<int> res = kmp(A, B);
    std::cout << "Ответ: ";
    //Если выходной вектор пустой, то печатается -1
    if(res.empty()){
        std::cout << -1;
        return 0;
    }
    //Если вектор не пустой, то выводится на экран ответ

    std::cout << res[0];
    return 0;
}