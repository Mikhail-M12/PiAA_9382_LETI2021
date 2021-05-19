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

std::vector<int> kmp(std::string& T, std::string& P){
    std::vector<int> res;
    //Составление строки для префикс функции
    std::string tmp = P + "#" + T;
    //Вызов префикс функции
    std::cout << "Вызов префикс функции" << std::endl;
    std::vector<int> pi = prefix(tmp);
    std::cout << "Префикс функция вернула: ";
    print_vector(pi);
    //Получает длину строки P
    int pLen = P.size();
    std::cout << "Длина строки P равна: " << pLen << std::endl;
    //Получает длину строки T
    int tLen = T.size();
    std::cout << "Длина строки T равна: " << tLen << std::endl;
    for(int i = 0; i < tLen; i++){
        std::cout << "Сравнение элементов " << "pi[pLen + 1 + i] ( = " << pi[pLen + 1 + i] << ") и pLen ( =" << pLen << ")" << std::endl;
        if(pi[pLen + 1 + i] == pLen){
            //Если значение в векторе префикс функции совпало с длиной строки, то заносим в итоговый вектор элемент откуда началась строка
            std::cout << "Добавление в результирующий вектор индекса " << (i - pLen +1) << std::endl;
            res.push_back(i - pLen + 1);
        }
    }
    return res;
}

int main(){
    //Объявление переменных, где будут храниться входдные данные
    std::string P, T;
    //Считывание данных
    std::cin >> P >> T;
    std::cout << "Вы ввели:\n\t"
              << P << "\n"
              << "и\n\t"
              << T << "\n";
    //Вызов алгоритма КМП
    std::vector<int> res = kmp(T, P);
    std::cout << "Ответ: ";
    //Если выходной вектор пустой, то печатается -1
    if(res.empty()){
        std::cout << -1;
        return 0;
    }
    //Если вектор не пустой, то выводится на экран ответ
    for (int i = 0; i < res.size(); ++i) {
        if (i != res.size()-1){
            std::cout << res[i] << ',';
        } else{
            std::cout << res[i];
        }
    }
    return 0;
}