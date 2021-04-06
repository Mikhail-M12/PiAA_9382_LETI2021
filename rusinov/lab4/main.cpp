#include "string"
#include "vector"
#include "iostream"

#define task 2


std::string getArrowString(int stringLength, int index) {
    auto string = std::string();
    for (int i = 0; i < stringLength; ++i) {
        if (i == index) string += "\u2193";
        else string += " ";
    }
    return string;
}


void print(std::string & first, std::string & second,
           int firstIndex, int secondIndex, const std::string& information) {
    std::cout << getArrowString(first.length(), firstIndex) << " | ";
    std::cout << getArrowString(second.length(), secondIndex) << '\n';
    std::cout << first << " | " << second << '\n';
    std::cout << information << "\n\n";
}


std::vector<int> getPrefixFunction(std::string & pattern) {
    auto prefixFunction = std::vector<int>(pattern.length());

    // вычисляем префикс-функцию для строки-шаблона

    int i = 1;
    int j = 0;

    // индексы символов, который мы в данный момент рассматриваем
    // и сравниваем

    std::cout << "[Формирование префикс-функции]" << std::endl;

    while (i != pattern.length()) {

        std::cout << "i = " << std::to_string(i) << "; j = " << std::to_string(j) << "\n";

        if (pattern[i] == pattern[j]) {

            // если символы равны, то запишем в префикс функцию
            // для символа i образ j + 1

            auto information = std::string("Символы равны, поэтому образ функции под индексом ") + std::to_string(i) + std::string(" равен ") + std::to_string(j + 1);
            information += ", j и i увеличиваются на 1";
            print(pattern, pattern, j, i, information);

            prefixFunction[i] = j + 1;
            i++;
            j++;
        } else if (j == 0) {

            // если символ j начальный, и символы i j не равны,
            // то образ функции для i = 0

            auto information = std::string("Символы не равны, и символ под индексом j - начальный, поэтому образ функции под индексом ") + std::to_string(i) + std::string(" равен 0");
            information += ", i увеличивается на 1";
            print(pattern, pattern, j, i, information);

            prefixFunction[i] = 0;
            i++;
        } else {
            auto information = std::string("Символы не равны, и символ под индексом j - не начальный, поэтому новое значение j - ") + std::to_string(prefixFunction[j - 1]);
            print(pattern, pattern, j, i, information);
            j = prefixFunction[j - 1]; // иначе сдвигаем j назад
        }
    }

    return prefixFunction;
}


std::vector<int> getSubstringIndexes(std::string & text, std::string & pattern) {
    auto prefixFunction = getPrefixFunction(pattern);

    int textIndex = 0;
    int substringIndex = 0;
    auto result = std::vector<int>();

    std::cout << "[Поиск шаблона в тексте]" << std::endl;

    // пока не рассмотрим весь текст
    while (textIndex != text.length()) {

        std::cout << "textIndex = " << std::to_string(textIndex) << "; substringIndex = " << std::to_string(substringIndex) << "\n";

        if (text[textIndex] == pattern[substringIndex]) {
            // если символы равны, то смотрим следующие символы

            auto info = "";

            if (substringIndex + 1 == pattern.length()) {
                // если индекс конечный для подстроки, то мы нашли заданную подстроку в тексте
                // сдвигаемся назад в подстроке

                info = "Символы равны, в подстроке не осталось символов для сравнения, поэтому найдено вхождение подстроки в текст";
                print(text, pattern, textIndex, substringIndex, info);

                result.push_back(textIndex - substringIndex);
                substringIndex = prefixFunction[substringIndex];
            } else {
                info = "Символы равны, и в подстроке еще есть символы для сравнения, рассматриваем следующие символы";
                print(text, pattern, textIndex, substringIndex, info);
            }

            textIndex += 1;
            substringIndex += 1;

        } else { // Если символы не равны
            // если символ подстроки первый, то сдвигаем индекс текста
            if (substringIndex == 0) {
                auto info = "Символы не равны, рассматривался начальный символ подстроки, поэтому рассматриваем следующий символ текста";
                print(text, pattern, textIndex, substringIndex, info);
                textIndex += 1;
            }
            // если не первый, то сдвигаемся по значению в префикс-функции
            else {
                auto info = "Символы не равны, рассматривался не начальный символ подстроки, теперь он равен " + std::to_string(prefixFunction[substringIndex - 1]);
                print(text, pattern, textIndex, substringIndex, info);
                substringIndex = prefixFunction[substringIndex - 1];
            }
        }
    }

    std::cout << "Все символы текста рассмотрены, завершение работы алгоритма" << std::endl;

    return result;
}


void firstTask(std::string & text, std::string & pattern) {
    auto result = getSubstringIndexes(text, pattern);

    if (!result.empty()) {
        for (int i = 0; i < result.size() - 1; ++i) std::cout << result[i] << ',';
        std::cout << result[result.size() - 1];
    } else std::cout << -1;
}


void secondTask(std::string & text, std::string & pattern) {

    if (text.length() != pattern.length()) {
        std::cout << -1;
        return;
    }

    text = text + text;
    auto result = getSubstringIndexes(text, pattern);
    if (!result.empty()) std::cout << result[0];
    else std::cout << -1;
}


int main() {
    std::string text;
    std::string pattern;

    std::cin >> text;
    std::cin >> pattern;

    if (task == 1) {
        firstTask(text, pattern);
    } else {
        secondTask(text, pattern);
    }

    return 0;
}