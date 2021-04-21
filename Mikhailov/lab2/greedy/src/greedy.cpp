/**
 * Решение задачи построения пути в ориентированном графе при помощи жадного алгоритма
 */

#include <iostream>
#include <map>
#include <vector>
#include <stack>

#define DEBUG

class Greedy {

    char begin{}, end{}; // начальная и конечная вершина
    // в контейнере графа хранятся значения таким образом, что по ключу можно получить все вершины, в которые можно попасть из вершины-ключа
    std::map<char, std::vector<std::pair<char, double>>> graph;
    std::map<char, bool> visited; // список всех пройденных вершин

    // выводит путь от начальной вершины в конечную, рекурсивно раскручивая стек
    void printPath(std::stack<char>& result)
    {
        if (result.empty())
            return;
        char res = result.top();
        result.pop();
        printPath(result);
        std::cout << res;
    }

public:

    // реализация алгоритма жадного поиска
    void solve()
    {
        std::stack<char> path; // стек, в котором хранится путь до текущей вершины
        std::stack<char> debugOutput; // для вывода информации в режиме отладки
        path.push(begin);
        char curVertex = path.top();
        // цикл выполняется пока на вершине стека не окажется конечная вершина графа или не будет обойден весь граф
        do {
#ifdef DEBUG
            debugOutput = path;
            printPath(debugOutput);
            std::cout << "\n";
#endif
            bool isPath = false; // флаг - имеются ли пути в другие еще не пройденные вершины из текущей
            char nextVertex;
            double minDistance;
            // проверка - существуют ли пути, если нет, вершина помечается как пройденная
            if (graph[curVertex].empty()) {
                visited[curVertex] = true;
                path.pop();
                curVertex = path.top();
                continue;
            }
            // проверка - есть ли еще непройденные вершины
            for (auto & i : graph[curVertex]) {
                if (!visited[i.first]) {
                    isPath = true;
                    nextVertex = i.first;
                    minDistance = i.second;
                    break;
                }
            }
            // если все вершины пройдены, то текущая помечается как посещенная
            if (!isPath) {
                visited[curVertex] = true;
                path.pop();
                curVertex = path.top();
                continue;
            }
            // поиск минимального ребра
            for (auto & i : graph[curVertex]) {
                if (!visited[i.first] && minDistance > i.second) {
                    nextVertex = i.first;
                    minDistance = i.second;
                }
            }
            path.push(nextVertex); // переход к вершине, до которой был найден кратчайший путь
            curVertex = path.top();
        } while (curVertex != end);
        std::cout << "Решение методом жадного поиска: ";
        printPath(path);
    }

    // считывает граф и помечает все вершины как непройденные
    void readGraph()
    {
        char start, finish;
        double distance;
        std::cout << "Введите граф (символ '*' - окончание данных): " << std::endl;
        std::cin >> begin >> end;
        while (std::cin >> start) {
            if (start == false || start == '*' || start == '!')
                break;
            std::cin >> finish >> distance;
            graph[start].push_back(std::make_pair(finish, distance));
            graph[finish];
            visited[start] = false;
            visited[finish] = false;
        }
    }

};


int main()
{
    std::cout << "=== Построение кратчайшего пути в ориентированном графе ===" << std::endl;
    auto *path = new Greedy();
    path->readGraph();
    path->solve();
    delete path;
    return 0;
}