#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

#define OUTPUTCOM

// структура данных орисывающая вершину
struct Vertex {
    // контейнер прямых переходов
    map<char, int> next;
    // контейнер всех переходов включая суфф ссылки
    map<char, int> go;
    // предыдущая вершина
    int prev_vert;
    // суф ссылка
    int suffix;
    // символ по которому пришли в вершину
    char prev_char;
    // номер паттерна
    int number;
    // является ли терминальной
    bool isTerminal = false;
};

int next_state(int ind, char chr, vector<Vertex>& ver_mas);

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

// функция для поиска шаблонов
void search_pat(string& text, vector<Vertex>& ver_mas, vector<pair<int, int>>& res, vector<string>& pat_mas) {
    #ifdef OUTPUTCOM
    cout << "Начинаем поиск !\n";
    #endif
    int curr = 0;
    // перебираем все символы в тексте
    for (int i = 0; i < text.size(); i++) {
        #ifdef OUTPUTCOM
        cout << "Текущий символ текста: " << text[i] <<  "\n";
        cout << "Текущая вершина: " << curr << "\n";
        #endif
        // ищим следующие состояние автомата
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
                // добавляем ответ
                res.push_back(make_pair(i + 2 -  pat_mas[ver_mas[tmp].number - 1].size(), ver_mas[tmp].number));
                #ifdef OUTPUTCOM
                cout << "\tТерминальная вершина, позиия в тексте = " << i + 2 -  pat_mas[ver_mas[tmp].number - 1].size() << " паттерн = " << pat_mas[ver_mas[tmp].number - 1] << "\n";
                #endif
            }
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
    ver_mas[current].number = ++count;
    ver_mas[current].isTerminal = true;
}

// функция считывающая паттерны
void read_pat(vector<Vertex>& ver_mas, vector<string>& pat_mas) {
    Vertex root;
    root.prev_vert = -1;
    root.suffix = -1;
    // добавляем корень
    ver_mas.push_back(root);
    int count = 0;
    // количество паттернов
    int pat_counter;
    #ifdef OUTPUTCOM
    cout << "Введите количество паттернов:\n";
    #endif
    cin >> pat_counter;
    // считываем паттерны
    for (int i = 0; i < pat_counter; i++) {
        #ifdef OUTPUTCOM
        cout << "Введите паттерн:\n";
        #endif
        string pattern;
        cin >> pattern;
        // добавляем в массив паттернов
        pat_mas.push_back(pattern);
        // долабляем строку в бор
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


// функция печатающая результат и обрезающая исходный текст
void print_result(vector<pair<int, int>>& res, vector<string>& pat_mas, string& text, string& cut_text) {
    vector<bool> what_cut(text.size());
    // перебираем массив ответов
    for(auto i: res ) {
        cout << i.first << " " << i.second << '\n';
        // перебираем строку паттерна
        for (int j = 0; j < pat_mas[i.second - 1].size(); j++) {
            what_cut[i.first - 1 + j] = true;
        }
    }
    // перебираем исходный текст
    for (int i = 0; i < text.size(); i++) {
        if (!what_cut[i]) {
            cut_text.push_back(text[i]);
        }
    }
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
        if(size(ver_mas[i].next) == 0) {
            cout << "Лист бора!" << '\n';
        }
    }
}

bool comp( const pair<int, int>& p1, const pair<int, int>& p2 ) {
    return (p1.first < p2.first) || ((p1.first == p2.first) && (p1.second < p2.second));
}

int main() {
    string text;
    string cut_text;
    #ifdef OUTPUTCOM
    cout << "Введите текст:\n";
    #endif
    // текст в котором будет происходить поиск
    cin >> text;
    // массив вершин
    vector<Vertex> ver_mas;
    // массив паттернов
    vector<string> pat_mas;
    // массив пар позиция в тексте - номер шаблона
    vector<pair<int, int>> res;
    // считывем паттерны
    read_pat(ver_mas, pat_mas);
    // запускаем поиск
    search_pat(text, ver_mas, res, pat_mas);
    // сортируем для вывода по порядку
    sort(res.begin(), res.end(), comp);
    // выводим результат
    print_result(res, pat_mas, text, cut_text);
    // выводим обрезанную строку
    #ifdef OUTPUTCOM
    cout << "\nОбрезанная строка: " << cut_text << "\n";
    // выводим макс кол-во дуг
    cout << "Максимальное количество дуг исходящих из одной вершины: " << max_edge(ver_mas) << "\n\n";
    #endif
    // выводим автомат
    #ifdef OUTPUTCOM
    print_auto(ver_mas);
    #endif
    return 0;
}
