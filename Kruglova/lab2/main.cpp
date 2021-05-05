#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <cstring>

using std::map;
using std::set;
using std::pair;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::list;

#define INF 10000000000.0

typedef struct Triple
{
    // структура, хранящая информацию о ребре и было ли оно пройдено
    char name;
    double weight;
    mutable bool flag;
    Triple() {}
    Triple(char _name, double _weight, bool _flag=false) : name(_name), weight(_weight), flag(_flag) {}
} Triple;

struct SetCompare
{
    bool operator()(Triple v1, Triple v2)
    {
        if (v1.weight == v2.weight)
            return v1.name < v2.name;
        return  v1.weight < v2.weight;
    }
};

class Graph
{
public:
    // point хранит зависимость вершин в виде: вершина - массив смежных вершин
    // Массив смежных вершин отсортирован по возрастанию веса ребра (SetCompare)
    map<char, set<Triple, SetCompare>> point;
    char start, end;
public:
    void init();
    void print_graph();
    std::stack<char> greedySearch();
    std::stack<char> aStar();
    std::stack<char> dijkstra();
    //вспомогающие
    int heuristic(char);
    std::stack<char> reconstruction(map<char, char>);

};

void Graph::init()
/* Читаем start, end. После заполняем массив зависимостей */
{
    string input;
    //cout << "Enter start and end point: ";
    getline(cin, input);
    start = input[0];
    end   = input[2];

    //cout << "Enter adjacency list:" << endl;
    while (getline(cin, input))
    {
        if (input.empty()) break;
        point[input[0]].emplace(input[2], std::stod(input.substr(4)));
    }
}

void Graph::print_graph()
{
    for (auto var : point)
    {
        cout << var.first << ": ";
        for (auto var2 : var.second)
            cout << var2.name << " " << var2.weight << " " << var2.flag << "; ";
        cout << std::endl;
    }
}

std::stack<char> Graph::greedySearch()
{
    // В стеке храним результат. Сразу записываем первую вершину
    // curr хранит массив смежных вершин к текущей вершине 
    std::stack<char> res;
    res.push(start);

    set<Triple, SetCompare> curr = point[res.top()];

    while (!res.empty() && res.top() != end)
    {
        bool can_go = false;
        char tmp;
        if (!curr.empty())
        {
            for (auto &var : point[res.top()]) //point[res.top()] == curr. Сделано для того, чтобы флаг изменялся
            // Ищем следующую непосещённую вершину
            {
                if (!var.flag)
                {
                    can_go = true;
                    var.flag = true;
                    tmp = var.name;
                    break;
                }
            }
        }

        if (can_go)
        {
            res.push(tmp);
            curr = point[tmp];
        } else {
            res.pop();
            if (!res.empty()) curr = point[res.top()];
        }
    }

    //зануляем флаг, чтобы не портить массив
    for (auto &var: point)
        for (auto &var2: var.second)
            var2.flag = false;

    return res;
}

void expand_stack(std::stack<char>& res)
{
    std::stack<char> tmp;
    tmp.swap(res);
    while (!tmp.empty())
    {
        res.push(tmp.top());
        tmp.pop();
    }
}

void print_stack(std::stack<char> res)
{
    while (!res.empty())
    {
        cout << res.top();
        res.pop();
    }
    cout << endl;
}

int Graph::heuristic(char curr)
{
    return abs(end - curr);
}

char minF(list <char> open, map <char, float> F){//поиск минимального значения f(x)
    char res = open.back();
    float min = F[res];

    cout << "Open list: ";
    for (auto var : open)
    {
        cout << var << " - " << F[var] << ";";
        if (F[var] <= min){
            res = var;
            min = F[var];
        }
    }
    cout << endl << "Selected vertex " << res << " - " << F[res] << endl; 
    return res;
}

bool inList(list<char> _list, char x)
{
    for (auto var : _list)
        if (var == x) return true;
    return false;
}

std::stack<char> Graph::reconstruction(map<char, char> from)
{
    std::stack<char> res;
    char curr = end;
    while (curr != start)
    {
        res.push(curr);
        curr = from[curr];
    }
    res.push(start);
    return res;
}

std::stack<char> Graph::aStar()
{
    std::stack<char> res; //стек результата
    list<char> close; //список пройденных вершин
    list<char> open = {start}; //список рассматриваемых вершин
    map<char, char> from; //карта пути
    map <char, float> G; //хранит стоимости путей от начальной вершины
    map <char, float> F; //оценки f(x) для каждой вершины
    G[start] = 0;
    F[start] = G[start] + heuristic(start);

    while (!open.empty())
    {
        cout << "Close list: ";
        for (auto var : close)
            cout << var << " ";
        cout << endl;

        char curr = minF(open, F);

        if (curr == end)
        {
            cout << "Path found!" << endl;
            res = reconstruction(from);//востанавливаем
            return res;
        }

        open.remove(curr);
        close.push_back(curr);

        for (auto neighbor : point[curr])
        {
            // if (inList(close, neighbor.name)) //если уже проходили, дальшше
            //     continue;
            
            float tmpG = G[curr] + neighbor.weight; //вычисление g(x) для обрабатываемого соседа

            if (inList(close, neighbor.name) && tmpG >= G[neighbor.name])
            {
                continue;
            }
            
            if (!inList(open, neighbor.name) || tmpG < G[neighbor.name])
            {
                from[neighbor.name] = curr;
                G[neighbor.name] = tmpG;
                F[neighbor.name] = G[neighbor.name] + heuristic(neighbor.name);
            }

            if (!inList(open, neighbor.name))
                open.push_back(neighbor.name);
        }
    }

    return res;
}

const char find_min_vertex(list<char> open, map<char,float> G)
{
    double min = INF;
    char ret;
    for (auto var : open)
    {
        if (G[var] < min)
        {
            min = G[var];
            ret = var;
        }
    }

    return ret;
}

std::stack<char> Graph::dijkstra()
{
    std::stack<char> res; //стек результата
    list<char> close = {start}; //список пройденных вершин
    list<char> open; //список рассматриваемых вершин
    map<char, char> from; //карта пути
    map <char, float> G; //хранит стоимости путей от начальной вершины
    G[start] = 0;
    for (auto var : point) {
        for (auto var2 : var.second)
        {
            if (inList(open, var2.name)) continue;
            open.push_back(var2.name);
            G[var2.name] = INF;
        }
    }
    open.remove(start);

    for (auto var : point[start])
    {
        G[var.name] = var.weight;
        from[var.name] = start;
    }

    cout << "***Info***" << endl;
    cout << "Initialization" << endl;
    cout << "Open list: ";
    for (auto var : open)
        cout << var << " ";
    cout << endl;
    cout << "Close list: ";
    for (auto var : close)
        cout << var << " ";
    cout << endl;
    cout << "Map: " << endl;
    for (auto var : from)
        cout << "to " << var.first << " from " << var.second << endl;
    cout << "Initialization complete" << endl;

    while (!open.empty())
    {
        char curr = find_min_vertex(open, G);
        cout << "Current vertex - " << curr << endl;

        close.push_back(curr);
        open.remove(curr);
        cout << "Open list: ";
        for (auto var : open)
            cout << var << " ";
        cout << endl;
        cout << "Close list: ";
        for (auto var : close)
            cout << var << " ";
        cout << endl;

        for (auto var : point[curr])
        {
            if (G[curr]+var.weight < G[var.name])
            {
                cout << "The path for the vertex " << var.name << " is recalculated. "
                    << "Old value = " << G[var.name] << ". New value = " << G[curr]+var.weight
                    << "." << endl;
                G[var.name] = G[curr]+var.weight;
                from[var.name] = curr;
            }
        }
        cout << "Map: " << endl;
        for (auto var : from)
            cout << "to " << var.first << " from " << var.second << endl;
    }

    cout << "The algorithm has finished its work. Reconstruction path.." << endl;

    return reconstruction(from);
}

int main(int argc, char** argv)
{
    Graph one;
    one.init();
    std::stack<char> res;
    
    if (argc == 2)
    {
        if (!strcmp(argv[1], "-greed\0") || !strcmp(argv[1], "-g\0"))
        {
            res = one.greedySearch();
            expand_stack(res);
            cout << "GreedySearch answer: ";
            print_stack(res);
        }
        if (!strcmp(argv[1], "-astar\0") || !strcmp(argv[1], "-as\0"))
        {
            res = one.aStar();
            cout << "aStarSearch answer: ";
            print_stack(res);
        }
        if (!strcmp(argv[1], "-dijkstra\0") || !strcmp(argv[1], "-d\0"))
        {
            res = one.dijkstra();
            cout << "Dijkstra answer: ";
            print_stack(res);
        }
    }

    return 0;
}
