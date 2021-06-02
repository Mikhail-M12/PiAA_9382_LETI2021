#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

int compare(std::pair<int, int> a, std::pair<int, int>  b) {
    if (a.first == b.first)
        return a.second < b.second;
    else
        return a.first < b.first;
}

struct bohr_vertex {//структура вершины бора
    int next_vertex[6];//массив вершин, в которые можно попасть из данной
    int num;//номер шаблона
    bool flag;//финальная вершина для шаблона
    int link;//переменная для хранения суффиксной ссылки
    int move[6];//массив переходов из одного состояния в другое
    int par;//номер вершины родителя
    char symbol;//символ по которому осуществляется переход от родителя
    int flink;//сжатые суффиксные ссылки
};

class Bohr{
public:
    Bohr() {
        bohr.push_back(make_vertex(-1, -1));
    }
    void find_count_of_arch() {
        int count[20];
        max_count_of_arch = 0;
        for (int i = 0; i < bohr.size(); i++) {
            count[i] = 0;
            std::cout << std::endl << "Вершина номер " << i << std::endl;
            if (i == 0) std::cout << "Это корень бора" << std::endl;
            else std::cout << "Вершина-родитель с номером " << bohr[i].par << " по символу " << vertex(bohr[i].symbol) << std::endl;
            std::cout << "Соседние вершины:" << std::endl;
            for (int j = 0; j < 6; j++) {
                if (bohr[i].next_vertex[j] != -1) {
                    std::cout << "Вершина " << bohr[i].next_vertex[j] << " по символу " << vertex(bohr[bohr[i].next_vertex[j]].symbol) << std::endl;
                    count[i]++;
                }
            }
            if (count[i] == 0) std::cout << "Это финальная вершина." << std::endl;
            std::cout << "Суффиксная ссылка: ";
            if (bohr[i].link == -1) std::cout << "еще не посчитана." << std::endl;
            else std::cout << vertex(bohr[i].link) << std::endl;
        }
        for (int i = 0; i < bohr.size(); i++)
            if (count[i] > max_count_of_arch) max_count_of_arch = count[i];

    }

    void add_string_to_bohr(std::string s) {//вставляет строку в бор
        std::cout << std::endl << "Добавляем шаблон \"" << s << "\" в бор." << std::endl;
        int num = 0;
        for (int i = 0; i < s.length(); i++) {//проходится по строке
            char ch = find_letter(s[i]);//находит номер символа
            if (bohr[num].next_vertex[ch] == -1) {//добавляется новая вершина если её не было
                bohr.push_back(make_vertex(num, ch));
                bohr[num].next_vertex[ch] = bohr.size() - 1;
                std::cout << "Добавим новую вершину " << bohr[num].next_vertex[ch] << " по символу " << s[i] << std::endl;
            }
            else std::cout << "Вершина по символу " << s[i] << " уже есть в боре" << std::endl;
            std::cout << "Перейдем к вершине " << bohr[num].next_vertex[ch] << std::endl;
            num = bohr[num].next_vertex[ch];//переходим к следующей вершине
        }
        std::cout << "Финальная вершина шаблона." << std::endl << std::endl;
        bohr[num].flag = true;
        pattern.push_back(s);
        bohr[num].num = pattern.size() - 1;
    }

    void find_all_possitions(std::string str) {//поиск шаблонов в строке
        int current = 0;//текущая вершина
        for (int i = 0; i < str.length(); i++) {
            std::cout << std::endl << "Символ текста " << str[i] << " с индексом " << i + 1 << std::endl;
            std::cout << std::endl << "Текущая вершина " << current << std::endl << "Вычислим функцию переходов." << std::endl << std::endl;
            current = get_auto_move(current, find_letter(str[i]));
            if (current != 0) std::cout << "Переход к вершине " << current << " по символу " << vertex(bohr[current].symbol) << std::endl;
            else std::cout << "Из вершины " << current << " нет ребра с символом " << str[i] << std::endl;
            for (int v = current; v != 0; v = get_suff_flink(v)) {
                if (bohr[v].flag) {
                    std::pair<int, int> res(i - pattern[bohr[v].num].length() + 2, bohr[v].num + 1);
                    answer.push_back(res);
                }
            }
            std::cout << std::endl << "Перейдем по хорошим суффиксным ссылкам вершины  " << current;
            if (i + 1 != str.length()) std::cout << " по символу " << str[i + 1] << std::endl;
            check(current, i + 1);//отмечаем по сжатым суффиксным ссылкам строки, которые нам встретились и их позицию
        }
        std::cout << std::endl << "Проход по строке текста завершен." << std::endl;
        std::cout << "------------------------------------------------------------------------------" << std::endl;
        for (int i = answer.size() -1; i >=0; i--) {
            std::string::const_iterator sub = find_end(str.begin(), str.end(), (pattern[answer[i].second - 1]).begin(), (pattern[answer[i].second - 1]).end());
            if (sub != str.end()) {
                if (i != 0 && answer[i - 1].first + pattern[answer[i - 1].second - 1].size() - 1 >= answer[i].first) {
                    str.erase(sub + answer[i - 1].first + pattern[answer[i - 1].second - 1].size() - answer[i].first, sub + pattern[answer[i].second - 1].size());
                }
                else
                    str.erase(sub, sub + pattern[answer[i].second - 1].size());
            }
        }
        sort(answer.begin(), answer.end(), compare);
        std::cout << std::endl << "Ответ:" << std::endl << "Позиция в тексте/номер шаблона" << std::endl;
        for (int i = 0; i < answer.size(); i++)
            std::cout << answer[i].first << " " << answer[i].second << std::endl;
        std::cout << std::endl << "Максимальное количество дуг, исходящих из одной вершины " << max_count_of_arch << std::endl;
        std::cout << "Строка после удаления найденных шаблонов: " << str << std::endl;
    }

private:
    int max_count_of_arch;
    bohr_vertex make_vertex(int par, char symbol) {//создание вершины бора
        bohr_vertex vertex;
        memset(vertex.next_vertex, 255, sizeof(vertex.next_vertex));
        vertex.flag = false;
        vertex.link = -1;
        memset(vertex.move, 255, sizeof(vertex.move));
        vertex.par = par;
        vertex.symbol = symbol;
        vertex.flink = -1;
        return vertex;
    }
    int find_letter(char symbol) {
        int ch;
        switch (symbol)
        {
            case 'A':
                ch = 0;
                break;
            case 'C':
                ch = 1;
                break;
            case 'G':
                ch = 2;
                break;
            case 'T':
                ch = 3;
                break;
            case 'N':
                ch = 4;
                break;
            default:
                break;
        }
        return ch;
    }

    char vertex(char v) {//для вывода на консоль
        char ch;
        switch (v)
        {
            case 0:
                ch = 'A';
                break;
            case 1:
                ch = 'C';
                break;
            case 2:
                ch = 'G';
                break;
            case 3:
                ch = 'T';
                break;
            case 4:
                ch = 'N';
                break;
            default:
                ch = '0';
                break;
        }
        return ch;
    }

    int get_auto_move(int v, char ch) {             //вычисляемая функция переходов
        if (bohr[v].move[ch] == -1) {
            if (bohr[v].next_vertex[ch] != -1) {            //если из текущей вершины есть ребро с символом ch
                std::cout << "Из вершины " << v << " есть ребро с символом " << vertex(ch) << std::endl;
                std::cout << "Переходим по этому ребру в вершину " << bohr[v].next_vertex[ch] << std::endl;
                bohr[v].move[ch] = bohr[v].next_vertex[ch];    //то идем по нему
            }
            else {//если нет
                if (v == 0) {//если это корень бора
                    bohr[v].move[ch] = 0;
                }
                else {
                    std::cout << "Перейдем по суффиксной ссылке." << std::endl << std::endl;
                    bohr[v].move[ch] = get_auto_move(get_suff_link(v), ch);  //иначе перейдем по суффиксальной ссылке
                }
            }
        }
        return bohr[v].move[ch];
    }

    int get_suff_link(int v) {//реализует получение суффиксной ссылки для данной вершины
        std::cout << std::endl << "Найдем суффиксную ссылку для вершины " << v << std::endl;
        if (bohr[v].link == -1) {
            if (v == 0 || bohr[v].par == 0) {//если это корень или начало шаблона
                if (v == 0) std::cout << "Текущая вершина - корень бора. Суффиксная ссылка равна 0." << std::endl;
                else std::cout << "Текущая вершина - начало шаблона. Суффиксная ссылка равна 0." << std::endl;
                bohr[v].link = 0;
            }
            else {
                std::cout << "Пройдем по суффиксной ссылке предка " << bohr[v].par << " и запустим переход по символу " << vertex(bohr[v].symbol) << std::endl;
                bohr[v].link = get_auto_move(get_suff_link(bohr[v].par), bohr[v].symbol);  //пройдем по суф.ссылке предка и запустим переход по символу.
                std::cout << "Значит суффиксная ссылка для вершины " << v << " равна " << bohr[v].link << std::endl << std::endl;
            }
        }
        else std::cout << "Суффиксная ссылка для вершины " << v << " равна " << bohr[v].link << std::endl << std::endl;
        return bohr[v].link;
    }

    int get_suff_flink(int v) {//функция вычисления сжатых суффиксальных ссылок
        if (bohr[v].flink == -1) {
            int u = get_suff_link(v);
            if (u == 0) {//если корень или начало шаблона
                bohr[v].flink = 0;
            }
            else {//если по суффиксальной ссылке конечная вершина-равен суффиксальной ссылке, если нет-рекурсия.
                bohr[v].flink = (bohr[u].flag) ? u : get_suff_flink(u);
            }
        }
        return bohr[v].flink;
    }

    void check(int v, int i) {
        for (int u = v; u != 0; u = get_suff_flink(u)) {
            if (bohr[u].flag) {
                std::cout << std::endl << "Вершина " << u << " конечная для шаблона " << bohr[u].num + 1 << std::endl;
                std::cout << std::endl << "Найден шаблон с номером " << bohr[u].num + 1 << ", позиция в тексте " << i - pattern[bohr[u].num].length() + 1 << std::endl;
            }
            else std::cout << std::endl << "Вершина " << u << " не конечная" << std::endl;
        }
    }

    std::vector<std::pair<int, int>> answer;
    std::vector <bohr_vertex> bohr;//бор
    std::vector <std::string> pattern;//шаблоны
};


int main() {
    std::string text;
    int n;//количество шаблонов
    Bohr bohr;
    std::cout << "Введите текст:" << std::endl;
    std::cin >> text;
    std::cout << "Введите количество шаблонов:" << std::endl;
    std::cin >> n;
    std::cout << "Введите набор шаблонов:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::string temp;//шаблон
        std::cin >> temp;
        bohr.add_string_to_bohr(temp);
    }
    std::cout << "Вычисление количество дуг из вершин." << std::endl;
    bohr.find_count_of_arch();
    std::cout << std::endl << "Нахождение шаблонов в тексте." << std::endl;
    bohr.find_all_possitions(text);
    return 0;
}
