#include <iostream>
#include <string>
#include <vector>
#define ADDINFO

// вычисляет префикс-функцию
std::vector<int> countPrefixFunction(std::string str) {
#ifdef ADDINFO
  std::cout << "Считаем префикс-функцию для строки \"" << str << "\".\n"
            << std::endl;
  std::cout << "Значение функции для строки из одного первого символа = 0"
            << std::endl;
#endif

  int stringLen = str.size();

  std::vector<int> prefixes(
      stringLen);   // вектор из значений максимальных префикс-суффиксов
  prefixes[0] = 0;  // значение перефикс-функции для строки длины 1 всегда = 0

  for (int i = 1; i < stringLen; i++)  // рассмотрим подстроки всех длин
  {
    int borderPrefixLen = prefixes[i - 1];  // это значение граничное (оно может
                                            // увеличиться лишь на 1)
    int prefixLen = borderPrefixLen;

#ifdef ADDINFO
    std::cout << "\nРассматриваем подстроку = \"" << str.substr(0, i + 1)
              << "\"." << std::endl;
    std::cout << "Длина \"граничного\" префикса предыдущей подстроки: "
              << str.substr(0, prefixLen) << std::endl;
#endif

    while (prefixLen > 0 &&
           str[prefixLen] !=
               str[i])  // если последние символы не совпали - уменьшаем длину
    {
      prefixLen = prefixes[prefixLen - 1];  // полагая ее равной p(k-1)

#ifdef ADDINFO
      std::cout << "Крайние символы не совпали -> переходим к длине префикса = "
                << prefixLen << "." << std::endl;
#endif
    }

    if (str[prefixLen] == str[i])  // если последний добавленный символ равен
                                   // последнему символу в только что
                                   // увеличенном префиксе
    {
      prefixLen +=
          1;  // увеличим длину префикса и переходим к следующему индексу

#ifdef ADDINFO
      std::cout << "Крайние символы совпали -> увеличиваем длину префикса на 1."
                << std::endl;
#endif
    } else if (prefixLen !=
               0)  // если же мы дошли до длины 0, но так и не нашли совпадения
    {
      prefixLen = prefixes[prefixLen - 1];  // то останавливаем перебор,
                                            // полагаем значение равным 0, и
                                            // переходим к следующему индексу

#ifdef ADDINFO
      std::cout
          << "Мы дошли до минимальной длины, но так и не полуили совпаение."
          << std::endl;
#endif
    }

    prefixes[i] = prefixLen;

#ifdef ADDINFO
    std::cout << "На этом шаге в итоге получен префикс длины = " << prefixLen
              << "." << std::endl;
#endif
  }

#ifdef ADDINFO
  std::cout << "\nИтоговая полученная префикс-функция: ";
  for (auto& it : prefixes) std::cout << it << " ";
  std::cout << "\n";
#endif
  return prefixes;
}

// находит индексы вхождений подстроки в текст
std::vector<int> findPatternWithKMP(std::string text, std::string pattern) {
#ifdef ADDINFO
  std::cout << "Вызов алгоритма Кнута-Морриса-Пратта\n";
  std::cout << "Текст: " << text << "\nШаблон: " << pattern << "\n\n";
#endif

  int n = text.size();
  int m = pattern.size();

  std::vector<int> indexes;
  std::vector<int> pattern_prefixes = countPrefixFunction(pattern);
  int match = 0;

#ifdef ADDINFO
  std::cout << "\nПереход к поиску подстроки:\n" << std::endl;
#endif

  for (int i = 0; i < n; i++) {
#ifdef ADDINFO
    std::cout << "Длина текущего совпадения: " << match << "\n";
    std::cout << "Символ текста: " << text[i]
              << "\nСимвол шаблона: " << pattern[match % m] << "\n\n";
#endif

    while (match > 0 &&
           pattern[match] != text[i])  // обновление совпадающей части
    {
#ifdef ADDINFO
      std::string pre = "_";
      int newMatch = pattern_prefixes[match - 1];
      if (newMatch > 0) pre = pattern.substr(0, newMatch);
      std::cout << "Обновление совпадающей части " << pattern.substr(0, match)
                << " -> " << pre << "\n";
#endif

      match = pattern_prefixes[match - 1];
    }

    if (pattern[match] ==
        text[i])  // если символы на текущей позиции в строках совпадают
    {
#ifdef ADDINFO
      std::cout << "Символы совпадают. Увеличение длины совпадающей части\n\n";
#endif

      match++;  // то длина совпадения увеличивается
    }

    if (match == m)  // если длина совпадения равна длине шаблона
    {
#ifdef ADDINFO
      std::cout << "Найдено совпадение\nИндекс начала искомой строки в тексте: "
                << i - match + 1 << std::endl;
#endif

      indexes.push_back(i - match +
                        1);  // то добавим это соввпадение в список индексов
    }

#ifdef ADDINFO
    if (match == 0)
      std::cout << "Найден несовпадающий символ. Длина совпадения = 0\n"
                << std::endl;
#endif
  }

#ifdef ADDINFO
  std::cout << "Алгоритм завершил работу\n";
#endif
  return indexes;
}

// выводит индексы вхождений подстроки
void printIndexes(std::vector<int> indexes) {
  if (!indexes.empty()) {
    int indexesCount = indexes.size() - 1;
    for (int i = 0; i < indexesCount; i++) {
      std::cout << indexes[i] << ",";
    }
    std::cout << indexes[indexesCount];

    return;
  }

  std::cout << "-1";
}

// проверка циклического сдвига
int circularShift(std::string shifted, std::string original) {
#ifdef ADDINFO
  std::cout << "Проверка циклического сдвига. \n\n";
#endif

  if (shifted.size() != original.size()) return -1;

  std::vector<int> indexes = findPatternWithKMP(shifted + shifted, original);
  if (!indexes.empty()) return indexes[0];

  return -1;
}

// выполняет второе задание
void doCircularShiftCheck() {
  std::string s1;
  std::string s2;

  std::cin >> s2 >> s1;  // считывание ввода пользователя

  int index = circularShift(s2, s1);  // вызов самого алгоритма

#ifdef ADDINFO
  if (index != -1) {
    std::cout << "Строка " << s2 << " является циклическим сдвигом строки "
              << s1 << "\n";
    std::cout << "Индекс начала вхождения: ";
  } else {
    std::cout << "Строка " << s2
              << " не является является циклическим сдвигом строки " << s1
              << "\n";
    std::cout << "Выведен код ошибки: ";
  }
#endif

  std::cout << index;  // вывод полученного ответа
}

// выполняет первое задание
void doKMPSearch() {
  std::string s1;
  std::string s2;

  std::cin >> s2 >> s1;  // считывание ввода пользователя

  std::vector<int> indexes =
      findPatternWithKMP(s1, s2);  // вызов самого алгоритма

#ifdef ADDINFO
  if (!indexes.empty()) {
    std::cout << "Индексы вхождений строки " << s2 << " в " << s1 << ": ";
  } else {
    std::cout << "Строка " << s2 << " не входит в" << s1 << "\n";
    std::cout << "Релуьтат: ";
  }
#endif

  printIndexes(indexes);  // выводит готовый ответ
}

int main() {
  setlocale(LC_ALL, "rus");
  // doKMPSearch();          // 1 задание
  doCircularShiftCheck();  // 2 задание
  return 0;
}