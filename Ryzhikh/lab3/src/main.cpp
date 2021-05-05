#include <iostream>
#include <map>
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <algorithm>

#define INFO


class MaxFlow {
private:
    char start;         // исток 
    char end;  //сток
    std::map<char, std::map<char, double>> edges;        // ребра с остаточной пропускной способностью
    std::map<char, std::map<char, double>> capacity;     // ребра с текущим потоком
    std::set<char> visited;  // посещенные вершины
    std::vector<char> path;  //путь

public:
    MaxFlow() {};
    void Read();
    void PrintEdges(const std::map<char, double>& e, const char& vert);
    void PrintFront(const std::queue<char>& f);
    void PrintCapacity();
    bool IsNewFront(const std::queue<char>& nf, char v);
    void FindPath();
    double AlgorithmFF();
};



void MaxFlow::Read() {   //считывание данных
    int n;
#ifdef INFO
    std::cout << "Введите количество ориентированных ребер графа:\n";
#endif
    std::cin >> n;
#ifdef INFO
    std::cout << "Введите исток и сток:\n";
#endif
    std::cin >> start >> end;
    char first, second;
    double weight;
#ifdef INFO
    std::cout << "Введите ребра графа и их пропускную способность:\n";
#endif
    for (int i = 0; i < n; i++) {
        std::cin >> first >> second >> weight;
        edges[first][second] = weight;
    }
}


void MaxFlow::PrintEdges(const std::map<char, double>& e, const char& ver) { //вывод ребер, исходящих из одной вершины
    for (auto& el : e)
        std::cout << ver << '-' << el.first << ": " << el.second << '\n';
}


void MaxFlow::PrintFront(const std::queue<char>& front) {  //вывод фронта
    std::queue<char> tmp = front;
    std::vector<char> arr;
    while (!tmp.empty()) {
        arr.push_back(tmp.front());
        tmp.pop();
    }
    for (auto& el : arr)
        std::cout << el;
    std::cout << '\n';
}


void MaxFlow::PrintCapacity() {  //вывод графов и их пропускной способности
    for (auto& a : capacity)
        for (auto& b : a.second)
            std::cout << a.first << ' ' << b.first << ' ' << b.second << '\n';
}


bool MaxFlow::IsNewFront(const std::queue<char>& f, char v) { //проверка на новый фронт
    auto tmp = f;
    while (!tmp.empty()) {
        char cur = tmp.front();
        tmp.pop();
        if (cur == v)
            return true;
    }
    return false;
}


void MaxFlow::FindPath() { //функция поиска пути в ширину
    char current = start;    // текущая вершина
    bool IsFound = false;   //конец поиска
    std::queue<char> frontier, NewFrontier; // непросмотренные вершины с пропускной способностью
    std::map<char, char> from;  // значение, откуда взята вершина (вершина, предыдущая вершина на пути)

    path.clear();
    visited.clear();
#ifdef INFO
    std::cout << "\nПоиск пути:\n";
#endif
    visited.emplace(start);

    while (!IsFound) {  //пока путь не найден
        std::vector<std::pair<char, double>> CurPathes; //ребра текущей вершины
#ifdef INFO
        std::cout << "\nТекущая вершина: " << current << '\n';
#endif
        if (edges.find(current) != edges.end()) { //получение ребер
            auto found = edges.find(current)->second;
            for (auto& el : found)
                CurPathes.push_back(std::make_pair(el.first, el.second));
#ifdef INFO
            PrintEdges(found, current);
#endif
        }
        else {
            CurPathes = std::vector<std::pair<char, double>>();
#ifdef INFO
            std::cout << "Нет ребер.\n";
#endif
        }

        auto iter_visited = visited.end();
        int n = (int)CurPathes.size();
        for (auto& vert : CurPathes) { //добавление всех непосещенных соседей к фронту
#ifdef INFO
            std::cout << "Проверка пути: " << current << '-' << vert.first << '\n';
#endif
            //проверка, если не было посещено или если пропускная способность больше 0
            if ((vert.second > 0) && (visited.find(vert.first) == visited.end()) &&
                (!IsNewFront(NewFrontier, vert.first) || vert.second > edges[from[vert.first]][vert.first])) {
#ifdef INFO
                std::cout << "Не было посещено ранее, пропускная способность больше 0 ==> добавление во фронт\n";
#endif
                if (!IsNewFront(NewFrontier, vert.first)) // добавление во фронт
                    NewFrontier.push(vert.first);
                from[vert.first] = current;
            }
            else
            {
#ifdef INFO
                std::cout << "Было посещено ранее или же вместимость равна 0.\n";
#endif
            }
        }
        if (!IsFound) {
            if (frontier.empty() && !(NewFrontier.empty())) { //переход к следующему фронту, если старый закончился; он становится посещенным
                auto tmp = NewFrontier;
                while (!tmp.empty()) {
                    char v = tmp.front();
                    if (v == end) {//проверка, если путь найден
#ifdef INFO
                        std::cout << "Путь найден. Текущая вершина конечная.\n";
#endif
                        IsFound = true;
                        break;
                    }
                    tmp.pop();
                    visited.emplace(v);
                }
                frontier = NewFrontier;
                while (!NewFrontier.empty())
                    NewFrontier.pop();
            }
            else if (frontier.empty() && NewFrontier.empty()) {
#ifdef INFO
                std::cout << "Путей больше нет.\n";
#endif
                break;
            }
#ifdef INFO
            std::cout << "Текущий фронт:\n"; //получаем следующую вершину из фронта
            PrintFront(frontier);
            if (!NewFrontier.empty())
            {
                std::cout << "Новый фронт:\n";
                PrintFront(NewFrontier);
            }
#endif
            current = frontier.front();
            frontier.pop();
        }
    }
    if (IsFound) { //получение пути
        char get = end;
        while (get != start) {
            path.push_back(get);
            get = from[get];
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
#ifdef INFO
        std::cout << "Путь: ";
        for (auto& v : path)
            std::cout << v;
        std::cout << '\n';
#endif
    }
}


double MaxFlow::AlgorithmFF() {  //алгоритм Форда-Фалкерсона
#ifdef INFO
    std::cout << "\nАлгоритм Форда-Фалкерсона:\n";
#endif
    double flow = 0;  //максимальный поток

    for (auto& v1 : edges)  //пропускная способность обнуляется
        for (auto& v2 : v1.second)
            capacity[v1.first][v2.first] = 0;

    while (FindPath(), !path.empty()) { //если путь существует
        char first = start;
        char second;
#ifdef INFO
        std::cout << "Пропускная способность пути:\n  "; //подсчет минимальной пропускной способности пути
#endif
        double MinCapacity = 1000;
        for (int i = 1; i < path.size(); i++) {
            second = path[i];
            double CurCapacity = edges[first][second];
            if (CurCapacity < MinCapacity)
                MinCapacity = CurCapacity;
#ifdef INFO
            std::cout << first << "-(" << CurCapacity << ")-" << second << ' ';
#endif
            first = second;
        }
#ifdef INFO
        std::cout << "\nМинимальная пропускная способность: " << MinCapacity << '\n';
#endif
        first = start;  //обновление графа
        for (int i = 1; i < path.size(); i++) {
            second = path[i];
            edges[first][second] -= MinCapacity;
            edges[second][first] += MinCapacity;
            first = second;
        }
        flow += MinCapacity; //обновление текущего максимального потока
        first = start; //обновление пропускной способности
        for (int i = 1; i < path.size(); i++) {
            second = path[i];
            if (capacity.find(first) != capacity.end() && capacity[first].find(second) != capacity[first].end())
                capacity[first][second] += MinCapacity;
            else
                capacity[second][first] -= MinCapacity;
            first = second;
        }
    }

    return flow;
}


int main() {
    setlocale(LC_ALL, "Russian");
    MaxFlow f;
    //f = MaxFlow();
    f.Read();
    double res = f.AlgorithmFF();
#ifdef INFO
    std::cout << "\n\nОтвет:\n";
#endif
    std::cout << res << '\n';  //максимальный поток
    f.PrintCapacity();
    return 0;
}