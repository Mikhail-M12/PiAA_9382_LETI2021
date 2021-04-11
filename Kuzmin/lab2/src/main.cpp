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
#define debugdetails 0
#define length 0 //для вывода длины пути

//вершина графа
class Vertex {
public:
    std::vector<std::pair<float, Vertex>> neighbours;
    char name;
    Vertex(char name) :name(name) {
        g = 10000;
        f = 10000;
        camefrom = nullptr;
    }
    Vertex() {}
    void addNeighbour(float distance, Vertex v) {
        neighbours.push_back(std::pair<float, Vertex>(distance, v));
    }
    float g;
    float f;
    Vertex* camefrom;
    friend bool operator==(Vertex v1, Vertex v2) {
        return v1.name == v2.name;
    }
    friend std::ostream& operator<< (std::ostream& out, Vertex v1) {
        out << v1.name << "(" << v1.f << ") ";
        return out;
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

template <typename T>
bool isInQueue(T q, Vertex element) {
    while (!q.empty()) {
        if (q.top()->name == element.name) return true;
        q.pop();
    }
    return false;
}
template <typename T>
bool isInVector(T vec, Vertex v) {
    for (auto it : vec)
        if (it.name == v.name)return true;
    return false;
}
//граф
class Graph {
public:
    std::vector<Vertex> vertexVector;

    bool isInGraph(char verName){
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

    void erase(Vertex v) {
        for (auto it = vertexVector.begin(); it != vertexVector.end(); ) {
            if (it->name == v.name) {
                it = vertexVector.erase(it);
            }
            else {
                ++it;
            }
        }
    }
    void addVertexByName(char verName) {
        if (!isInGraph(verName))
            vertexVector.push_back(Vertex(verName));
    }

    char root, goal;
    char goal2 = '-';

    //ввод
    void input() {
        char verName1;
        char verName2;
        float distance;
        std::ifstream file("tests/input.txt");
        std::cout << "Ввод:\n";
        std::string tmp;
        while (std::getline(file, tmp)) {
            std::cout << tmp << "\n";
        }
        std::cout << "\n";
        file.close();
        file.open("tests/input.txt");
       
        std::getline(file, tmp);
        root = tmp[0];
        goal = tmp[2];
        goal2 = tmp[tmp.size() - 1];

        while (file >> verName1 >> verName2 >> distance) {
            addVertexByName(verName1);
            addVertexByName(verName2);
            (*this)(verName1)->addNeighbour(distance, *(*this)(verName2));
        }
    }
};

//возвращает пару расстояние + вершина для вершины v 
std::pair<float, Vertex> getMin(Vertex v, std::vector<Vertex> blocked) {

    std::pair<float, Vertex> min = std::pair<float, Vertex>(100000, Vertex('-'));
    for (auto it : v.neighbours) {
        if (it.first < min.first) {
            bool breakflag = false;
            for (auto it2 : blocked) {
                if (it2.name == it.second.name) {
                    breakflag = true;
                    break;
                }
            }
            if (!breakflag)
                min = it;
        }
    }
    return  min;
}
//эвристическая функция для A*
int heuristic(Vertex v1, Vertex v2, Vertex v3) {

    int a1 = abs(v1.name - v2.name);
    int a2 = abs(v1.name - v3.name);
    if (a1 < a2) return a1;
    else return a2;
}
//алгоритм A*
void aStar(Graph graph, char name1, char name2, char name3) {

    //очередь с приоритетом для просмотра вершин
    auto cmp = [&](Vertex* v1, Vertex* v2) {
        return v1->f > v2->f;
    };
    std::priority_queue<Vertex*, std::vector<Vertex*>, decltype(cmp)> opened(cmp);

    std::vector<Vertex> closed;

    std::deque<Vertex> path;

    Vertex v1_name;
    Vertex v2_name;

    if (graph(name2) && graph(name3)) {
        v1_name = *graph(name2);
        v2_name = *graph(name3);
    }
    else if (graph(name2)) {
        v1_name = *graph(name2);
        v2_name = v1_name;

    }
    else if (graph(name3)) {
        v2_name = *graph(name3);
        v1_name = v2_name;
    }
    else {
        std::cout << "\nПути не существует\n";
        return;
    }

    bool finish1 = false;
    bool finish2 = false;

    graph(name1)->g = 0;
    graph(name1)->f = heuristic(*graph(name1), v1_name, v2_name);

#ifdef debugdetails
    std::cout << "\nНачальная вершина " << *graph(name1) << " добавлена в очередь";
#endif 

    opened.push(graph(name1));
    //основной цикл
    while (!opened.empty()) {
        Vertex* current = opened.top();

#ifdef debugdetails
        std::cout << "\n\nТекущее состояние очереди: ";
        print_queue(opened);
        std::cout << "Вершина " << current->name << " извлечена из очереди\n";
        std::cout << "Просмотр вершины " << current->name << "\n";
#endif
        finish1 = current->name == v1_name.name;
        finish2 = current->name == v2_name.name;

        if (finish1 || finish2) {
#ifdef debugdetails
            std::cout << "Вершина  " << current->name << " является конечной. Завершение алгоритма\n";
#endif
            break;
        }

        opened.pop();
        closed.push_back(*current);

        for (auto neighbour : current->neighbours) {

            Vertex* it = graph(neighbour.second.name);
            float tmp = current->g + neighbour.first;

#ifdef debugdetails
            std::cout << "\nПросмотр соседа  " << current->name << ": " << it->name << std::endl;
            std::cout << "Новое расстояние: " << tmp << std::endl;
            std::cout << "Уже известное расстояние g до данного соседа: " << it->g << std::endl;
#endif
            //если путь через текущую вершину до соседа больше чем уже уже имеющейся
            if (tmp >= it->g && isInVector(closed, *it)) {

#ifdef debugdetails
                std::cout << "Вершина " << *it << " уже была обработана. Расстояние до нее нельзя улучшить";
#endif
                continue;
            }
            //обновление пути до соседа
            else if (tmp < it->g) {
#ifdef debugdetails
                std::cout << "Создан новый путь к " << *it << " через " << current->name << ": " << tmp << "\n";
#endif
                float s = it->f;
                it->camefrom = graph(current->name);
                it->g = tmp;
                it->f = tmp + heuristic(*it, v1_name, v2_name);

#ifdef debugdetails
                std::cout << "Обновление f " << s << " -> " << it->f << "\n";
#endif
            }
            //добавление вершины в очередь
            if (!isInQueue(opened, *it)) {
#ifdef debugdetails
                std::cout << "Вершина " << *it << " добавлена в очередь\n";
#endif
                opened.push(it);
            }
            //обновление очереди
            else {
#ifdef debugdetails
                std::cout << "Вершина " << it->name << " уже находится в очереди. Обновление очереди\n";
#endif
                std::deque<Vertex*> tmp_q;
                while (opened.top()->name != it->name) {
                    tmp_q.push_back(opened.top());
                    opened.pop();
                }
                tmp_q.push_back(opened.top());
                opened.pop();

                while (!tmp_q.empty()) {
                    opened.push(tmp_q.back());
                    tmp_q.pop_back();
                }
            }
        }
    }

    if (!finish1 && !finish2) {
        std::cout << "\nПути не существует\n";
        return;
    }

    //вывод
    Vertex t = (finish1) ? *graph(name2) : *graph(name3);

    float distance = t.g;
    path.push_front(t);
    while (t.name != name1) {

        t = *(t.camefrom);
        path.push_front(t);
    }
    float count = 0;

    std::cout << "\nНайденный путь: ";

    for (auto it : path) {
        std::cout << it.name;
    }

#ifdef length
    std::cout << "\nдлина: " << distance;
#endif
}

//функция, реализующая жадный алгоритм
void greedy(Graph graph, char name1, char name2) {

    std::vector<Vertex> path;
    std::vector<Vertex> block;
    float count = 0;
    float lastdistance = 0;

    path.push_back(*graph(name1));

#ifdef debugdetails
    std::cout << "\n\nПереход к начальной вершине " << name1 << "\n";
#endif

    //основной цикл
    while (path.back().name != name2) {

        std::pair<float, Vertex> nextVertex = getMin(path.back(), block);

#ifdef debugdetails
        std::cout << "\nСоседи вершины " << path.back().name << ":\n";
        for (auto it : path.back().neighbours) {
            std::cout << "Вершина " << it.second.name << ", расстояние " << it.first;
            for (auto it2 : block) {
                if (it.second.name == it2.name) {
                    std::cout << " (уже посещена)";
                    break;
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
#endif
        //если найден путь из данной вершины
        if (nextVertex.second.name != '-') {
#ifdef debugdetails
            std::cout << "Переход к вершине " << nextVertex.second.name << "\n";
#endif
            count += nextVertex.first;
            path.push_back(*graph(nextVertex.second.name));
            block.push_back(path.back());
            lastdistance = nextVertex.first;
    }
       //если не найден, переход к последней
        else {
#ifdef debugdetails
            std::cout << "Нет доступных путей из вершины " << path.back().name << "\nВозврат к предыдущей вершине ";
#endif
            block.push_back(path.back());
            path.pop_back();
            if (path.empty()) {
                std::cout << "\nПути не существует\n";
                return;
            }
#ifdef debugdetails
            std::cout << path.back().name << "\n";
#endif
            count -= lastdistance;
            }
}
    //вывод

    std::cout << "\nНайденный путь: ";

    for (auto it : path)
        std::cout << it.name;
#ifdef length
    std::cout << "\nдлина: " << count << "\n";
#endif
}

int main()
{
    Graph graph;
    setlocale(LC_ALL, "rus");
    graph.input();

    std::cout << "\nЖадный алгоритм \nНачало - " << graph.root << ", конец - " << graph.goal;

    greedy(graph, graph.root, graph.goal);

    std::cout << "\nA* \nНачало - " << graph.root << ", конец - " << graph.goal;
    if (graph.goal2 != graph.goal) std::cout << " или " << graph.goal2;
#ifdef debugdetails
    std::cout << "\nВ скобках указывается значение f вершины\n";
#endif
    aStar(graph, graph.root, graph.goal, graph.goal2);
    std::cout << "\n";
    return 0;
}

