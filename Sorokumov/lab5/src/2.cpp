#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

struct numbers {
    int index;
    int pattern_num;
};


struct bohr_vertex {
    int next_vertex[6];//массив вершин, в которые можно попасть из данной
    bool flag;//финальная вершина для шаблона
    int link;//переменная для хранения суффиксной ссылки
    int move[6];//массив переходов из одного состояния в другое
    int par;//номер вершины родителя
    char symbol;//символ по которому осуществляется переход от родителя
    int flink;//сжатые суффиксные ссылки
    int pattern_num[40];//номера подшаблонов
};

class Borh{
public:
    Borh(){
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
            char ch = find(s[i]);//находит номер символа
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
        for (int i = 0; i < 40; i++) {
            if (bohr[num].pattern_num[i] == -1) {
                bohr[num].pattern_num[i] = pattern.size() - 1;
                break;
            }
        }
    }

    void find_all_pos(std::string s) {
        int u = 0;
        for (int i = 0; i < s.length(); i++) {
            std::cout << std::endl << "Символ текста " << s[i] << " с индексом " << i + 1 << std::endl;
            std::cout << std::endl << "Текущая вершина " << u << std::endl << "Вычислим функцию переходов." << std::endl << std::endl;
            u = get_auto_move(u, find(s[i]));
            if (u != 0) std::cout << "Переход к вершине " << u << " по символу " << vertex(bohr[u].symbol) << std::endl;
            else std::cout << "Из вершины " << u << " нет ребра с символом " << s[i] << std::endl;
            std::cout << std::endl << "Перейдем по хорошим суффиксным ссылкам вершины  " << u;
            if (i+1!= s.length()) std::cout << " по символу " << s[i + 1] << std::endl;
            check(u, i + 1);
        }
        std::cout << std::endl << "Проход по строке текста завершен." << std::endl;
    }

    void answer(std::vector<std::string> patterns, std::vector<int> patterns_pos, std::string text, std::string temp){
        int arr[10];
        int n = 0;
        std::vector<int> c(text.length(), 0);

        std::cout << "------------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl << "Найдем шаблон с джокерами в тексте" << std::endl;
        for (int i = 0; i < num.size(); i++) {
            std::cout << std::endl << "Текущий подшаблон " << pattern[num[i].pattern_num] << " с индексом в тексте " << num[i].index + 1 << " и индексом в шаблоне " << patterns_pos[num[i].pattern_num] << std::endl;
            if (num[i].index < patterns_pos[num[i].pattern_num] - 1) {//если индекс подшаблона меньше позиции подшаблона в строке
                std::cout << "индекс подшаблона равен " << num[i].index + 1 << " и меньше позиции подшаблона в шаблоне " << patterns_pos[num[i].pattern_num] << std::endl;
                continue;
            }
            c[num[i].index - patterns_pos[num[i].pattern_num] + 1]++;//увеличиваем счетчик количества пропусков
            std::cout << std::endl << "Количество пропусков между шаблонами равно " << c[num[i].index - patterns_pos[num[i].pattern_num] + 1] << std::endl;
            std::cout << "Количество джокеров между шаблонами равно " << patterns.size() << std::endl << std::endl;
            if (c[num[i].index - patterns_pos[num[i].pattern_num] + 1] == patterns.size() && //если количество пропусков равно колличеству джокеров
                num[i].index - patterns_pos[num[i].pattern_num] + 1 <= text.length() - temp.length()) { //и есть место чтобы закончить шаблон
                std::cout << "Количество джокеров между шаблонами равно количеству пропусков. Найден исходный шаблон в тексте" << std::endl;
                std::cout << "На позиции " << num[i].index - patterns_pos[num[i].pattern_num] + 2 << std::endl;
                arr[n] = num[i].index - patterns_pos[num[i].pattern_num] + 2;
                n++;
            }

        }

        std::cout << "------------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl << "Ответ:" << std::endl;
        for (int i = 0; i < n; i++) std::cout << arr[i] << std::endl;
        for (int i = n - 1; i >= 0; i--) {
            if (i != 0 && arr[i - 1] + temp.size() - 1 >= arr[i])
                text.erase(arr[i - 1] + temp.size() - 1, arr[i]-arr[i-1] );
            else
                text.erase(arr[i] - 1, temp.size());
        }
        std::cout << std::endl << "Максимальное количество дуг, исходящих из одной вершины " << max_count_of_arch << std::endl;
        std::cout << "Строка после удаления найденных шаблонов: " << text << std::endl;
    }
private:
    int max_count_of_arch;
    std::vector<numbers> num;
    std::vector <bohr_vertex> bohr;//бор
    std::vector <std::string> pattern;//шаблоны

    bohr_vertex make_vertex(int par, char symbol) {//создание вершины бора
        bohr_vertex vertex;
        memset(vertex.next_vertex, 255, sizeof(vertex.next_vertex));
        vertex.flag = false;
        vertex.link = -1;
        memset(vertex.move, 255, sizeof(vertex.move));
        vertex.par = par;
        vertex.symbol = symbol;
        vertex.flink = -1;
        memset(vertex.pattern_num, 255, sizeof(vertex.pattern_num));
        return vertex;
    }

    int find(char symbol) {//алфавит
        int ch;
        switch (symbol)
        {
            case 'A':
                ch = 0;
                break;
            case 'C':
                ch = 1;
                break;
            case'G':
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
        struct numbers s;
        for (int u = v; u != 0; u = get_suff_flink(u)) {
            if (bohr[u].flag) {
                for (int j = 0; j < 40; j++) {
                    if (bohr[u].pattern_num[j] != -1) {
                        s.index = i - pattern[bohr[u].pattern_num[j]].length();
                        s.pattern_num = bohr[u].pattern_num[j];
                        std::cout << std::endl << "Вершина " << u << " конечная для шаблона " << s.pattern_num+1 << std::endl;
                        std::cout << "Найден подшаблон с номером " << s.pattern_num+1 << ", позиция в тексте " << s.index << std::endl;
                        num.push_back(s);
                    }
                    else
                        break;
                }
            }
            else std::cout << std::endl << "Вершина " << u << " не конечная" << std::endl;
        }
    }




};


int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<std::string> patterns;  //подстроки при делении по джокеру
    std::vector<int> patterns_pos; //позиции подстрок
    std::string text;//текст
    std::string temp;//шаблон
    char joker;//джокер
    std::string pat;
    std::cout << "Введите текст:" << std::endl;
    std::cin >> text;
    std::cout << "Введите шаблон:" << std::endl;
    std::cin >> temp;
    std::cout << "Введите джокер:" << std::endl;
    std::cin >> joker;
    Borh borh;

    std::cout << std::endl << "Разделим шаблон на подстроки без джокера" << std::endl;
    for (int i = 0; i < temp.length(); i++) {
        if (temp[i] != joker) {
            patterns_pos.push_back(i + 1);
            for (int j = i; temp[j] != joker && j != temp.length(); j++) {
                pat += temp[j];
                i++;
            }
            borh.add_string_to_bohr(pat);
            patterns.push_back(pat);
            pat.clear();
        }
    }


    std::cout << "Вычисление количество дуг из вершин." << std::endl;
    borh.find_count_of_arch();


    std::cout << std::endl << "Нахождение шаблонов в тексте." << std::endl;
    borh.find_all_pos(text);
    borh.answer(patterns, patterns_pos, text, temp);

    return 0;
}


