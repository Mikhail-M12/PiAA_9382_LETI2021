#include <iostream>
#include <vector>

#define INFO

class KMP {
private:
	std::string P, T;
	std::vector<int> arrPrefix;
	std::vector<int> answer;
public:
	KMP() {};
	void Read();
	void PrintAnswer();
	void Prefix();
	void AlgorithmKMP();
};


void KMP::Read() {
#ifdef INFO
    std::cout << "Введите первую строку P:\n";
#endif
    std::cin >> P;
#ifdef INFO
    std::cout << "Введите вторую строку T:\n";
#endif
    std::cin >> T;
}

void KMP::PrintAnswer() {
#ifdef INFO
    std::cout << "\nОтвет:\n";
#endif
    if (!answer.size()) {
#ifdef INFO
        std::cout << "P не входит в T\n";
#endif
        std::cout << -1;
    }
    else {
#ifdef INFO
        std::cout << "Индексы начал вхождений P в T:\n";
#endif
        for (auto a : answer) {
            if (a == answer.front())
                std::cout << a;
            else
                std::cout << "," << a;
        }
    }
    std::cout << '\n';
}

void KMP::Prefix() {   //префикс-функция
#ifdef INFO
    std::cout << "\nПодсчет префикс-функции:\n\n";
#endif
    int pLength = P.length();
    arrPrefix = std::vector<int>(pLength);
    arrPrefix[0] = 0;
#ifdef INFO
    std::cout << "Значение префикс-функции для символа " << P[0] << " под номером " << 0 << " равно " << 0 << "\n\n";
#endif
    for (int i = 1; i < pLength; i++) {
        int curLength = arrPrefix[i - 1];
#ifdef INFO
        std::cout << "Вычисление значения префикс-функции для символа " << P[i] << " под номером " << i << "\n";
#endif
        while (curLength > 0 && (P[curLength] != P[i])) {   // если предыдущий суффикс нельзя расширить, то рассматриваем суф-фикс меньшего размера
#ifdef INFO
            std::cout << "Предыдущий суффикс размера " << curLength << " нельзя расширить.\n";
#endif
            curLength = arrPrefix[curLength - 1];
#ifdef INFO
            std::cout << "Рассмотрим новый суффикс меньшего раз-мера: " << curLength << '\n';
#endif
        }

        if (P[curLength] == P[i]) {    // проверка на расширение
#ifdef INFO
            std::cout << "Суффикс длинны " << curLength << " можно расширить.\n";
#endif
            curLength++;
#ifdef INFO
            std::cout << "Новый размер суффикса: " << curLength << '\n';
#endif
        }
#ifdef INFO
        std::cout << "Значения префикс-функции равно " << curLength << "\n\n";
#endif
        arrPrefix[i] = curLength;
    }
#ifdef INFO
    std::cout << "\nЗначения префикс-функции:\n";
    for (auto j : P) {
        std::cout << j << ' ';
    }
    std::cout << '\n';
    for (auto i : arrPrefix) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
#endif
}


void KMP::AlgorithmKMP() {
    Prefix();
#ifdef INFO
    std::cout << "\n\nАлгоритм Кнута-Морриса-Пратта:\n\n";
#endif
    int tSize = T.size();
    int pSize = P.size();
    int pIndex = 0;
    int tIndex = 0;
    while (tIndex < tSize) {
#ifdef INFO
        std::cout << "Рассматривается " << pIndex << " символа образца " << P[pIndex] << " и " << tIndex << " символа текста " << T[tIndex] << '\n';
#endif
        if (P[pIndex] == T[tIndex]) {  //если символы равны
#ifdef INFO
            std::cout << "Найдено совпадение " << pIndex << " символа образца " << P[pIndex] << " и " << tIndex << " символа текста " << T[tIndex] << '\n';
#endif
            pIndex++;
            tIndex++;
            if (pIndex == pSize) {  //если вхождение найдено
#ifdef INFO
                std::cout << "Вхождение найдено, индекс равен " << tIndex - pIndex << "\n\n";
#endif
                answer.push_back(tIndex - pIndex);
                pIndex = arrPrefix[pIndex - 1]; //переход на по-зицию равную предпоследнему значению префикс-функции
            }
        }
        else if (pIndex == 0) // если сравнение с первым символом
        {
#ifdef INFO
            std::cout << "Cовпадение " << pIndex << " символа образца " << P[pIndex] << " и " << tIndex << " символа текста " << T[tIndex] << " не найдено\n";
            std::cout << "Сдвигаемся дальше по тексту\n";
#endif
            tIndex++;
        }
        else  //если же по образцу продвинулись
        {
#ifdef INFO
            std::cout << "Cовпадение " << pIndex << " символа образца " << P[pIndex] << " и " << tIndex << " символа текста " << T[tIndex] << " не найдено\n";
            std::cout << "Индекс образца принимает значение предыдущего префикса: " << arrPrefix[pIndex - 1] << '\n';
#endif
            pIndex = arrPrefix[pIndex - 1];
        }
#ifdef INFO
        std::cout << '\n';
#endif
    }
}


int main() {
    setlocale(LC_ALL, "Russian");
    KMP k;
    k = KMP();
    k.Read();
    k.AlgorithmKMP();
    k.PrintAnswer();
    return 0;
}
