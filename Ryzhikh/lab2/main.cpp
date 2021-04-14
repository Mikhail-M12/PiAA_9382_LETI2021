#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;



class FindingPath {
public:
    FindingPath() = default;
    vector<char> GreedyAlgorithm();
    vector<char> AStar();
    void Sort();
    void Read();
    int Heuristic(char a, char b);

private:
    map<char, vector<pair<char, double>>> graph;
    map<char, bool> visited;
    char start;
    char end;
    int number;
};

struct Sorting { //������� ���������� ��� ������������ �������
    bool operator() (pair<char, double> a, pair<char, double> b) {
        //���� ��������� ���� ������ �����, �� ������������ ������� �� ��� � ���������� �������, ���� ��������� ������, �� ������� �� ���
        if (a.second == b.second)
            return (a.first < b.first);
        else
            return (a.second > b.second);
    }
};

int FindingPath::Heuristic(char a, char b) {
    return abs(a-b);
}

vector<char> FindingPath::AStar() {  //�*
    map<char, pair<vector<char>, double>> ShortPathes;  //������� ���������� ����
    vector<char> vertex;
    priority_queue < pair<char, double>, vector<pair<char, double>>, Sorting> PriorityQueue; //������� � ���������

    PriorityQueue.push({ start, 0 });
    vertex.push_back(start);
    ShortPathes[start].first = vertex;

    while (!PriorityQueue.empty()) {  //���� ������� �� �����
        if (PriorityQueue.top().first == end) {  //���� ������� �������� �������
            return ShortPathes[end].first;  //�� ������������� �����
        }

        auto TmpVertex = PriorityQueue.top();  //��������� ������������ ������� �� �������
        PriorityQueue.pop();

        for (auto& i : graph[TmpVertex.first]) {  //��������������� ��� �������, ������� ��������� � ������� ��������
            double CurLength = ShortPathes[TmpVertex.first].second + i.second;
            if (ShortPathes[i.first].second == 0 || ShortPathes[i.first].second > CurLength) { //���� ���� ��� ��� ��������� ���� ������
                vector<char> path = ShortPathes[TmpVertex.first].first;  //����������� � ���� ������������ ������� ������� ������� � ���������� �����
                path.push_back(i.first);
                ShortPathes[i.first] = { path, CurLength };  //���������� ���� � ����������
                int heur = Heuristic(end, i.first);
                //cout << i.first << ' ' << heuristic[i.first] << '\n';
                PriorityQueue.push({ i.first, heur + ShortPathes[i.first].second }); //������������ � ������� ������� ������� 
            }

        }

    }
    return ShortPathes[end].first;
}



void FindingPath::Sort() {
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        cout << "��� ������� " << it->first << " ���� ��������� ������� �������:\n";
        for (int i = 0; i < it->second.size(); i++) {
            cout << it->second[i].first << '(' << it->second[i].second << ')' << ' ';
        }
        cout << endl;
        std::sort(it->second.begin(), it->second.end(), [](pair<char, double>& a, pair<char, double>& b) -> bool {return a.second < b.second; });
    
        cout << "��������������� �������:\n";
        for (int j = 0; j < it->second.size(); j++) {
            cout << it->second[j].first << '(' << it->second[j].second << ')' << ' ';
        }
        cout << endl;
    }
}


void FindingPath::Read() {
    char start, end;
    cin >> start >> end;
    this->start = start;
    this->end = end;
    int count = 0;

    while (cin >> start) {
        if (start == '0')  //������ ��������� ����� ������
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
    double min;
    vector<char> result;
    result.reserve(this->number);
    result.push_back(this->start);

    char CurVertex = this->start;

    while (CurVertex != this->end) {
        char NextVertex;
        min = 100;
        bool found = false;

        for (auto& i : this->graph[CurVertex]) {
            if (!visited[i.first] && i.second < min) {
                min = i.second;
                NextVertex = i.first;
                found = true;
        
            }
        }
        visited[CurVertex] = true;

        if (!found) {
            if (!result.empty()) {
                result.pop_back();
                CurVertex = result.back();
            }
            continue;
        }
        CurVertex = NextVertex;
        result.push_back(CurVertex);
    }

    return result;
}


int main() {
    setlocale(LC_ALL, "Russian");
    FindingPath answer;
    answer.Read();
    answer.Sort();
    vector<char> out = answer.AStar();
    cout << "�����:";
    for (auto& i : out) {
        cout << i;
    }
    return 0;
}