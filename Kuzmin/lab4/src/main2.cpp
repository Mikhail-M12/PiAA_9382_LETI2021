#include <iostream>
#include <string>
#include <vector>
#define INFO

//вычисление префикс-функции
std::vector<int> countPrefixFunction(std::string str) {

	int m = str.size();
//вектор из значений максимальных префикс-суффиксов
#ifdef INFO
	std::cout << "Вычисление префикс-функции для "<< str<<"\n";
#endif
	std::vector<int> p(str.size());
	p[0] = 0;

	//итерация по строке
	for (int i = 1; i < m; i++) {

		//длина макс. префикс-суффикса текущего префикс-суффикса

		int len = p[i - 1]; 
#ifdef INFO
		std::cout << "Текущая подстрока " << str.substr(0, i + 1) << "\n";
		if (len > 0)
			std::cout << "Префикс предыдущей подстроки, который возможно дополнить: " << str.substr(0, len) << "\n";
		else
			std::cout << "Префикс предыдущей подстроки является нулевым\n";
		
#endif

		//цикл, который проверит все символы-кандидаты на составление префикса и дойдет до нуля, если среди них нет подходящих
		while (len > 0 && str[len] != str[i]){

#ifdef INFO
			std::string pre = "_";
			if (len > 0)
				pre = str.substr(0, len);	
			std::cout << "Пара-кандидат на дополнение префикса " << pre << " : " << str[len] << ", " << str[i] << "\n";
#endif
			//берется следующий возможный префикс-суффикс
			len = p[len - 1];

#ifdef INFO
			pre = "_";
			if (len > 0)
				pre = str.substr(0, len);
			std::cout << "Невозможно дополнить текущий префикс, переход к префиксу " << pre << "\n";
#endif		
		}		
		//увеличение длины префикса
		if (str[len] == str[i])
			len += 1;

		//изменение дополняемого префикса
		else if (len != 0) {
			len = p[len - 1];
		}


		p[i] = len;
#ifdef INFO
		std::cout << "Получен префикс длины " << len << " для подстроки длины " << i + 1 << "\n\n";
#endif

	}
#ifdef INFO
	std::cout << "Полученная префикс-функция: ";
	for (auto& it : p)
		std::cout << it << " ";
	std::cout << "\n";
#endif
	return p;

}
//поиск подстроки
std::vector<int> findPatternKMP(std::string text, std::string pattern){

	int n = text.size();
	int m = pattern.size();
#ifdef INFO
	std::cout << "Поиск подстроки по алгоритму Кнута-Морриса-Пратта\n";
	std::cout << "Текст - " << text << "\nСтрока: " << pattern << "\n\n";
#endif
	std::vector<int> indexes;
	std::vector<int> pattern_prefixes = countPrefixFunction(pattern);
	int match = 0;
#ifdef INFO
	std::cout << "Переход к поиску подстроки\n\n";
#endif

	for (int i = 0; i < n; i++) {
#ifdef INFO
		std::cout << "Длина текущего совпадения: " << match << "\n";
		std::cout << "Символ текста: " << text[i] << "\nСимвол искомой строки: " << pattern[match % m] << "\n\n";
#endif		
		//обновление совпадающей части
		while (match > 0 && pattern[match] != text[i]){
#ifdef INFO
			std::string pre = "_";
			int newMatch = pattern_prefixes[match - 1];
			if (newMatch > 0)
				pre = pattern.substr(0, newMatch);
			std::cout << "Обновление совпадающей части " << pattern.substr(0, match) << " -> " << pre <<"\n";
#endif
			match = pattern_prefixes[match - 1];
		}


		//если символы на текущей позиции в строках совпадают
		//длина совпадения увеличивается
		if (pattern[match] == text[i]) {
#ifdef INFO
			std::cout << "Символы совпадают. Увеличение длины совпадающей части\n\n";
#endif
			match++;
		}
		//изменени длины совпадающей части префикса
		else if (match != 0){
#ifdef INFO
			std::cout << "Найден несовпадающий символ. Изменение длины совпадающей части\n\n";
#endif
			match = pattern_prefixes[match - 1];
		
		}

		//если в какой-то момент длина совпадения станет равной длине строки
		if (match == m) {
#ifdef INFO
			std::cout << "Найдено совпадение\nИндекс начала искомой строки в тексте: " << i - match + 1 << "\n\n";
#endif	
			indexes.push_back(i - match + 1);
		}
		
#ifdef INFO
		if (match == 0)
		std::cout << "Найден несовпадающий символ. Длина совпадения остается = 0\n\n";
#endif
	}
#ifdef INFO
	std::cout << "Алгоритм завершен\n";
#endif
	return indexes;
}

//вывод индексов вхождений подстроки
void printIndexes(std::vector<int> indexes){
	
	if (!indexes.empty()) {
		int _len = indexes.size() - 1;
		for (int i = 0; i < _len; i++)
			std::cout << indexes[i] << ",";
		std::cout << indexes[_len];
		return;
	}
	std::cout << "-1";
}

//проверка циклического сдвига
int circularShift(std::string shifted, std::string original) {

	if (shifted.size() != original.size())
		return -1;
#ifdef INFO
	std::cout << "Проверка циклического сдвига. \n\n";
#endif
	std::vector<int> indexes = findPatternKMP(shifted + shifted, original);
	if (!indexes.empty())
		return indexes[0];

	return -1;

}
//вывод результата проверки циклического сдвига
void doCircularShiftCheck() {
	std::string s1;
	std::string s2;
	std::cin >> s2 >> s1;
	int index = circularShift(s2, s1);
#ifdef INFO
	if (index != -1) {
		std::cout << "Строка " << s2 << " является циклическим сдвигом строки " << s1 << "\n";
		std::cout << "Индекс начала вхождения: ";
	}
	else {
		std::cout << "Строка " << s2 << " не является является циклическим сдвигом строки " << s1 << "\n";
		std::cout << "Релуьтат: ";
	}
#endif
	std::cout << index;
}
//вывод результата алгоритма КМП
void doKMPSearch() {
	std::string s1;
	std::string s2;
	std::cin >> s2 >> s1;
	std::vector<int> indexes = findPatternKMP(s1, s2);
#ifdef INFO
	if (!indexes.empty()) {

		std::cout << "Индексы вхождений строки " << s2 << " в " <<s1<<": ";
	}
	else {
		std::cout << "Строка " << s2 << " не входит в" << s1 << "\n";
		std::cout << "Релуьтат: ";
	}
#endif
	printIndexes(indexes);
}
int main() {

	setlocale(LC_ALL, "rus");
	//doKMPSearch();
	doCircularShiftCheck();
	return 0;
}