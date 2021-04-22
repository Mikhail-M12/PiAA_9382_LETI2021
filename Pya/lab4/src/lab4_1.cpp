#include <iostream>
#include <vector>
#include <fstream>

class KMP {
private:
    std::string P, T;
    std::vector<int> answer;
    std::vector<size_t> prefix;
public:
    void readData(std::istream& fin){
        fin >> P >> T;
    }

    void writePrefix(bool withP, int n) {
        if (withP) {
            std::cout << "Вывод префикс функции\n";
            for (auto i : P) {
                std::cout << i << " ";
            }
            std::cout << "\n";
        }
        for (int i = 0; i < n;i++) {
            std::cout << prefix[i] << " ";
        }
        std::cout << "\n";
    }

    void writeStepForPorT(int j, int i, bool isP) {
        std::string temp = P;
        if (!isP)
            temp = T;
        for (int l = 0; l < temp.length(); l++) {
            if (l == j || l == i)
                std::cout << "\"" << temp[l] << "\"";
            else
                std::cout << temp[l];
        }
        std::cout << "\n";
    }

    void calculatePrefix() {
        std::cout << "Начинаем подсчет префикс функции\n";
        std::cout << "i - индекс первого символа для сравнения, j - индекс второго символа для сравнения\n";
        size_t n = P.length();
        prefix = std::vector<size_t>(n);
        for (size_t i = 1; i < n; ++i) {
            size_t j = prefix[i - 1];
            while ((j > 0) && (P[i] != P[j])) {
                std::cout << "j не равен нулю, и символы не одинаковы\n";
                std::cout << "Присвоим j значение префикса предыдущего символа, на который указывала j\n";
                writeStepForPorT(j, i, true);
                j = prefix[j - 1];
                std::cout << "j = " << j << "\n";
            }
            if (P[i] == P[j]) {
                std::cout << "Символы одинаковы, смещаем j и i\n";
                writeStepForPorT(j, i, true);
                ++j;
            } else {
                std::cout << "Символы не одинаковы, смещаем i\n";
                writeStepForPorT(j, i, true);
            }
            prefix[i] = j;
            writePrefix(false, i + 1);
        }
        writePrefix(true, n);
    }

    void writeVisualisation(int k, int l) {
        writeStepForPorT(k, k, false);
        writeStepForPorT(l, l, true);
        writePrefix(false, prefix.size());
    }
    void doAlgoritm() {
        int k = 0, l = 0;
        size_t n = P.length(), m = T.length();
        calculatePrefix();
        std::cout << "Начинаем поиск подстоки в строке\n";
        std::cout << "k - индекс символа строки для сравнения, l - индекс символа подстроки для сравнения\n";
        while (k < m) {
            if (T[k] == P[l]) {
                std::cout << "Символы одинаковы, смещаем k и l\n";
                writeVisualisation(k, l);
                k++;
                l++;
                if (l == n) {
                    std::cout << "Подстрока найдена!\nИндекс начала вхождения подстроки в строке " << k - l << "\n";
                    writeVisualisation(k, l);
                    answer.push_back(k - l);
                    std::cout << "Присваиваем l значение префикса предыдущего символа, на который указывала l\n";
                    l = prefix[l - 1];
                    std::cout << "l = " << l << "\n";
                }
            } else if (l == 0) {
                std::cout << "Символы не одинаковы, l = 0, смещаем k\n";
                writeVisualisation(k, l);
                k++;
            } else {
                std::cout << "Символы не одинаковы\n";
                std::cout << "Присваиваем l значение префикса предыдущего символа, на который указывала l\n";
                writeVisualisation(k, l);
                l = prefix[l - 1];
                std::cout << "l = " << l << "\n";
            }
        }
    }

    void writeAnswer() {
        if (answer.empty()) {
            std::cout << "Подстрока не входит в строку\n";
            std::cout << "-1";
            return;
        }
        std::cout << "Подстрока входит в строку\nИндексы начала вхождения подстроки в строку\n";
        for (auto i : answer) {
            if (i == answer.front())
                std::cout << i;
            else
                std::cout << "," << i;
        }
        std::cout << "\n";
    }
};


void startProgram() {
    char answ = 'y';
    while (answ == 'y') {
        std::cout << "Хотите считать данные из файла или ввести самостоятельно?(1/2)\n";
        std::cin >> answ;
        KMP *kmp = new KMP();
        if (answ == '2') {
            std::cout << "Введите подстроку и строку\n";
            kmp->readData(std::cin);
        } else {
            std::ifstream fin("test1.txt");
            kmp->readData(fin);
            fin.close();
        }
        kmp->doAlgoritm();
        kmp->writeAnswer();
        std::cout << "Хотите продолжить?(y/n)\n";
        std::cin >> answ;
    }
}

int main() {
    startProgram();
    return 0;
}