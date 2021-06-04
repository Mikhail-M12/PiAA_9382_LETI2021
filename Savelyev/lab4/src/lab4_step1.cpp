#include <iostream>
#include <vector>

// Вычисление префикс функции
std::vector<int> create_prefix(std::string text) {
  int text_length = text.length();
  std::vector<int> prefix_arr(text_length);
  prefix_arr[0] = 0;
  std::cout << "\nЗначение префикс-функции для символа под номером: " << 0
            << " (" << text[0] << ")"
            << " = " << 0 << '\n';
  for (int i = 1; i < text_length; i++) {
    int current_length = prefix_arr[i - 1];
    std::cout
        << "\nВычисление значения префикс-функции для символа под номером: "
        << i << " (" << text[i] << ")" << '\n';
    // Если предыдущий суффикс нельзя расширить, нужно рассмотреть суффикс
    // меньшего размера prefix_arr[current_length - 1]
    while (current_length > 0 && (text[current_length] != text[i])) {
      std::cout << "Предыдущий суффикс размера " << current_length
                << " нельзя расширить" << '\n';
      current_length = prefix_arr[current_length - 1];
      std::cout << "Рассмотрим новый суффикс меньшего размера: "
                << current_length << '\n';
    }
    // Проверяем можно ли расширить
    if (text[current_length] == text[i]) {
      std::cout << "Суффикс длинны " << current_length << " можно расширить"
                << '\n';
      current_length++;
      std::cout << "Новый размер суффикса: " << current_length << '\n';
    }
    std::cout << "Значения префикс-функции равно " << current_length << '\n';
    // заносим соответствующие значение в массив
    prefix_arr[i] = current_length;
  }

  std::cout << "\nЗначения префикс-функции:" << '\n';
  // Выводим символы текста
  for (auto j : text) {
    std::cout << j;
    std::cout.width(3);
  }
  std::cout << '\n';
  // Выводим соответствующие значения
  for (auto i : prefix_arr) {
    std::cout << i;
    std::cout.width(3);
  }
  std::cout << "\n\n";
  return prefix_arr;
}

void kmp(std::string sample, std::string text, std::vector<int> prefix_arr,
         std::vector<int>& answer) {
  int text_size = text.size();
  int sample_i = 0;
  int text_i = 0;
  std::cout << "Алгоритм Кнута-Морриса-Пратта"
            << "\n\n";
  while (text_i < text_size) {
    // если символы совпали
    if (text[text_i] == sample[sample_i]) {
      std::cout << "Найдено совпадение " << sample_i << " символа образца "
                << "(" << sample[sample_i] << ") "
                << "и " << text_i << " символа текста "
                << "(" << text[text_i] << ") " << '\n';
      sample_i++;
      text_i++;
      // если нашли вхождение
      if (sample_i == sample.size()) {
        std::cout << "Нашли вхождение, индекс равен " << text_i - sample_i
                  << "\n\n";
        answer.push_back(text_i - sample_i);
        // переходим на позицию равную предпоследнему значению префикс функции
        sample_i = prefix_arr[sample_i - 1];
      }
    }
    // если сравнение было с первым символом
    else if (sample_i == 0) {
        std::cout << "Не совпали " << sample_i << " символа образца "
                  << "(" << sample[sample_i] << ") "
                  << "и " << text_i << " символа текста "
                  << "(" << text[text_i] << ") " << "\n";
      text_i++;
    }
    // если по образцу продвинулись
    else {
        std::cout << "Не совпали " << sample_i << " символа образца "
                  << "(" << sample[sample_i] << ") "
                  << "и " << text_i << " символа текста "
                  << "(" << text[text_i] << ") " << "\n";
      sample_i = prefix_arr[sample_i - 1];
    }
  }
}

int main() {
  std::vector<int> answer;
  std::string string_a;
  std::string string_b;
  std::cout << "Введите строку А" << '\n';
  std::cin >> string_a;
  std::cout << "Введите строку В" << '\n';
  std::cin >> string_b;


  kmp(string_a, string_b, create_prefix(string_a), answer);
  std::cout << "" << '\n';
  bool flag = true;
  if (!answer.size()) {
    std::cout << -1;
  } else {
    // выводим ответ
    for (auto a : answer) {
    // если не первый символ из массива
      if (flag) {
        flag = false;
      } else {
        std::cout << ",";
      }
      std::cout << a;
    }
  }


  return 0;
}
