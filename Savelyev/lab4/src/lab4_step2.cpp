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

// проверка на циклический сдвиг
int is_cycle(std::string string_a, std::string string_b) {
  // проверяем длинну
  if (string_a.length() != string_b.length()) {
    std::cout << "Длинны не равны => не циклический сдвиг" << '\n';
    return -1;
  }
  // вызываем функция для создания массива префиксоф
  std::vector<int> prefix_arr_b = create_prefix(string_b);
  int string_a_length = string_a.length();
  int string_a_length_2 = string_a_length * 2;
  int cur_len_b = 0;
  for (int i = 0; i < string_a_length_2; i++) {
    // так как индексов в два раза больше
    int j = i % string_a_length;
    // если символы не совпали
    if (string_b[cur_len_b] != string_a[j]) {
      std::cout << "Не совпали  " << j << " символа строки А "
                << "(" << string_a[j] << ") "
                << "и " << cur_len_b << " символа строки В "
                << "(" << string_b[cur_len_b] << ") "
                << "\n\n";
    }
    while (cur_len_b > 0 && string_b[cur_len_b] != string_a[j]) {
      cur_len_b = prefix_arr_b[cur_len_b - 1];
    }
    // если совпали символы
    if (string_b[cur_len_b] == string_a[j]) {
      std::cout << "Найдено совпадение " << j << " символа строки А "
                << "(" << string_a[j] << ") "
                << "и " << cur_len_b << " символа строки В "
                << "(" << string_b[cur_len_b] << ") " << '\n';
      cur_len_b++;
    }
    // если нашли вхождение
    if (cur_len_b == string_a_length) {
      std::cout << "\nНашли вхождение, индекс равен " << i - cur_len_b + 1
                << "\n\n";
      return i - cur_len_b + 1;
    }
  }
  std::cout << "Не циклический сдвиг" << '\n';
  return -1;
}


int main() {
  std::vector<int> answer;
  std::string string_a;
  std::string string_b;

  std::cout << "Введите строку А" << '\n';
  std::cin >> string_a;
  std::cout << "Введите строку В" << '\n';
  std::cin >> string_b;

  // stepic_2

  std::cout << is_cycle(string_a, string_b) << '\n';


  return 0;
}
