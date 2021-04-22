#include <iostream>
#include <vector>
#include <fstream>
class KMP {
private:
    std::string B, A;
    int answer;
    std::vector<size_t> prefix;
public:
    void readData(std::istream& fin){
        fin >> A >> B;
        std::cout << "Для того чтобы определить, является ли А циклическим сдвигом В, воспользуемся алгоритмом поиска подстроки в строке\n";
        std::cout << "В нашем случае будем искать строку В в модифицированной А строке\n";
        std::cout << "Модифицируем строку А, сложив ее с собой\n";
        A = A + A;
    }

    void writePrefix(bool withP, int n) {
        if (withP) {
            std::cout << "Вывод префикс функции\n";
            for (auto i : B) {
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
        std::string temp = B;
        if (!isP)
            temp = A;
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
        size_t n = B.length();
        prefix = std::vector<size_t>(n);
        for (size_t i = 1; i < n; ++i) {
            size_t j = prefix[i - 1];
            while ((j > 0) && (B[i] != B[j])) {
                std::cout << "j не равен нулю, и символы не одинаковы\n";
                std::cout << "Присвоим j значение префикса предыдущего символа, на который указывала j\n";
                writeStepForPorT(j, i, true);
                j = prefix[j - 1];
                std::cout << "j = " << j << "\n";
            }
            if (B[i] == B[j]) {
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
        answer = -1;
        size_t n = B.length(), m = A.length();
        if (n != m / 2) {
            std::cout << "B и A не имеют одинаковую длину\n";
            return;
        }
        calculatePrefix();
        std::cout << "Определяем, является ли А циклическим сдвигом В\n";
        std::cout << "k - индекс символа строки А, l - индекс символа строки В\n";
        while (k < m) {
            if (A[k] == B[l]) {
                std::cout << "Символы одинаковы, смещаем k и l\n";
                writeVisualisation(k, l);
                k++;
                l++;
                if (l == n) {
                    std::cout << "В в модифицированной А найдена!\nИндекс начала строки В в А " << k - l << "\n";
                    writeVisualisation(k, l);
                    answer = k - l;
                    return;
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
        if (answer == -1) {
            std::cout << "B не является циклическим сдвигом A\n";
        } else {
            std::cout << "B является циклическим сдвигом A\nИндекс начала В в А\n";
        }
        std::cout << answer << "\n";
    }
};

void startProgram() {
    char answ = 'y';
    while (answ == 'y') {
        std::cout << "Хотите считать данные из файла или ввести самостоятельно?(1/2)\n";
        std::cin >> answ;
        KMP *kmp = new KMP();
        if (answ == '2') {
            std::cout << "Введите A и B\n";
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
