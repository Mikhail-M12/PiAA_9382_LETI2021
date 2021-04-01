/**
 * Вар. 7. "Мультипоточный" А*: на каждом шаге из очереди с приоритетами извлекается n вершин
 * (или все вершины, если в очереди меньше n вершин). n задаётся пользователем.
 */

#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <queue>

#define DEBUG

class Asterisk
{
    char begin{}, end{}; // начальная и конечная вершина
    size_t nParallel{}; // количество вершин, снимаемых одновременно
    std::map<char, std::vector<std::pair<char, double >>> graph; // граф
    std::map<char, bool> viewedVertexes; // список просмотренных вершин
    std::map<char, std::pair<char, double>> shortestPaths; // кратчайшие пути до вершин

    // структура для хранения вершины графа
    struct Vertex
    {
        char name;
        double edge;
    };

    // компаратор для очереди с приоритетом
    struct Cmp
    {
        bool operator()(const Vertex &a, const Vertex &b)
        {
            if (a.edge == b.edge)
                return a.name < b.name;
            else
                return a.edge > b.edge;
        }
    };

    // восстанавливает весь путь от конечной до начальной вершины по списку найденных кратчайших путей
    void printPath(char a)
    {
        if (a == begin) {
            std::cout << a;
            return;
        }
        printPath(shortestPaths[a].first);
        std::cout << a;
    }

public:

    // реализация алгоритма построения кратчайшего пути методом А*
    void solve()
    {
        std::vector <Vertex> parallelVertexes; // массив вершин, снятых за один шаг
        std::priority_queue <Vertex, std::vector<Vertex>, Cmp> checkList; // список вершин, которые нужно рассмотреть, в начале очереди находятся вершины с самым низким приоритетом

        checkList.push(Vertex{begin, 0 + double(end - begin)});
        while (!checkList.empty() && checkList.top().name != end) { // цикл закончится, как только опустеет очередь или будет достигнута конечная вершина
#ifdef DEBUG
            for (auto &it : shortestPaths) {
                std::cout << "mW[" << it.first << "]: ";
                printPath(it.first);
                std::cout << ' ';
            }
            std::cout << std::endl;
#endif
            // вершины извлекаются пока мы не пройдем их все или очередь не опустеет
            for (int i = 0; i < nParallel && !checkList.empty(); i++) {
                Vertex v = checkList.top();
                if (v.name == end)
                    continue;
                parallelVertexes.push_back(v);
                checkList.pop();
            }
            for (auto curVertex : parallelVertexes) { // рассмотрение всех извлеченных вершин
                viewedVertexes[curVertex.name] = true;
                for (int j = 0; j < graph[curVertex.name].size(); j++) { // все смежные вершины
                    std::pair<char, double> newVertex = graph[curVertex.name][j];
                    if (viewedVertexes[newVertex.first]) // если вершина уже была рассмотрена, то она пропускается
                        continue;
                    if (shortestPaths[newVertex.first].second == 0 || shortestPaths[newVertex.first].second > shortestPaths[curVertex.name].second + newVertex.second) {
                        // если вершина еще не была рассмотрена или новый найденный путь короче,
                        // то эта врешина добавляется в очередь и запоминается новый кратчайший путь до нее
                        shortestPaths[newVertex.first].second = shortestPaths[curVertex.name].second + newVertex.second;
                        shortestPaths[newVertex.first].first = curVertex.name;
                        checkList.push(Vertex{newVertex.first, shortestPaths[newVertex.first].second + double(end - newVertex.first)}); // добавление в список для рассмотрения
                    }
                }
            }
            parallelVertexes.clear();
        }
        std::cout << "Решение методом А*: ";
        printPath(end);
    }

    // считывает граф
    void readGraph()
    {
        char start, finish;
        double path;
        std::cout << "Введите количество вершин, извлекаемых из очереди за один шаг: ";
        std::cin >> nParallel;
        std::cout << "Введите граф (символ '*' - окончание данных): " << std::endl;
        std::cin >> begin >> end;
        while (std::cin >> start) {
            if (start == false || start == '*' || start == '!')
                break;
            std::cin >> finish >> path;
            graph[start].push_back(std::make_pair(finish, path));
        }
    }

};

int main()
{
    std::cout << "=== Построение кратчайшего пути в ориентированном графе ===" << std::endl;
    auto *path = new Asterisk();
    path->readGraph();
    path->solve();
    delete path;
    return 0;
}