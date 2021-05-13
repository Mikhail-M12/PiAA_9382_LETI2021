#include <iostream>
#include <vector>

#define INFO

class KMP {
private:
	std::string A, B;
	std::vector<int> arrPrefix;
	int answer;
public:
	KMP() {};
	void Read();
	void PrintAnswer();
	void Prefix();
	void CyclicShift();
};

void KMP::Read() {
#ifdef INFO
    std::cout << "Введите первую строку А:\n";
#endif
    std::cin >> A;
#ifdef INFO
    std::cout << "Введите вторую строку В:\n";
#endif
    std::cin >> B;
}


void KMP::PrintAnswer() {
#ifdef INFO
    std::cout << "\n\nОтвет:\n";
#endif
    if (answer == -1) {
#ifdef INFO
        std::cout << "A Не является циклическим сдвигом B.\n";
#endif
        std::cout << answer << '\n';
    }
    else {
#ifdef INFO
        std::cout << "A является циклическим сдвигом B.\n";
        std::cout << "Индекс начала строки B в А: ";
#endif
        std::cout << answer << '\n';
    }
}


void KMP::Prefix() {   //префикс-функция
#ifdef INFO
    std::cout << "\nПодсчет префикс-функции:\n\n";
#endif
    int bLength = B.length();
    arrPrefix = std::vector<int>(bLength);
    arrPrefix[0] = 0;
#ifdef INFO
    std::cout << "Значение префикс-функции для символа " << B[0] << " под номером " << 0 << " равна " << 0 << '\n';
#endif
    for (int i = 1; i < bLength; i++) {
        int curLength = arrPrefix[i - 1];
#ifdef INFO
        std::cout << "Вычисление значения префикс-функции для символа " << B[i] << " под номером " << i << "\n";
#endif

        while (curLength > 0 && (B[curLength] != B[i])) {   // если предыдущий суффикс нельзя расширить, то рассматриваем суф-фикс меньшего размера
#ifdef INFO
            std::cout << "Предыдущий суффикс размера " << curLength << " нельзя расширить.\n";
#endif
            curLength = arrPrefix[curLength - 1];
#ifdef INFO
            std::cout << "Рассмотрим новый суффикс меньшего раз-мера: " << curLength << '\n';
#endif
        }

        if (B[curLength] == B[i]) {    // проверка на расширение
#ifdef INFO
            std::cout << "Суффикс длинны " << curLength << " можно расширить.\n";
#endif
            curLength++;
#ifdef INFO
            std::cout << "Новый размер суффикса: " << curLength << '\n';
#endif
        }
#ifdef INFO
        std::cout << "Значение префикс-функции равно " << curLength << "\n\n";
#endif
        arrPrefix[i] = curLength;
    }
#ifdef INFO
    std::cout << "\nЗначения префикс-функции:\n";
    for (auto j : B) {
        std::cout << j << ' ';
    }
    std::cout << '\n';
    for (auto i : arrPrefix) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
#endif
}


void KMP::CyclicShift() {
    if (A.length() != B.length()) {
#ifdef INFO
        std::cout << "Длинны строк не равны, значит это не цикли-ческий сдвиг.\n";
#endif
        answer = -1;
        return;
    }

    Prefix();
#ifdef INFO
    std::cout << "\n\nОпределение, является ли А циклическим сдвигом B:\n\n";
#endif
    int aLength = A.length();
    int curBLength = 0;
    for (int i = 0; i < aLength * 2; i++) { //поиск по удовоенной первой строке
        int j = i % aLength;
        if (B[curBLength] != A[j]) //если символы не равны
#ifdef INFO
            std::cout << "Несовпадение " << j << " символа строки А " << '(' << A[j] << ')' << " и " << curBLength << " символа строки В " << '(' << B[curBLength] << ")";
#endif
        while (curBLength > 0 && B[curBLength] != A[j]) {
            curBLength = arrPrefix[curBLength - 1];
        }
        std::cout << '\n';
        if (B[curBLength] == A[j]) { //если символы равны
#ifdef INFO
            std::cout << "Найдено совпадение " << j << " символа строки А " << '(' << A[j] << ')' << " и " << curBLength << " символа строки В " << '(' << B[curBLength] << ")";
#endif
            curBLength++;
        }

        if (curBLength == aLength) {  //если нашлось вхождение
            answer = i - curBLength + 1;
#ifdef INFO
            std::cout << "\n\nВхождение нашлось. Индекс равен " << answer << '\n';
#endif
            return;
        }
    }

    answer = -1;
    return;
}


int main() {
    setlocale(LC_ALL, "Russian");
    KMP k;
    k = KMP();
    k.Read();
    k.CyclicShift();
    k.PrintAnswer();
    return 0;
}
