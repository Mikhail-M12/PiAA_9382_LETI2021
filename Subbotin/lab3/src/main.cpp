#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <vector>

#define DEBUG

class FordFulkerson {
public:
    FordFulkerson();
    bool bfs();
    void doFulkerson();
    std::vector<std::pair<char, int>> getAdjacentVertices(char u);

private:
    char s;
    char t;
    std::map<char, char> parent;
    std::map<char, std::map<char, int>> graph;
    std::map<char, std::map<char, int>> residualGraph;
};

std::vector<std::pair<char, int>> FordFulkerson::getAdjacentVertices(char u) {
    std::vector<std::pair<char, int>> edges;
    for (auto edge : residualGraph[u]) {
        edges.emplace_back(std::make_pair(edge.first, edge.second));
    }
    return edges;
}

bool FordFulkerson::bfs() {
#ifdef DEBUG
    std::cout << std::endl
              << "Ищем путь от s к t с помощью алгоритма поиска в ширину: " << std::endl;
#endif
    std::queue<char> verticesToExplore;
    verticesToExplore.push(s);
    std::map<char, bool> visited;
    visited[s] = true;

    //пока есть вершины, которые хотим посетить
    while (!verticesToExplore.empty()) {
        char u = verticesToExplore.front();
        verticesToExplore.pop();
        std::vector<std::pair<char, int>> edges = getAdjacentVertices(u);
#ifdef DEBUG
        std::cout << "Достаем вершину " << u << " из очереди и проходимся по ее сооседям: " << std::endl;
        for (auto edge : edges) {
            std::cout << edge.first << " ";
        }
        std::cout << std::endl;
#endif
        //смотрим смежные вершины с текущей
        for (auto edge : edges) {
#ifdef DEBUG
            std::cout << "Соседняя вершина " << edge.first << " "
                      << "ребро с ней имеет capacity "
                      << edge.second << ", ";
            visited[edge.first] ? std::cout << "вершина посещалась ранее" : std::cout << "вершина не посещалась ранее";
            std::cout << std::endl;

#endif
            //если capacity > 0 и вершина еще не посещалась
            if (edge.second > 0 && !visited[edge.first]) {
                //записываем ее в очередь, чтобы потом пройтись
                verticesToExplore.push(edge.first);
                //записываем в map для восстановления пути
                parent[edge.first] = u;
                //помечаем посещенной
                visited[edge.first] = true;
#ifdef DEBUG
                std::cout << "Записываем вершину в очередь и в путь" << std::endl;
#endif

                //если же достигли конечной вершины -- путь найден, заканчиваем bfs
                if (edge.first == t) {
#ifdef DEBUG
                    std::cout << "Достигли конечной вершины, путь найден, завершаем поиск." << std::endl
                              << std::endl;
#endif
                    return true;
                }
            }
        }
    }
    return false;
}

void FordFulkerson::doFulkerson() {
    char u, v;
    residualGraph = graph;

    int max_flow = 0;


    //пока есть путь от s к t
    while (bfs()) {
        int path_flow = std::numeric_limits<int>::max();

#ifdef DEBUG
        std::cout << "Ищем минимальный capacity среди ребер из пути: " << std::endl;
#endif
        //проходимся по найденному пути и ищем ребро с минимальным capacity
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
#ifdef DEBUG
            std::cout << '(' << u << ',' << v << ',' << residualGraph[u][v] << ')' << ' ';
#endif
            path_flow = std::min(path_flow, residualGraph[u][v]);
        }
#ifdef DEBUG
        std::cout << std::endl
                  << "Минимальный capacity: " << path_flow << std::endl;
        std::cout << "Проходимся по пути, изменяя текущие capacity ребер: " << std::endl;
#endif

        //проходимся по пути снова и отнимаем от каждого capacity минимальный
        //также для ребер в обратном направлении увеличиваем значение на тот же минимальный capacity
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            residualGraph[u][v] -= path_flow;
            residualGraph[v][u] += path_flow;
        }

#ifdef DEBUG

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            std::cout << '(' << u << ',' << v << ',' << residualGraph[u][v] << ')' << ' ';
        }
        std::cout << std::endl;

        std::cout << "И также меняем значение заполненности ребер для пути в обратном направлении: " << std::endl;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            std::cout << '(' << v << ',' << u << ',' << residualGraph[v][u] << ')' << ' ';
        }
        std::cout << std::endl;

#endif

        //инкрементируем общий поток
        max_flow += path_flow;
#ifdef DEBUG
        std::cout << "Текущий общий поток: " << max_flow << std::endl;
#endif
    }

#ifdef DEBUG
    std::cout << std::endl
              << "Доступных путей больше нет, завершаем алгоритм." << std::endl
              << std::endl;
#endif

    std::cout << max_flow << std::endl;
    for (auto const &vertex : graph) {
        for (auto const neighbor : graph[vertex.first]) {
            int flow = (neighbor.second - residualGraph[vertex.first][neighbor.first] < 0) ? 0 : neighbor.second - residualGraph[vertex.first][neighbor.first];
            std::cout << vertex.first << " " << neighbor.first << " " << flow << std::endl;
        }
    }
}

FordFulkerson::FordFulkerson() {
    int N;
    std::cin >> N;
    std::cin >> s >> t;
    for (int i = 0; i < N; i++) {
        char u, v;
        int capacity;
        std::cin >> u >> v >> capacity;
        graph[u][v] = capacity;
    }
}

int main() {
    FordFulkerson ford;
    ford.doFulkerson();
    return 0;
}
