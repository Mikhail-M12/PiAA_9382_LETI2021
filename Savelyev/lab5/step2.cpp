#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#define OUTPUTCOM

// структура данных орисывающая вершину
struct Vertex {
    // контейнер прямых переходов
    map<char, int> next;
    // контейнер всех переходов включая суфф ссылки
    map<char, int> go;
    // символ по которому пришли в вершину
    char prev_char;
    // предыдущая вершина
    int prev_vert;
    // суф ссылка
    int suffix;
    // массив для хранения номеров паттернов
    vector<int> number;
    // является ли терминальной
    bool isTerminal = false;
};

int next_state(int index, char symb, vector<Vertex>& ver_mas);

// функция добалвения строки в бор
void add_to_bor(string& pat_string, vector<Vertex>& ver_mas, int& count) {
    #ifdef OUTPUTCOM
    cout << "Добавляем строку " << pat_string << " в бор.\n";
    #endif
    int current = 0;
    // обходим все символы паттерна
    for (int i = 0; i < pat_string.size(); i++) {
        #ifdef OUTPUTCOM
        cout << "\tТекущий символ паттерна: " << pat_string[i] << ".\n";
        cout << "\tТекущая вершина: " << current << ".\n";
        #endif
        // если прямой переход по символу не возможен
        if (ver_mas[current].next.find(pat_string[i]) == ver_mas[current].next.end()) {
            #ifdef OUTPUTCOM
            cout << "\tПуть через дугу с символом " << pat_string[i] << " не был найден. Добавляем новую вершину под номером " << ver_mas.size() << ".\n\n";
            #endif
            Vertex ver;
            ver.suffix = -1;
            ver.prev_vert = current;
            ver.prev_char = pat_string[i];
            ver_mas.push_back(ver);
            ver_mas[current].next[pat_string[i]] = ver_mas.size() - 1;
        }
        // если переход по символу есть
        else{
            #ifdef OUTPUTCOM
            cout << "\tПуть с таким ребром уже существует.\n\n";
            #endif
        }
        // переходим в следующую вершину
        current = ver_mas[current].next[pat_string[i]];
    }
    // выводим номер паттерна
    #ifdef OUTPUTCOM
    cout << "Паттерн зарегистрированн под номером " << count + 1 << ".\n";
    // выводим терминальную вершину и ее глубину в боре
    cout << "Терминальная вершина паттерна " << current << ".\n\n";
    #endif
    // записываем номер паттерна
    ver_mas[current].number.push_back(++count);
    ver_mas[current].isTerminal = true;
}

// функция вычисляющая суффикс ссылку
int get_suf(int ind, vector<Vertex>& ver_mas) {
    // Если суф ссылка не была найдена
    if (ver_mas[ind].suffix == -1) {
        // Если корень или потомок корня
        if (ind == 0 || ver_mas[ind].prev_vert == 0) {
            ver_mas[ind].suffix = 0;
        }
        else {
            // Рекурсивный поиск суфф ссылки
            #ifdef OUTPUTCOM
            cout << "\tИщем суф. ссылку из суффикса родительской вершины " << ver_mas[ind].prev_vert << " через дугу с символом "<< ver_mas[ind].prev_char << ".\n";
            #endif
            ver_mas[ind].suffix = next_state(get_suf(ver_mas[ind].prev_vert, ver_mas), ver_mas[ind].prev_char, ver_mas);
        }
    }
    #ifdef OUTPUTCOM
    cout << "\tНайдена суф. ссылка из вершины " << ind << " равная " << ver_mas[ind].suffix << ".\n\n";
    #endif
    return ver_mas[ind].suffix;
}

// функция перехода в след состояние автомата
int next_state(int ind, char chr, vector<Vertex>& ver_mas) {
    #ifdef OUTPUTCOM
    cout << "\tИщем путь из вершины " << ind << " по дуге с символом " << chr << ".\n";
    #endif
    // Если для данного символа нет перехода в массиве go
    if (ver_mas[ind].go.find(chr) == ver_mas[ind].go.end()) {
        // Если найден прямой переход по символу
        if (ver_mas[ind].next.find(chr) != ver_mas[ind].next.end()) {
            ver_mas[ind].go[chr] = ver_mas[ind].next[chr];
        }
        else {
              if (ind == 0) {
                  #ifdef OUTPUTCOM
                  cout << "\tЭто корень!\n";
                  #endif
                  // суф. ссылке присваеваем значение 0
                  ver_mas[ind].go[chr] = 0;
              }
              else {
                  // переходим по суф ссылке
                  #ifdef OUTPUTCOM
                  cout << "\tНет прямого пути, поиск пути от суф. ссылки этой вершины через дугу с символом " << chr << ".\n";
                  #endif
                  ver_mas[ind].go[chr] = next_state(get_suf(ind, ver_mas), chr, ver_mas);
              }
        }
    }
    #ifdef OUTPUTCOM
    cout << "\tНайден путь из вершины "<< ind << " в вершину " << ver_mas[ind].go[chr] <<".\n";
    #endif
    return ver_mas[ind].go[chr];
}

void search_pat(string& text, vector<Vertex>& ver_mas, vector<int>& res, vector<int>& pattern_offset_mas, int pat_length, vector<string>& pat_mas) {
    #ifdef OUTPUTCOM
    cout << "Начинаем поиск !\n";
    #endif
    int curr = 0;
    // обходим весь текст
    for (int i = 0; i < text.size(); i++) {
        #ifdef OUTPUTCOM
        cout << "Текущий символ текста: " << text[i] <<  "\n";
        cout << "Текущая вершина: " << curr << "\n";
        #endif
        // выполняем переход в автомате
        curr = next_state(curr, text[i], ver_mas);
        #ifdef OUTPUTCOM
        cout << "Перешли в вершину: " << curr << "\n";
        cout << "Проверяем является ли она терминальной или ссылается на терминальную:\n";
        #endif
        // заодно получаем суф ссылки
        for (int tmp = curr; tmp != 0; tmp = get_suf(tmp, ver_mas)) {
            #ifdef OUTPUTCOM
            cout << "\tПроверяем вершину : " << tmp <<"\n";
            #endif
            // если вершина терминальная
            if (ver_mas[tmp].isTerminal) {
                // перебираем все паттерны
                for (int j = 0; j < ver_mas[tmp].number.size(); j++) {
                    // если находится в промежутке от 0 до размера текста - размер паттерна
                    if ((i + 1 - pattern_offset_mas[ver_mas[tmp].number[j] - 1] - pat_mas[ver_mas[tmp].number[j] - 1].size() >= 0) && (i + 1 - pattern_offset_mas[ver_mas[tmp].number[j] - 1] - pat_mas[ver_mas[tmp].number[j] - 1].size() <= text.size() - pat_length)) {
                        res[i + 1 - pattern_offset_mas[ver_mas[tmp].number[j] - 1] - pat_mas[ver_mas[tmp].number[j] - 1].size()]++;
                        #ifdef OUTPUTCOM
                        cout << "\tТерминальная вершина," << " подпаттерн = " << pat_mas[ver_mas[tmp].number[j] - 1] << " Количество совпавщих подпаттернов " << res[i + 1 - pattern_offset_mas[ver_mas[tmp].number[j] - 1] - pat_mas[ver_mas[tmp].number[j] - 1].size()] <<
                            " из " << pattern_offset_mas.size() << ".\n";
                        #endif
                        if (res[i + 1 - pattern_offset_mas[ver_mas[tmp].number[j] - 1] - pat_mas[ver_mas[tmp].number[j] - 1].size()] == pattern_offset_mas.size()) {
                            #ifdef OUTPUTCOM
                            cout << "\tНайдено вхождение паттерна !" << '\n';
                            #endif
                        }
                    }
                }
            }
            // если вершина не терминалная
            else {
                #ifdef OUTPUTCOM
                cout << "\tНе является терминальной.\n\n";
                #endif
            }
        }
        #ifdef OUTPUTCOM
        cout << "Текущая суф. ссылка указывает на корень, считываем следующий символ текста." << "\n\n";
        #endif
    }
}

// функция печатающая результат и обрезающая исходный текст
void print_result(vector<int>& res, int pat_counter, string& cut_text, int pat_length, string& text) {
    #ifdef OUTPUTCOM
    cout << "Индексы вхождения шаблона в текст\n";
    #endif
    vector<bool> what_cut(text.size());
    // перебираем массив ответов
    for (int i = 0; i < res.size(); i++) {
        // если совпали все подпатерны
        if (res[i] == pat_counter) {
            cout << i + 1 << "\n";
            for (int j = 0; j < pat_length; j++) {
                what_cut[i + j] = true;
            }
        }
    }
    // перебираем исходный текст
    for (int i = 0; i < what_cut.size(); i++) {
        if (!what_cut[i])
            cut_text.push_back(text[i]);
    }
}

// функция разбивающая паттерн на подпаттерны
void split_pattern(string pattern, char joker, vector<string>& pat_mas, vector<int>& pattern_offset_mas){
    #ifdef OUTPUTCOM
    cout << "Разобьем исходный паттерн по джокерам, на подпатерны.\n";
    #endif
    string buf = "";
    // перебираем все символы паттерна
    for (int i = 0; i < pattern.size(); i++) {
        // если встретили символ джокера
        if (pattern[i] == joker){
            // в буфере есть символы
            if (buf.size() > 0) {
                // добавляем в массив паттернов
                pat_mas.push_back(buf);
                #ifdef OUTPUTCOM
                cout << "\tНайден новый подпаттерн: " << buf << "\n";
                #endif
                // добавляем индекс вхождения
                pattern_offset_mas.push_back(i - buf.size());
                #ifdef OUTPUTCOM
                cout << "\tИндекс начала в исходном паттерне: " << i - buf.size() << "\n";
                #endif
                buf = "";
            }
        }
        else {
            // добавляем символ в буфер
            buf.push_back(pattern[i]);
            if (i == pattern.size() - 1) {
                // добавляем в массив паттернов
                pat_mas.push_back(buf);
                #ifdef OUTPUTCOM
                cout << "\tНайден новый подпаттерн: " << buf << "\n";
                #endif
                // добавляем индекс вхождения
                pattern_offset_mas.push_back(i - buf.size() + 1);
                #ifdef OUTPUTCOM
                cout << "\tИндекс начала в исходном паттерне: " << i - buf.size() + 1 << "\n";
                #endif
            }
        }
    }
}

// функция считывающая паттерн
void read_pat(vector<Vertex>& ver_mas, char& joker, vector<int>& pattern_offset_mas, int& pat_length, vector<string>& pat_mas) {
    Vertex root;
    root.prev_vert = -1;
    root.suffix = -1;
    // добавляем корень
    ver_mas.push_back(root);
    int count = 0;
    #ifdef OUTPUTCOM
    cout << "Введите паттерн:\n";
    #endif
    string pattern;
    cin >> pattern;
    #ifdef OUTPUTCOM
    cout << "Введите символ джокера:\n";
    #endif
    cin >> joker;
    pat_length = pattern.size();
    // разбиваем паттерн на подпаттерны
    split_pattern(pattern, joker, pat_mas, pattern_offset_mas);
    // заносим в бор
    for (auto pattern : pat_mas) {
        add_to_bor(pattern, ver_mas, count);
    }

}

// функция поиска максимального количества дуг исходящих из вершины
int max_edge(vector<Vertex> ver_mas) {
    int max = ver_mas[0].next.size();
    // перебираем все вершины в массиве
    for(auto i : ver_mas) {
        if (i.next.size() > max)
            max = i.next.size();
    }
    return max;
}

// выводим автомат
void print_auto(vector <Vertex> ver_mas) {
    cout << "Автомат:\n";
    // перебираем все вершины по порядку
    for (int i = 0; i < ver_mas.size(); i++) {
        cout << "Дуги изходящие из вершины " << i << ":\n";
        for (auto a: ver_mas[i].next) {
            if(a.second > 0) {
                cout <<  " ---" << a.first << "--->  " << a.second << "\n";
            }
        }
        // если нет потомков
        if(ver_mas[i].next.size() == 0) {
            cout << "Лист бора!" << '\n';
        }
    }
}

int main() {
    string text;
    string cut_text;
    char joker;
    // массив вершин
    vector<Vertex> ver_mas;
    // массив паттернов
    vector<string> pat_mas;
    vector<int> res(110000);
    vector<int> pattern_offset_mas;
    int pat_length;

    #ifdef OUTPUTCOM
    cout << "Введите текст:\n";
    #endif
    cin >> text;

    for (int i = 0; i < 110000; i++){
        res[i] = 0;
    }
    // считываем паттерн
    read_pat(ver_mas, joker, pattern_offset_mas, pat_length, pat_mas);
    search_pat(text, ver_mas, res, pattern_offset_mas, pat_length, pat_mas);
    // выводим результат
    print_result(res, pat_mas.size(), cut_text, pat_length, text);
    #ifdef OUTPUTCOM
    cout << "\nОбрезанная строка: " << cut_text << "\n";
    // выводим макс кол-во дуг
    cout << "Максимальное количество дуг исходящих из одной вершины: " << max_edge(ver_mas) << "\n\n";
    #endif
    #ifdef OUTPUTCOM
    // выводим автомат
    print_auto(ver_mas);
    #endif

    return 0;
}
