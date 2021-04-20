#include <stack>
#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <functional>
#include <queue>
#include <string>
#include <map>

#define INFO


class Edge;
//вершина графа
class Vertex {
public:

    char name;
    Vertex(char name) :name(name) {}
    Vertex() {}

    bool operator==(Vertex v2) {
        return this->name == v2.name;
    }

    friend bool operator<(Vertex v1, Vertex v2) {
        return v1.name < v2.name;
    }
};


//ребро графа
class Edge {
public:
    Vertex v1;
    Vertex v2;
    int flow = 0;
    int capacity;
    int originalCapacity;
    Edge* reverseEdge;
    bool hasReverse = false;
    bool isReverse = false;
    void addReverseEdge(Edge* e) {
        this->reverseEdge = e;
        this->hasReverse = true;
        if (e != nullptr) {
            e->reverseEdge = this;
            e->hasReverse = true;
        }
    }
    Edge() {}
    Edge(Vertex v1, Vertex v2, int capacity, int flow = 0) :
        v1(v1), v2(v2), capacity(capacity), flow(flow) {
        originalCapacity = capacity;

    }
    friend std::ostream& operator<< (std::ostream& out, Edge e) {
        out << "(" << e.v1.name << "," << e.v2.name << "," << e.capacity << ") ";
        return out;
    }
    friend bool operator< (Edge e1, Edge e2) {
        if (e1.v1.name < e2.v1.name)
            return true;

        else if (e1.v1.name == e2.v1.name)
            return (e1.v2.name < e2.v2.name);

        return false;
    }
    bool operator==(Edge e) {
        return this->v1.name == e.v1.name && this->v2.name == e.v2.name;
    }

};

template<typename T>
void print_queue(T q) {
    while (!q.empty()) {
        std::cout << *q.top();
        q.pop();
    }
    std::cout << '\n';
}


template <typename T, typename B>
bool isInVector(T vec, B e) {
    for (auto it : vec)
        if (it == e)return true;
    return false;
}

//граф
class Graph {
public:
    std::vector<Vertex> vertexVector;
    std::vector<Edge> edgeVector;

    bool isInGraph(char verName) {
        for (auto it : vertexVector)
            if (it.name == verName)
                return true;
        return false;
    }

    Vertex* operator()(char verName) {
        for (auto& it : vertexVector) {
            if (it.name == verName)
                return &it;
        }
        return nullptr;
    }
    Edge* operator()(char verName1, char verName2) {
        for (auto& it : edgeVector) {
            if (it.v1.name == verName1 && it.v2.name == verName2)
                return &it;
        }
        return nullptr;
    }

        //вывод ребер
        void printEdges() {
            std::sort(edgeVector.begin(), edgeVector.end());
            for (auto it : edgeVector) {

                if (it.flow < 0)it.flow = 0;
                std::cout << it.v1.name << " " << it.v2.name << " " << it.flow << "\n";
            }
        }

        //добавление вершины
        void addVertexByName(char verName) {
            if (!isInGraph(verName))
                vertexVector.push_back(Vertex(verName));
        }

        //добавление ребра
        void addEdge(char v1, char v2, int cap, int flow = 0) {

            Edge* addedEdge = new Edge(v1, v2, cap);
            if (!isInVector(edgeVector, Edge(v1, v2, cap))) {

                edgeVector.push_back(*addedEdge);

            }
        }

        char root, goal;

        //ввод
        bool input() {
            char verName1;
            char verName2;
            int capacity;
#ifdef FILEINPUT
            std::cout << "Ввод:\n";

            while (std::getline(file, tmp)) {
                std::cout << tmp << "\n";
            }
            std::cout << "\n";
            file.close();
#endif

            //считывание списка ребер
            int n;
            bool inputSuccess = true;
            std::cin >> n >> root >> goal;
            if (n < 0) inputSuccess = false;

            for (int i = 0; i < n; i++) {
                std::cin >> verName1 >> verName2 >> capacity;
                if (capacity < 0) inputSuccess = false;
                addVertexByName(verName1);
                addVertexByName(verName2);
                addEdge(verName1, verName2, capacity);
            }

            //добавление обратных ребер
            for (auto& it : edgeVector) {
                it.addReverseEdge((*this)(it.v2.name, it.v1.name));
                if (it.reverseEdge == nullptr) {
                    it.addReverseEdge(new Edge(it.v2.name, it.v1.name, 0, it.capacity));
                }
            }
            return inputSuccess;
        }
    };

    Edge findMaxCapacityEdge(Graph graph, Vertex v1, std::vector<Vertex> blocked) {

        //ребро, которое возвращается, если пути нет
        Edge max = Edge('_', '-', -200);
        bool reverse = false;

        //лямбда функция проверки, подходит ли ребро для продолжения пути
        auto lambda = [&](Edge it, bool reverse) {
            bool found = false;
#ifdef INFO
            if (it.v1 == v1) {
                std::cout << "Вершина " << it.v2.name << " дуга ";
                if (reverse) std::cout << "\"обратная\" ";
                std::cout << it;
            }
#endif
            if (it.v1 == v1 && max.capacity < it.capacity && it.capacity > 0) {

                bool has = isInVector(blocked, it.v2); //если вершина не содержится среди посещенных
                if (!has) {
                    max = it;
                    max.isReverse = reverse;
                }
                else {
#ifdef INFO 
                    std::cout << "(уже посещена)";
#endif      
                }
            }
#ifdef INFO
            if (it.v1 == v1) std::cout << "\n";
#endif
        };
        //-------

        //проверка для обычных ребер
        for (const auto& it : graph.edgeVector) {
            lambda(it, 0);
        }

        //для "обратных"
        for (const auto& it : graph.edgeVector) {
            auto it2 = *graph(it.v1.name, it.v2.name)->reverseEdge;
            lambda(it2, 1);
        }
        return max;
    }
    std::vector<Edge> findPath(Graph graph) {


        std::vector<Edge> path;
        std::vector<Vertex> block;
        std::vector<Vertex> pathV;
        pathV.push_back(*graph(graph.root));

#ifdef INFO
        std::cout << "\n\nПоиск пути \n";
#endif
        Vertex current = *graph(graph.root);

        //основной цикл
        while (!pathV.empty()) {

#ifdef INFO
            std::cout << "\nСоседи вершины " << current.name << ":\n";

#endif
            //нахождение макисмальной дуги
            Edge nextEdge = findMaxCapacityEdge(graph, current, block);

            //если нашлась продолжение пути
            if (nextEdge.capacity != -200) {
#ifdef INFO
                std::cout << "Переход по дуге " << nextEdge << "\n";
#endif
                current = *graph(nextEdge.v2.name);
                path.push_back(nextEdge);
                pathV.push_back(current);
                if (nextEdge.v2.name == graph.goal) break;
                block.push_back(current);
            }
            //если нет, возвращение к последней посещенной вершине
            else {
#ifdef INFO
                std::cout << "Нет доступных путей из вершины " << current.name << "\nВозврат к предыдущей вершине\n";
#endif
                block.push_back(pathV.back());
                pathV.pop_back();

                if (!path.empty()) path.pop_back();

                if (pathV.empty()) {
#ifdef INFO
                    std::cout << "\nПути не существует\nЗавершение алгоритма\n\n";
#endif
                    return path;
                }
                else current = *graph(pathV.back().name); //когда ребер в пути нет, но есть одна вершина - начальная
            }
            //----------
        }

        //вывод
#ifdef INFO
        std::cout << "\nНайденный путь: ";
        for (auto it : pathV)
            std::cout << it.name;
        std::cout << "\n";
#endif

        return path;
    }

    void findMaxFlow(Graph graph) {

        Graph rGraph(graph); //остаточная сеть
        int maxflow = 0;
        Edge* currEdge;
        std::vector<Edge> currentPath = findPath(rGraph);

        while (currentPath.size() != 0) {
            int min = currentPath.front().capacity;
            //вычисление минимальной пропускной способности дуги в найденном пути
            for (const auto& pathEdge : currentPath) {

                int tmpCap = pathEdge.capacity;
                min = (tmpCap < min) ? tmpCap : min;
            }
            //------
#ifdef INFO
            std::cout << "Минимальная пропускная спосбность: " << min << "\n";
#endif
            //обновление потоков и пропускных спобностей ребер, участвуюших в пути
            for (auto pathEdge : currentPath) {

                //если ребро в пути не является обратным
                currEdge = rGraph(pathEdge.v1.name, pathEdge.v2.name);

                //если является, то его надо взять как обратное от ребра с противоположными вершинами
                if (currEdge == nullptr) {
                    currEdge = rGraph(pathEdge.v2.name, pathEdge.v1.name)->reverseEdge;
                }

#ifdef INFO
                std::cout << "\nПропускная способность: " << *currEdge << " изменена с " << currEdge->capacity;
                std::cout << " на " << currEdge->capacity - min << "\n";
                std::cout << "Пропускная спосбность \"обратного\" ребра: " << *currEdge->reverseEdge << " изменена с " << currEdge->reverseEdge->capacity;
                std::cout << " на " << currEdge->reverseEdge->capacity + min << "\n";

                std::cout << "Поток по дуге: " << *currEdge << " изменен с " << currEdge->flow;
                std::cout << " на " << currEdge->flow + min << "\n";
                std::cout << "Поток по \"обратной\" дуге: " << *currEdge->reverseEdge << " изменен с " << currEdge->reverseEdge->flow;
                std::cout << " на " << currEdge->reverseEdge->flow - min << "\n";
#endif 

                //обновление потоков и пропускных способностей
                currEdge->flow += min;
                currEdge->capacity -= min;
                Edge* reverseOfCurr = rGraph(currEdge->v2.name, currEdge->v1.name);

                if (reverseOfCurr != nullptr) {
                    reverseOfCurr->reverseEdge = currEdge;
                    currEdge->reverseEdge = reverseOfCurr;
                }

                currEdge->reverseEdge->flow -= min;
                currEdge->reverseEdge->capacity += min;
            }
            maxflow += min;
            currentPath = findPath(rGraph);
        }

        std::cout << maxflow << "\n";
        rGraph.printEdges();
    }

    int main()
    {
        Graph graph;
        setlocale(LC_ALL, "rus");
        bool input = graph.input();
        if (!input) {
            std::cout << "Некорректный ввод\n";
            exit(-1);
        }
        std::cout << "\nПоиск максимального потока. "<<graph.root << " - источник, " << graph.goal << " - сток";
        findMaxFlow(graph);
        return 0;
    }
