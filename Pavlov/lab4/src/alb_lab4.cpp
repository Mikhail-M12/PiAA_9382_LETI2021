#include <iostream>
#include <string>
#include <vector>

#define TASK 1
//#define OUTPUT

std::vector<int> prefix_func(std::string str) {
#ifdef OUTPUT
    std::cout << "Построение префикс-функции для строки " << str << "\n";
#endif
    std::vector<int> pi(str.length(), 0);
    int i = 1, j = 0;
    while (i < str.length()) {
#ifdef OUTPUT
        std::cout << "\nСравниваются " << j + 1 << "-й символ (" << str[j]  << ") и " << i + 1 << "-й символ (" << str[i] << ") строки\n";
#endif
        if (str[i] == str[j]) {
#ifdef OUTPUT
            std::cout << "Символы равны, сравнение продолжается на позицию дальше\n";
#endif
            pi[i++] = ++j;
        }
        else if (j != 0) {
#ifdef OUTPUT
            std::cout << "Символы не равны, возврат к префиксу последнего совпавшего символа (" << str[j - 1] << ")\n";
#endif
            j = pi[j - 1];
        }
        else {
#ifdef OUTPUT
            std::cout << "Символы не равны и не найдено ни одного подходящего ненулевого префикса, сдвиг на позицию вперёд\n";
#endif
            pi[i++] = 0;
        }
    }
#ifdef OUTPUT
    std::cout << "\nПрефикс-функция построена и имеет следующий вид: ";
    for (auto &el : pi) {
        std::cout << el << " ";
    }
    std::cout << "\n";
#endif
    return pi;
}

std::vector<int> indVec(const std::string& pattern, const std::string& text) {
    std::vector<int> indVec;
    std::vector<int> prefix = prefix_func(pattern);
    int satisfied = pattern.length();

    int k = 0, l = 0;
#ifdef OUTPUT
    std::cout << "\nОсуществляется поиск шаблона " << pattern << " в строке " << text << "\n";
#endif
    while (k < text.length()) {
#ifdef OUTPUT
        std::cout << "\n";
#endif
        if (text[k] == pattern[l]) {
#ifdef OUTPUT
            std::cout << l << "-й символ шаблона (" << pattern[l] << ") и " << k << "-й символ строки (" << text[k] << ") совпадают\n";
#endif
            k++;
            l++;
            if (l == satisfied) {
#ifdef OUTPUT
                std::cout << "\nНайдено вхождение шаблона в строку, индекс символа шаблона меняется на предыдущий префикс pi(" << pattern[l - 1] << ") = " << prefix[l - 1] << "\n";
#endif
                l = prefix[l - 1];
                indVec.push_back(k - satisfied);
            }
        }
        else if (l != 0) {
#ifdef OUTPUT
            std::cout << "Символы " << pattern[l] << " и " << text[k] << " не равны, возврат к префиксу последнего совпавшего символа (" << pattern[l - 1] << ")\n";
#endif
            l = prefix[l - 1];
        }
        else {
#ifdef OUTPUT
            std::cout << "Символы " << pattern[l] << " и " << text[k] << " не равны и не найдено ни одного подходящего ненулевого префикса, сдвиг на позицию вперёд, сравнение с начала шаблона\n";
#endif
            k++;
        }

    }
#ifdef OUTPUT
    std::cout << "Поиск закончен, ";
#endif
    if (indVec.empty()) {
#ifdef OUTPUT
        std::cout << "не найдено ни одного вхождения\n";
#endif
        indVec.push_back(-1);
    }
#ifdef OUTPUT
    else {
        std::cout << "индексы вхождения шаблона в строку: ";
        for (auto& el : indVec) {
            std::cout << el << " ";
        }
        std::cout << "\n";
    }
#endif

    return indVec;
}

int isCyclicShift(const std::string& A, const std::string& B) {
#ifdef OUTPUT
    std::cout << "Проверка строки " << A << " на соответствие циклическому сдвигу строки " << B << "\n";
#endif
    if (A.length() != B.length()) {
#ifdef OUTPUT
        std::cout << "Длины сравниваемых строк не равны, циклический сдвиг невозможен.\n";
#endif
        return -1;
    }
    return indVec(B, A + A)[0];
}

int main()
{
    setlocale(LC_ALL, "rus");
    std::string str1 = "", str2 = "";
    std::cin >> str1 >> str2;

    if (TASK == 1) {
        std::vector<int> index = indVec(str1, str2);
        for (int i = 0; i < index.size(); i++) {
            std::cout << index[i];
            if (i < index.size() - 1) {
                std::cout << ",";
            }
        }
    }
    else if (TASK == 2) {
        int cyclic = isCyclicShift(str1, str2);
#ifdef OUTPUT
        if (cyclic > -1) {
            std::cout << str1 << " является циклическим сдвигом " << str2 << "\n";
        }
        else {
            std::cout << str1 << " не является циклическим сдвигом " << str2 << "\n";
        }
#endif
        std::cout << cyclic;
    }
    else {
        std::cout << "Неизвестный номер задания, завершение работы\n";
    }

    std::cout << "\n";
    return 0;
}