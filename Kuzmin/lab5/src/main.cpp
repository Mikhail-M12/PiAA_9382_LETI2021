#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
#include <functional>
#define NEWLINE std::cout<<"\n";
#define INFO
class Vertex {
public:
    std::map<char, Vertex*> neighbours;
    int name;
    Vertex(int  name) :name(name) {
        cameFrom = nullptr;
        suffixLink = nullptr;
    }
    void addNeighbour(char sym, Vertex* v) {
        neighbours[sym] = v;
    }
    bool operator<(Vertex v1) {
        return this->name < v1.name;
    }
    friend std::ostream& operator<<(std::ostream& out, Vertex* v) {
        out << v->name << "(" << v->str << ")";
        return out;
    }
    ~Vertex() {
        delete suffixLink;
        delete cameFrom;
    }
    Vertex* suffixLink;
    Vertex* cameFrom;
    std::vector<int> reachable;
    std::string str;
    char symbol;
    int end = 0; //для обозначениея того, является ли вершина концом строки

};
template <typename A, typename B>
A findByValue(std::map<A, B> m, B value) {
    for (const auto& it : m)
        if (value == it.second)
            return it.first;
    return 0;
}
template <typename T>
bool isInVector(T vec, Vertex v) {
    for (auto it : vec)
        if (it->name == v.name)
            return true;
    return false;
}
template <typename T>
void printQueue(T q) {
    while (!q.empty()) {
        std::cout << q.front()->name << " ";
        q.pop();
    }
    NEWLINE
}

//класс для бора
class Graph {
public:
    std::vector<Vertex*> vertexVector;
    Vertex* root;

    Vertex* operator()(char verName) {
        for (auto& it : vertexVector) {
            if (it->name == verName)
                return it;
        }
        return nullptr;
    }


    std::string getStringByVertex(Vertex* v) {
        std::string str;
        Vertex* cf = v->cameFrom;

    }
    //построение бора
    void buildGraph(std::vector<std::string> templates) {
#ifdef INFO
        std::cout << "Построение бора\n";
#endif 
        vertexVector.push_back(new Vertex(0));
        int vername = 0;
        int template_num = 1;

        //просмотр каждого образца
        for (const auto& str : templates) {

            Vertex* curr = vertexVector[0];
#ifdef INFO
            std::cout << "\nОбразец: " << str << "\n";
#endif
            //просмотр каждого символа для данного образца
            for (const auto& sym : str) {

#ifdef INFO
                std::cout << "\nСимвол: " << sym << std::endl;
                std::cout << "Текущая вершина: " << curr << std::endl;


                std::cout << "Соседи текущей вершины: ";
                for (auto& nei : curr->neighbours)
                    std::cout << "(" << nei.first << " " << nei.second->name << ") ";
                NEWLINE
#endif
                auto search = curr->neighbours.find(sym);

                //если нельзя перейти по существующей вершине
                if (search == std::end(curr->neighbours)) {
                    curr->neighbours[sym] = new Vertex(++vername);
                    Vertex* cameFrom = curr;
                    curr = curr->neighbours[sym];
                    vertexVector.push_back(curr);
                    curr->cameFrom = cameFrom;
                    curr->str += curr->cameFrom->str + sym;
#ifdef INFO
                    std::cout << "Создание новой вершины: " << curr->name << "\n";
#endif     
                }
                //если можно перейти по существующей вершине
                else {
                    Vertex* cameFrom = curr;
                    curr = search->second;
                    curr->cameFrom = cameFrom;
#ifdef INFO
                    std::cout << "Переход по существующей вершине: " << curr->name << "\n";
#endif
                }

                curr->symbol = sym;
            }
            curr->end = template_num++;
            curr->reachable.push_back(curr->end);

#ifdef INFO
            std::cout << "Вершина " << curr << " назначена терминальной\n";
#endif
        }
    }
    void buildSuffixLinks() {
#ifdef INFO
        std::cout << "\nПостроение суффиксных ссылок\n";
#endif
        std::queue< Vertex*> q; //очередь для поиска в ширину
        Vertex* curr = vertexVector[0];
        Vertex* old;
        root = curr;
        curr->suffixLink = curr;
        q.push(curr);
        while (!q.empty()) {
#ifdef INFO
            std::cout << "\nСостояние очереди: ";
            printQueue(q);
#endif
            curr = q.front();
            q.pop();
#ifdef INFO
            std::cout << "Текущая вершина - " << curr << "\nСоседи текущей вершины ";
#endif
            //добавление соседей в очередь
            for (const auto& it : curr->neighbours) {
#ifdef INFO
                std::cout << it.second << " ";
#endif
                q.push(it.second);
            }
#ifdef INFO
            std::cout << " добавлены в очередь\n";
#endif

#ifdef INFO
            std::cout << "Построение суффиксной ссылки для " << curr << "\n";
#endif
           //изначально в качестве суфф. ссылки присваивется корень бора
            curr->suffixLink = root;
            root->cameFrom = root;
            old = curr;
            char sym;
            if (curr->name != 0) {
                sym = *(curr->str.end() - 1);
            }
            //функция поиска суффиксной ссылки
            std::function<Vertex* (Vertex*)> findSuffixLink = [&](Vertex* v){
                Vertex* tmpSuffixLink = (v->suffixLink->name == 0) ? tmpSuffixLink = v->cameFrom->suffixLink : tmpSuffixLink = v->suffixLink;
              
#ifdef INFO
                std::cout << "Предыдущая вершина - " << v->cameFrom << "\n";
                std::cout << "Проверка соседей ее суффиксной ссылки " << tmpSuffixLink << ": ";
                for (const auto& nei : tmpSuffixLink->neighbours)
                    std::cout << "(" << nei.first << " " << nei.second->name << ") ";
                NEWLINE

#endif 
                //поиск подходящего соседа
                auto result = tmpSuffixLink->neighbours.find(sym);
                if (result != std::end(tmpSuffixLink->neighbours) && result->second->str.size() < old->str.size())
                    return result->second;

                //рекурсивный вызов функции поиска суффиксной ссылки, если это возможно
                return (v->name != 0 && tmpSuffixLink->name != 0) ? findSuffixLink(tmpSuffixLink) : root;
            };

            old->suffixLink = (old->name != 0) ? findSuffixLink(old) : old;
#ifdef INFO
            std::cout << "Суффиксная ссылка для вершины " << old << " - " << old->suffixLink << "\n";
#endif
        }
    }
    void findReachableThroughSuffixLinks(std::vector<std::string> patterns) {
#ifdef INFO
        std::cout << "\n\nПоиск строк, достижимых из вершин по суффиксным ссылкам\n";
#endif
      
        std::queue<Vertex*> q; //очередь для обхода в ширину
        Vertex* current;
        q.push(root);
        while (!q.empty()) {
            current = q.front();
            q.pop();
            //добавление соседей в очередь
            for (auto& it : current->neighbours){
                q.push(it.second);
            }
            Vertex* tmpSuff = current->suffixLink;
           
            bool a = tmpSuff->end != 0; //если вершина терминальная
            bool b = !tmpSuff->reachable.empty();//если имеются достижимые вершины по суффиксным ссылкам

            if (a || b) {
                if (a) {
                    //добавление новой вершины в вектор доступных
                    current->reachable.push_back(tmpSuff->end);
                }
                if (b) {

                    //объединение векторов доступных вершин
                    std::vector<int> v1 = current->reachable;
                    std::vector<int> v2 = tmpSuff->reachable;
                    std::vector<int> dest1;

                    std::sort(v1.begin(), v1.end());
                    std::sort(v2.begin(), v2.end());
                    std::set_union(v1.begin(), v1.end(),
                        v2.begin(), v2.end(),
                        std::back_inserter(dest1));
                    current->reachable = dest1;
                }
            }
        }
#ifdef INFO
        for (const auto& it : vertexVector) {
            for (const auto& it2 : it->reachable)
                if (it2 != it->end)
                    std::cout << "Образец " << patterns[it2 - 1] << " достижим из " << it << " по суффиксным ссылкам\n";
        }
#endif
    }

};
template <class T>
void printVector(std::vector<T> vec) {
    for (auto it : vec) {
        std::cout << it << " ";
    }
    std::cout << "\n";
}

//класс, реализующий автомат
class StateMachine {
public:
    Vertex* currentState;
    std::string alphabet = "ACGTN";
    std::map<std::pair<Vertex*, char>, Vertex*> statesMap;
    void buildStateMachine(Graph trie) {
#ifdef INFO
        std::cout << "\nПостроение автомата\n";
#endif
        for (const auto& it : trie.vertexVector) {

#ifdef INFO
            std::cout << "\nВершина " << it->name << "\n";
            std::cout << "Возможные переходы по конечным ссылкам:\n";
            for (const auto& n : it->neighbours) {
                std::cout << "Через " << n.first << " к " << n.second->name << "\n";
            }
            if (it->name != 0) {
                std::cout << "Возможные переходы по суффиксным ссылкам:\n";
                std::function<void(Vertex*)> printSuffixLink = [&](Vertex* suff) {

                    std::cout << "Суффиксная ссылка " << suff->name << ":\n";
                    for (const auto& n : suff->neighbours) {
                        std::cout << "Через " << n.first << " к " << n.second->name << "\n";
                    }
                    if (suff->name != 0)
                        printSuffixLink(suff->suffixLink);
                };
                printSuffixLink(it->suffixLink);
            }

#endif
            //вычисляем переход для каждого символа
            for (const auto& symbol : alphabet) {
                statesMap[std::make_pair(it, symbol)] = outState(it, symbol);
#ifdef INFO
                int s = statesMap[std::make_pair(it, symbol)]->name;
                std::cout << "Символ " << symbol << ". ";
                if (s != 0)
                    std::cout << "Найден переход к вершине " << statesMap[std::make_pair(it, symbol)]->name << "\n";
                else
                    std::cout << "Переход не найден. Установлен 0\n";
#endif
            }
            NEWLINE
        }
#ifdef INFO
        std::cout << "Построенный автомат:\n";

        for (const auto& it : trie.vertexVector) {
            std::cout << it->name << ":[";
            for (const auto& it2 : alphabet) {
                std::cout << it2 << "->" << statesMap[std::make_pair(it, it2)]->name;
                if (it2 != 'N')
                    std::cout << ", ";
            }
            
            std::cout << "]\nСтрока, соответствующая вершине - " << it->str << "\n";
        }
        NEWLINE
#endif
    }
    StateMachine(Vertex* state) :currentState(state) {}

    //функция перехода из состояния в состояние
    Vertex* outState(Vertex* state, char next) {
        for (auto& it : state->neighbours) {
            if (it.first == next) {
                return it.second;
            }
        }
        if (state->name == 0)
            return state;

        return outState(state->suffixLink, next);
    }
};

std::vector<std::pair<int, int>> findMatches(std::string text, std::vector<std::string> patterns, bool skip = false) {
    Graph trie;
    std::vector<std::pair<int, int>> indexesAndPatterns;
#ifdef INFO
    std::cout << "Поиск образцов:\n";
    for (const auto& it : patterns)
        std::cout << it << "\n";
    std::cout << "В тексте: ";
    std::cout << text << "\n\n";
#endif
    trie.buildGraph(patterns);
    trie.buildSuffixLinks();
    trie.findReachableThroughSuffixLinks(patterns);
    StateMachine a(trie.root);
    a.buildStateMachine(trie);
#ifdef INFO
    std::cout << "Обработка текста\n\n";
#endif
    for (int i = 0; i < text.size(); i++) {

#ifdef INFO
        std::cout << "Текущее состояние " << a.currentState << "\n";
        std::cout << "Символ на вход " << text[i] << "\n";
#endif
        //обрабатываем очередной символ
        a.currentState = a.statesMap[std::make_pair(a.currentState, text[i])];

#ifdef INFO
        std::cout << "Переход к состоянию " << a.currentState << "\n";
#endif
        //если встретился конец строки
        if (a.currentState->end > 0) {
#ifdef INFO
            std::cout << "Найден образец " << a.currentState->end << "(" << a.currentState->str << ") на позиции " << i - a.currentState->str.size() + 2 << "\n";
#endif
            auto tmp_pairss = std::make_pair(i - a.currentState->str.size() + 2, a.currentState->end);
            indexesAndPatterns.push_back(tmp_pairss);
            if (skip){
#ifdef INFO
                std::cout << "Переход в начальное состояние\n\n";
#endif
                a.currentState = trie.root;
                continue;
            }
            NEWLINE
        }
        
        //проверка строк, достижимых по суффиксным ссылкам
        for (const auto& en : a.currentState->reachable){

            if (en != a.currentState->end) {
#ifdef INFO
                std::cout << "Найден образец " << en << "(" << patterns[en - 1] << ") на позиции " << i - patterns[en - 1].size() + 2 << " через суффиксные ссылки\n";
#endif

                auto tmp_pair = std::make_pair(i - patterns[en - 1].size() + 2, en);
                indexesAndPatterns.push_back(tmp_pair);
                if (skip) {
                   
#ifdef INFO         
                    std::cout << "Переход в начальное состояние\n\n";
#endif
                    a.currentState = trie.root;
                    break;
                }
            }
        }
       
        //-----------------------
#ifdef INFO
        std::cout << "\n";
#endif

    }
    //сортировка получившегося вектора пар индексов
    auto cmp = [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return (b.first != a.first) ? b.first > a.first: b.second > a.second;
    };
    std::sort(indexesAndPatterns.begin(), indexesAndPatterns.end(), cmp);
#ifdef INFO
    std::cout << "Завершение алгоритма\n";
#endif
    return indexesAndPatterns;
}
void findWildCard(std::string text, std::string pattern, char sign) {
#ifdef INFO
    std::cout << "Поиск образца с джокером:\n";
    std::cout << pattern << "\n";
    std::cout << "В тексте: ";
    std::cout << text << "\n\n";
    std::cout << "Создание вектора, заполненного нулями длины " << text.size() << "\n";
    std::cout << "Поиск подстрок, не содержащих джокер\n";
#endif
    using namespace std;
    vector<string> noWildCardStrings;
    vector<int> indexes;
    vector<int> indexVector(text.size(), 0);
    string curr;
    int sz = indexVector.size();
    pattern += sign;
    int size = pattern.size();

    //поиск подстрок в образце, не содержащих джокер
    for (int i = 0; i < size; i++) {
        if (pattern[i] == sign) {
            if (!curr.empty()) {
                noWildCardStrings.push_back(curr);
                indexes.push_back(i - curr.size() + 1);
            }
            curr.clear();
            continue;
        }
        curr += pattern[i];
    }

#ifdef INFO
    std::cout << "Полученный вектор подстрок и их индексов вхождений в образец:\n";
    for (int i = 0; i < noWildCardStrings.size(); i++) {
         std::cout << noWildCardStrings[i] << " " << indexes[i] << "\n";
     }
    std::cout << "Поиск данных подстрок в тексте с помощью алгоритма Ахо-Корасик\n\n";
#endif
    //поиск подстрок в тексте алгоритмом Ахо-Корасик
    std::vector<std::pair<int, int>> pVec = findMatches(text, noWildCardStrings);
#ifdef INFO
    std::cout << "Найденные вхождения:\n";
    for (const auto& it : pVec) {
        std::cout << it.first <<" "<< it.second << "\n";
    }
#endif

    //измененеи соответсвующих элементов вектора индексов
    for (const auto& it : pVec){

        int index = it.first - indexes[it.second - 1];
        if (index >= 0 && index < sz) {
#ifdef INFO
            std::cout << "Увеличение значение элемента нулевого вектора с индексом " << index + 1 << "\n";
#endif
            indexVector[index]++;
        }
    }
    
    int nWSize = noWildCardStrings.size();
    int c = 1;

    //просмотр элементов вектора индексов
    for (const auto& it : indexVector) {
        if (it == nWSize && pattern.size() <= text.size() - c + 2) {
#ifdef INFO
            std::cout << "Образец найден на позиции ";
#endif
            std::cout << c << "\n";
        }
        c++;
    }
#ifdef INFO
    std::cout << "Завершение алгоритма\n";
#endif
}
//функция для ввода текста и образцов и реализации алгоритма Ахо-Корасик
void interfaceAhoCorasick() {
 
    std::vector<std::string> templates;
    std::string text;
    std::cin >> text;
    int n;
    std::cin >> n;
    if (n < 0) {
        std::cout << "Некорректный ввод\n";
        return;
    }
    for (int i = 0; i < n; i++) {
        std::string t;
        std::cin >> t;
        templates.push_back(t);
    }
    auto pV = findMatches(text, templates);
    for (auto it : pV)
        std::cout << it.first << " " << it.second << "\n";

}

//функция для ввода текста и образца и реализации алгоритма поиска с джокером
void interfaceWildCard() {
    std::string text;
    std::cin >> text;
    std::string pattern;
    std::cin >> pattern;
    std::string alphabet = "ACGTN";
    bool pass = false;
    char wildCard;
    std::cin >> wildCard;
    for (const auto& it : pattern)
        if (it != wildCard)
            pass = true;

    for (const auto& it : alphabet)
        if (it == wildCard)
            pass = false;

    if (!pass){
        std::cout << "Некорректный ввод\n";
        return;
    }
    findWildCard(text, pattern, wildCard);
}
int main() {

    setlocale(LC_ALL, "rus");

    //interfaceWildCard();
    interfaceAhoCorasick();
    return 0;
}