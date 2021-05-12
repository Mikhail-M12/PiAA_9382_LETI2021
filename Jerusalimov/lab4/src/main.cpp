#include <iostream>
#include <vector>

bool choiseInfo = 0;
int choiseTask;

class KMP {
public:

    void Display() {
        if (choiseInfo) {
            std::cout << info;
        }
        info = "";
    }

    std::string PrintAnimAnswer(int j, int i, std::string pattern, std::string txt) {
        std::string res = "";

        for (int k = 0; k < i; ++k) {
            res += "  ";
        }
        res += "i\n";
        for (auto& k : txt) {
            res += std::string(1, k) + " ";
        }
        res += '\n';

        for (int k = 0; k < j; ++k) {
            res += "  ";
        }
        res += "j\n";
        for (auto& k : pattern) {
            res += std::string(1, k) + " ";
        }
        res += "\n\n";

        return res;

    }

    std::vector<int> Calculate(std::string pattern, std::string txt) {
        std::vector<int> pi;
        std::vector<int> check;


        pi = PrefixAndSuffix(pattern);//Подсчитываем префикс-функцию

        int i = 0, j = 0;//Заводим два индекса для прохода по основной строке и по паттерну соответственно.

        info += "Start index: \n" + PrintAnimAnswer(j, i, pattern, txt);
        Display();

        while (i < txt.length()) {
            //если символы поставили то инкрементируем индексы и переходим на следю символ
            if (pattern[j] == txt[i]) {
                info += "j = i: increment both\n";
                ++j;
                ++i;
            }

            //Индекс совпадает с длиной подстроки, значит мы дошли до конца и было совпадение
            //Записываем индекс начала найденной строки.
            if (j == pattern.length()) {
                info += "j = template length, match found, add index to result.\n"
                    "j move to the beginning of the pattern\n";
                check.push_back(i - j);
                j = pi[j - 1];
            }
            else if (i < txt.length() && pattern[j] != txt[i]) {

                info += "j != i: ";

                //если не первый элемент в паттерне, то возвращаемся на элемент, следующий за префиксом
                if (j != 0) {
                    info += "j != 0 increment j\n";
                    j = pi[j - 1];
                }
                //иначе рассматриваем следующий элемент в тексте
                else {
                    info += "j = 0 increment i, look at the next elem\n";
                    i++;
                }
            }
            info += "New index: \n";
            info += PrintAnimAnswer(j, i, pattern, txt);
            Display();
        }
        info += "All text is cheked, end algorithm\n";
        Display();
        return check;

    }

    void Task1(std::string pattern, std::string text) {
        result = Calculate(pattern, text);

        if (result.empty()) {
            std::cout << "-1";
        }
        else {
            for (int i = 0; i < result.size(); i++) {
                std::cout << result[i];
                if (i != result.size() - 1)
                    std::cout << ",";
            }
        }
    }

    void Task2(std::string pattern, std::string text) {

        if (pattern.length() != text.length()) {
            std::cout << "-1";
            return;
        }

        result = Calculate(text, pattern + pattern);
        if (result.empty()) {
            std::cout << "-1";
        }
        else {
            std::cout << result[0];
        }
    }

private:
    std::string printPrefixFunc(int j, int i, std::string pattern, std::vector<int> pi) {
        std::string res = "";


        for (int k = 0; k < j; ++k) {
            res += "  ";
        }
        res += "j ";

        for (int k = 0; k < i - j - 1; ++k) {
            res += "  ";
        }
        res += "i \n";

        for (auto& k : pattern) {
            res += std::string(1, k) + " ";
        }
        res += '\n';
        for (auto& k : pi) {
            res += std::string(1, k) + " ";
        }
        res += " - prefix func\n";


        return res;
    }

    std::vector<int> PrefixAndSuffix(std::string pattern) {

        std::vector<int> pi(pattern.length());//Сразу инициализируем длину префикс-функции
        pi[0] = 0;//Первый эллемент в массиве 0

        int j = 0;
        int i = 1;

        info += "____________________Calculate Prefix func_________________________________\nInitial state of the pattern\n" +
            printPrefixFunc(j, i, pattern, pi);
        Display();

        while (i < pattern.length()) {

            //если два текущих элемента в паттерне одинаковы
            //то запоминаем, что при несовпадении на элементе i + 1, следует перейти на элемент с индексом j
            if (pattern[i] == pattern[j]) {
                info += "Elements with indices i and j are the same, remember j and advance both indices\n";
                j++;
                pi[i] = j;
                i++;
            }
            else {
                info += "Elements with indices i and j are different\n";
                if (j != 0) {
                    info += "j is not zero, change j to the element with index j-1 in the prefix function array\n";
                    j = pi[j - 1];
                }
                else {
                    info += "j - zero, write zero to the element with index i and move i to the right\n";
                    pi[i] = 0;
                    i++;
                }
            }
            info += printPrefixFunc(j, i, pattern, pi) + "\n";
            Display();
        }
        info += "Index i has reached the end of the pattern string, we finish building the prefix function array.\n____________________________________________________________________\n";
        Display();
        return pi;
    }

    std::vector<int> result;
    std::string info;

};

int main() {
    std::string pattern, txt;

    std::cout << "Display intermediate data?\n Yes - 1 : No - 0\n";
    std::cin >> choiseInfo;
    std::cout << "Select task number, 1 or 2\n";
    std::cin >> choiseTask;
    std::cout << "Enter data\n";
    std::cin >> pattern >> txt;

    KMP toDo;
    if (choiseTask == 1) {
        toDo.Task1(pattern, txt);
    }
    else {
        toDo.Task2(pattern, txt);
    }
    system("pause>nul");
    return 0;
}
