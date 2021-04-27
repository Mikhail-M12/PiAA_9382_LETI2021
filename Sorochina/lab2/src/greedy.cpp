#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using vectPair = std::vector<std::pair<char, double>>;

#define COMMENTS

struct Edge
{
    char v1; //структура для хранения имен двух вершин, соединненых ребром, и длины ребра
    char v2;
    double len;
    Edge(char a, char b, double c) : v1(a), v2(b), len(c) {}
};

//функции
void input(std::vector<Edge> &);                                                                   //ok
void findWay(char from, char to, std::vector<Edge> &edges, bool &flag, std::vector<char> &answer); //функция поиска пути в графе
vectPair adjacentVer(char ver, std::vector<Edge> edges);                                           //функция поиска смежных с данной вершин
//ok

int main()
{
    char fromEdge, toEdge;
    std::vector<Edge> edges;
    std::string in;
    getline(std::cin, in);
    fromEdge = in[0];
    toEdge = in[2];
    input(edges);
    std::vector<char> answer;
    bool flag = false;
    findWay(fromEdge, toEdge, edges, flag, answer);
    if (flag)
    {
#ifdef COMMENTS
        std::cout << "Ответ:\n";
#endif
        for (auto i : answer)
        {
            std::cout << i;
        }
    }
    else
    {
        std::cout << "No way from " << fromEdge << " to " << toEdge << ".";
    }
    std::cout << '\n';
    return 0;
}

void input(std::vector<Edge> &edges) //ok
{
    std::string in;
    while (getline(std::cin, in))
    {
        if (in.empty())
            break;
        edges.push_back({in[0], in[2], std::stod(in.substr(3))});
    }
}

vectPair adjacentVer(char ver, std::vector<Edge> edges)
//находит смежные и сортирует их по длине ребра в порядке возрастания
{
    vectPair adjVer;
    for (auto i : edges)
    {
        if (i.v1 == ver)
        {
            adjVer.push_back({i.v2, i.len});
        }
    }
    std::sort(adjVer.begin(), adjVer.end(), [](std::pair<char, double> a, std::pair<char, double> b) {
        return a.second < b.second;
    });
    return adjVer;
}

void findWay(char from, char to, std::vector<Edge> &edges, bool &flag, std::vector<char> &answer)
//функция поиска пути, если таковой имеется
{
    std::map<char, vectPair> dict;
    std::vector<char> vertices;
    vectPair vect;
    int ansSize = 0;
    char ver = from, ver2;
    while (ver != to)
    {
        if (dict.count(ver) == 0)
        {
            vertices.push_back(ver);
            dict[ver] = adjacentVer(ver, edges);
        }
#ifdef COMMENTS
        std::cout << "Смежные с [" << ver << "]\n";
        for (auto i : dict[ver])
        {
            std::cout << '[' << i.first << ',' << i.second << ']';
        }
        std::cout << '\n';
#endif
        if (dict[ver].size() > 0)
        {

            if (ansSize > 0 && ver != answer[ansSize - 1] || ansSize == 0)
            {
/*if (find(answer.begin(), answer.end(), ver) != answer.end())
                {
                    answer.clear();
                    ansSize = 0;
                }*/
#ifdef COMMENTS
                std::cout << "Добавили в ответ вершину [" << ver << "]\n";
#endif
                answer.push_back(ver);
                ansSize++;
            }
            ver2 = ver;
            ver = dict[ver][0].first;
#ifdef COMMENTS
            std::cout << "------------------------\n";
            std::cout << "Новая рассматриваемая вершина [" << ver << "]\n";
#endif
#ifdef COMMENTS
            std::cout << "В данный момент в ответ записано:\n";
            if (ansSize > 0)
            {
                for (auto i : answer)
                {
                    std::cout << i;
                }
            }
            else
            {
                std::cout << "nothing";
            }
            std::cout << "\n";
#endif
            dict[ver2].erase(dict[ver2].begin());
        }
        else
        {
            if (ver == from)
            {
#ifdef COMMENTS
                std::cout << "Дошли до начальной вершины => пути нет\n";
#endif
                break;
            }
            ver = answer[ansSize - 1];
#ifdef COMMENTS
            std::cout << "Смежных нет, возвращаемся на шаг назад\n";
            std::cout << "------------------------\n";
            std::cout << "Новая рассматриваемая вершина [" << ver << "]\n";
#endif
            answer.pop_back();
            ansSize--;
#ifdef COMMENTS
            std::cout << "В данный момент в ответ записано:\n";
            if (ansSize > 0)
            {
                for (auto i : answer)
                {
                    std::cout << i;
                }
            }
            else
            {
                std::cout << "nothing";
            }
            std::cout << "\n";
#endif
        }
    }
    answer.push_back(ver);
    if (ver == to)
    {
#ifdef COMMENTS
        std::cout << "Добавили в ответ вершину [" << ver << "]\n";
#endif
        flag = 1;
        return;
    }
}
