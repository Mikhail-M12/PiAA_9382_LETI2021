#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;

#define INFO

struct Sorting { //функция сортировки для приоритетной очереди
    bool operator() (pair<char, double> a, pair<char, double> b) {
        //если стоимость двух вершин равна, то возвращается меньшая из них в алфавитном порядке, если стоимость разная, то большая из них
        if (a.second == b.second)
            return (a.first < b.first);
        else
            return (a.second > b.second);
    }
};

class FindingPath {
public:
    FindingPath() = default;
    vector<char> GreedyAlgorithm();
    vector<char> AStar();
    void Sort();
    void SortAStar();
    void Read();
    int Heuristic(char a, char b);
    void PrintQueue(priority_queue<pair<char, double>, vector<pair<char, double>>, Sorting>);
private:
    map<char, vector<pair<char, double>>> graph;
    map<char, bool> visited;
    char start;
    char end;
    int number;
};

int FindingPath::Heuristic(char a, char b) {
    return abs(a - b);
}

void FindingPath::PrintQueue(priority_queue<pair<char, double>, vector<pair<char, double>>, Sorting> queue)
{
    auto newQueue = queue;
    cout << "Приоритетная очередь: ";
    while (!newQueue.empty())
    {
        cout << newQueue.top().first << '(' << newQueue.top().second << ')';
        newQueue.pop();
    }
    cout << endl;
}



vector<char> FindingPath::AStar() {  //А*
    #ifdef INFO
    cout << "\nАлгоритм А*:\n";
    #endif
    map<char, pair<vector<char>, double>> ShortPathes;  //текущие кратчайшие пути
    vector<char> vertex;
    priority_queue < pair<char, double>, vector<pair<char, double>>, Sorting> PriorityQueue; //очередь в алгоритме

    PriorityQueue.push({ start, 0 });
    vertex.push_back(start);
    ShortPathes[start].first = vertex;

    while (!PriorityQueue.empty()) {  //пока очередь не пуста
        #ifdef INFO
        PrintQueue(PriorityQueue);
        #endif
        if (PriorityQueue.top().first == end) {  //если найдена конечная вершина
            #ifdef INFO
            cout << "В приоритетной очереди конечная вершина первая, следовательно, найден кратчайший маршрут!" << endl;
            #endif
            return ShortPathes[end].first;  //то заканчивается поиск
        }
        auto TmpVertex = PriorityQueue.top();  //достается приоритетная вершина из очереди
        #ifdef INFO
        cout << "Из приоритетной очереди удаляется вершина " << TmpVertex.first << TmpVertex.second << endl;
        cout << "Текущая вершина - " << TmpVertex.first << endl;
        #endif
        PriorityQueue.pop();

        if (graph[TmpVertex.first].empty())
        {
            #ifdef INFO
            cout << "   Смежных вершин нет" << endl;
            #endif
        }

        for (auto& i : graph[TmpVertex.first]) {  //рассматриваются все вершины, которые соединены с текущей вершиной
            #ifdef INFO
            cout << "   Рассматривается смежная для " << TmpVertex.first << " вершина "<< i.first << endl;
            #endif
            double CurLength = ShortPathes[TmpVertex.first].second + i.second;
            //if (!ShortPathes[i.first].second == 0)
            //    #ifdef INFO
            //    cout << "   Пути к следующей вершине нет" << endl;
            //    #endif
            //if (!ShortPathes[i.first].second > CurLength)
            //{
            //    #ifdef INFO
            //    cout << "   Путь от начала графа до конца через вершину " << i.first << " не оптимальный" << endl;
            //    #endif
            //}
            if (ShortPathes[i.first].second == 0 || ShortPathes[i.first].second > CurLength) { //если пути нет или найденный путь короче
                #ifdef INFO
                cout << "       В путь родительской вершины добавляется текущая вершина " << i.first <<"(" << CurLength << ")"<< endl;
                #endif
                vector<char> path = ShortPathes[TmpVertex.first].first;  //добавляется в путь родительской вершины текущая вершина с кратчайшим путем
                path.push_back(i.first);
                ShortPathes[i.first] = { path, CurLength };  //обновление пути и расстояния
                int heur = Heuristic(/*TmpVertex.first*/ end, i.first);
                #ifdef INFO
                cout << "       Эвристика для вершин " << end << " и " << i.first << " = " << heur << endl;
                #endif
                PriorityQueue.push({ i.first, heur + ShortPathes[i.first].second }); //записывается в очередь текущая вершина
                #ifdef INFO
                cout << "           В приоритетную очередь добавляется вершиина с учётом эвристики: " << i.first << '(' << heur + ShortPathes[i.first].second << ')' << endl << endl;
                #endif      
            }

        }

    }
    return ShortPathes[end].first;
}


void FindingPath::SortAStar() {
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        #ifdef INFO
        cout << "Для вершины " << it->first << " есть следующие смежные вершины:\n";
        for (int i = 0; i < it->second.size(); i++) {
            cout << it->second[i].first << '(' << it->second[i].second << ')' << ' ';
        }
        cout << endl;
        #endif
        std::sort(it->second.begin(), it->second.end(), [](pair<char, double>& a, pair<char, double>& b) -> bool 
            { 
                if (a.second == b.second)
                    return (a.first < b.first);
                else
                    return (a.second < b.second); 
            });
        #ifdef INFO
        cout << "Отсортированные вершины:\n";
        for (int j = 0; j < it->second.size(); j++) {
            cout << it->second[j].first << '(' << it->second[j].second << ')' << ' ';
        }
        cout << endl;
        #endif
    }
}


void FindingPath::Sort() {
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        #ifdef INFO
        cout << "Для вершины " << it->first << " есть следующие смежные вершины:\n";
        for (int i = 0; i < it->second.size(); i++) {
            cout << it->second[i].first << '(' << it->second[i].second << ')' << ' ';
        }
        cout << endl;
        #endif
        std::sort(it->second.begin(), it->second.end(), [](pair<char, double>& a, pair<char, double>& b) -> bool {return a.second < b.second; });
        #ifdef INFO
        cout << "Отсортированные вершины:\n";
        for (int j = 0; j < it->second.size(); j++) {
            cout << it->second[j].first << '(' << it->second[j].second << ')' << ' ';
        }
        cout << endl;
        #endif
    }
}


void FindingPath::Read() {
    char start, end;
    cin >> start >> end;
    this->start = start;
    this->end = end;
    int count = 0;

    while (cin >> start) {
        if (start == '0')  //символ остановки ввода данных
            break;
        double weight;
        cin >> end >> weight;
        graph[start].push_back({ end,weight });
        visited[start] = false;
        visited[end] = false;
        count++;
    }
    this->number = count;
}



vector<char> FindingPath::GreedyAlgorithm() {
    #ifdef INFO
    cout << "\nЖадный алгоритм:\n";
    #endif
    double min;
    vector<char> result;
    result.reserve(this->number);
    result.push_back(this->start);

    char CurVertex = this->start;

    while (CurVertex != this->end) {
        #ifdef INFO
        cout << "Текущая вершина - " << CurVertex << endl;
        #endif
        char NextVertex;
        min = 100;
        bool found = false;
        #ifdef INFO
        cout << "Смежные вершины: ";
        for (auto& i : this->graph[CurVertex])
            cout << i.first << '(' << i.second << ')' << ' ';
        cout << endl;
        #endif
        for (auto& i : this->graph[CurVertex]) {
            //cout << "Рассматривается смежная вершина - " << i.first << endl;
            if (!visited[i.first] && i.second < min) {
                #ifdef INFO
                cout << "Идём в вершину " << i.first << "(" << i.second << ")\n";
                #endif
                min = i.second;
                NextVertex = i.first;
                found = true;
        
            }
        }
        //cout << endl;
        visited[CurVertex] = true;

        if (!found) {
            if (!result.empty()) {
                #ifdef INFO
                cout << "   Из вершины " << CurVertex << " конечная вершина недостижима, возвращаемся обратно\n\n";
                #endif
                result.pop_back();
                CurVertex = result.back();
            }
            continue;
        }
        CurVertex = NextVertex;
        result.push_back(CurVertex); 
        #ifdef INFO
        cout << endl;
        #endif
    }
    #ifdef INFO
    cout << "Конец алгоритма!\n";
    #endif
    return result;
}


int main() {
    setlocale(LC_ALL, "Russian");
    FindingPath answer;
    answer.Read();
    answer.Sort();
    vector<char> out = answer.GreedyAlgorithm();
    #ifdef INFO
    cout << "Ответ:";
    #endif
    for (auto& i : out) {
        cout << i;
    }
    return 0;
}