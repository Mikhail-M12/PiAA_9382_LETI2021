#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#define COMMENTS
#define PATH

struct Vertex //структура для хранения информации о вершине
{
    char name;                                      //имя вершины
    bool seen;                                      //флаг использования при поиске пути
    std::pair<int, Vertex *> from;                  //из какой вершины пришли и длина дуги
    std::map<char, std::pair<int, int>> neighbours; //хранит информацию о смежных вершинах
    Vertex()
    {
        seen = 0;
        from = {0, nullptr};
    }
};

bool cmp(Vertex *a, Vertex *b);
// для сортировки вершин в графе
void input(int numberOfEdges, std::vector<Vertex *> &graph, std::vector<std::pair<char, char>> &edges);
//функция ввода и инициализации массива ребер и вершин
bool isEdge(std::vector<std::pair<char, char>> edges, char ver1, char ver2);
//проверка было ли дано такое ребро
void answer(std::vector<Vertex *> graph, char from, std::vector<std::pair<char, char>> &edges);
//функция вывода ответа
std::pair<Vertex *, Vertex *> chooseVer(std::vector<Vertex *> graph, std::vector<char> vertices);
//функция выбора очередной вершины
Vertex *retVer(char name, std::vector<Vertex *> graph);
//функция поиска вершины в графе
bool flow(std::vector<Vertex *> &graph, char start, char end);
//функция поиска очередного пути в графе
int minCapacity(char end, std::vector<Vertex *> graph);
//функция нахождения пропускной способности для данного пути
void recount(int min, char end, std::vector<Vertex *> &graph);
//фукнция пересчета остаточных пропускных способностей
void clearFrom(std::vector<Vertex *> &graph);
//очиста меток, проставленных во время поиска пути
void maxFlow(std::vector<Vertex *> &graph, char start, char end);
//функция поиска максимального потока

int main()
{
    int numberOfEdges;
    char start, end;
    std::cin >> numberOfEdges >> start >> end;
    std::vector<Vertex *> graph;
    std::vector<std::pair<char, char>> edges;

    input(numberOfEdges, graph, edges);
    std::sort(graph.begin(), graph.end(), cmp);
    maxFlow(graph, start, end);
    answer(graph, start, edges);
    return 0;
}

bool cmp(Vertex *a, Vertex *b)
// для сортировки в графе
{
    return a->name < b->name;
}

void input(int numberOfEdges, std::vector<Vertex *> &graph, std::vector<std::pair<char, char>> &edges)
//функция ввода и инициализации массива ребер и вершин
{
    char from, to;
    int len;
    Vertex *ver;
    for (int i = 0; i < numberOfEdges; i++)
    {
        std::cin >> from >> to >> len;
        edges.push_back({from, to});
        ver = retVer(from, graph);
        if (ver != nullptr) //если вершина есть в графе
        {
            ver->neighbours[to] = {len, 0}; //то добавляется смежная вершина к списку смежных
        }
        else
        { //иначе добавляется новая вершина в граф
            ver = new Vertex;
            ver->name = from;
            ver->neighbours[to] = {len, 0};
            graph.push_back(ver);
        }
        ver = retVer(to, graph);      //для обратного ребра
        if (!isEdge(edges, to, from)) //если ребра еще не было
        {                             //то либо добавляем саму вершину(если ее не было), либо смежную ей
            if (ver == nullptr)
            {
                ver = new Vertex;
                ver->name = to;
                ver->neighbours[from] = {0, 0};
                graph.push_back(ver);
            }
            else
            {
                ver->neighbours[from] = {0, 0};
            }
        }
    }
}

bool isEdge(std::vector<std::pair<char, char>> edges, char ver1, char ver2)
//проверка было ли дано такое ребро
{
    for (auto i : edges)
    {
        if (i.first == ver1 && i.second == ver2)
        {
            return 1;
        }
    }
    return 0;
}

void answer(std::vector<Vertex *> graph, char from, std::vector<std::pair<char, char>> &edges)
//функция вывода ответа
{
    auto start = retVer(from, graph);
    int max = 0;
    for (auto i : start->neighbours)
    {
        max += i.second.second;
    }
    std::cout << max << '\n';
    for (auto ver : graph)
    {
        for (auto neib : ver->neighbours)
        {
            if (isEdge(edges, ver->name, neib.first))
            {
                if (neib.second.second > 0)
                {
                    std::cout << ver->name << " " << neib.first << " " << neib.second.second << '\n';
                }
                else
                {
                    std::cout << ver->name << " " << neib.first << " 0\n";
                }
            }
        }
    }
}

std::pair<Vertex *, Vertex *> chooseVer(std::vector<Vertex *> graph, std::vector<char> vertices)
//функция выбора очередной вершины
{
    int min = 26, check;
    Vertex *minV;
    Vertex *prev;
    Vertex *ver;
#ifdef COMMENTS
    std::cout << "\t\t\tВыбираем новую дугу\n";
#endif
    for (auto name : vertices)
    {
        ver = retVer(name, graph);
        for (auto neib : ver->neighbours)
        {
            if (retVer(neib.first, graph)->seen == 1) //если в вершину уже "заходили", то пропускаем
            {
                continue;
            }
            check = abs(neib.first - name); //расстояние именами вершин
            if ((check < min || check == min && neib.first < minV->name) && neib.second.first > 0)
            {
                prev = ver;
                min = check;
                minV = retVer(neib.first, graph);
#ifdef COMMENTS
                std::cout << "\t\t\t\tНовая дуга выбрана: [" << prev->name << "," << neib.first << "]\n";
#endif
            }
        }
    }
    if (min == 26)
    {
        return {nullptr, nullptr};
    }
    return {minV, prev};
}

Vertex *retVer(char name, std::vector<Vertex *> graph)
//функция поиска вершины в графе
{
    for (auto i : graph)
    {
        if (i->name == name)
        {
            return i;
        }
    }
    return nullptr;
}

bool flow(std::vector<Vertex *> &graph, char start, char end)
//функция поиска очередного пути в графе
{
    std::vector<char> vertices; //для хранения пройденных вершин
    vertices.push_back(start);
    auto ver = retVer(start, graph);
    ver->seen = 1;
#ifdef COMMENTS
    std::cout << "\t\tПоиск пути:\n";
#endif
    while (ver->name != end)
    {
#ifdef COMMENTS
        std::cout << "\t\t\tУже использованные вершины:\n\t\t\t[";
        for (auto q : vertices)
        {
            std::cout << q << " ";
        }
        std::cout << "]\n";
#endif
        auto newVers = chooseVer(graph, vertices); //выбор следующего
        auto newV = newVers.first;
        if (newV == nullptr)
        {
            return 0; //если потоков больше нет
        }
#ifdef COMMENTS
        std::cout << "\t\t\tВыбранная вершина: [" << newV->name << "]\n";
#endif
        auto prev = newVers.second;
        vertices.push_back(newV->name);
        newV->seen = 1;
        newV->from = {prev->neighbours[newV->name].first, prev};
        ver = newV;
    }
    return 1; //если дуга нашлась, то данные о вершинах в графе будут изменены, и можно будет восстановить путь
}

int minCapacity(char end, std::vector<Vertex *> graph)
//функция нахождения пропускной способности для данного пути
{
    auto ver = retVer(end, graph);
    int min = ver->from.first;
    while (ver->from.second != nullptr) //пока не дойдем до начала
    {
        if (min > ver->from.first)
        {
            min = ver->from.first;
        }
        ver = ver->from.second;
    }
    return min;
}

void recount(int min, char end, std::vector<Vertex *> &graph)
//фукнция пересчета остаточных пропускных способностей
{
    auto ver = retVer(end, graph)->from.second;
    char prev = end;
#ifdef PATH
    std::cout << "\t\tПропускная способность данного пути: " << min << "\n";
    std::vector<char> path;
#endif
#ifdef COMMENTS
    std::cout << "\t\tНачало пересчета пропускных способностей:\n";
#endif
    while (ver->from.second != nullptr) //пока не дойдем до начала
    {
#ifdef PATH
        path.push_back(ver->name);
#endif
#ifdef COMMENTS
        std::cout << "\t\t\tИз пропускной способности ребра [" << ver->name << ", " << prev << "] было вычтено " << min << "\n";
        std::cout << "\t\t\tК пропускной способности ребра [" << prev << ", " << ver->name << "] было прибавлено " << min << "\n\n";
#endif
        ver->neighbours[prev].first -= min; //вычитаем из использованного ребра
        ver->neighbours[prev].second += min;
        auto prevVer = retVer(prev, graph);
        prevVer->neighbours[ver->name].first += min; //добавляем к обратному ребру
        prevVer->neighbours[ver->name].second -= min;
        prev = ver->name;
        ver = ver->from.second;
    }
#ifdef COMMENTS
    std::cout << "\t\t\tИз пропускной способности ребра [" << ver->name << ", " << prev << "] было вычтено " << min << "\n";
    std::cout << "\t\tПересчет пропускных способностей окончен\n";
#endif
#ifdef PATH
    path.push_back(ver->name);
    std::cout << "\t\tПуть:\n\t\t";
    for (char i = path.size() - 1; i >= 0; i--)
    {
        std::cout << path[i];
    }
    std::cout << end;
#endif
    ver->neighbours[prev].first -= min;
    ver->neighbours[prev].second += min;
#ifdef COMMENTS
    std::cout << "\n\n";
#endif
}

void clearFrom(std::vector<Vertex *> &graph)
//очиста меток, проставленных во время поиска пути
{
    for (auto ver : graph)
    {
        ver->from = {0, nullptr};
        ver->seen = 0;
    }
}

void maxFlow(std::vector<Vertex *> &graph, char start, char end)
//функция поиска максимального потока
{
    std::vector<char> vertices; //для хранения пройденных вершин
    int min;
#ifdef COMMENTS
    std::cout << "Начат поиск макс. потока\n";
#endif
    while (flow(graph, start, end))
    {
        //для поиск пропускной способности найденного пути
        min = minCapacity(end, graph);
#ifdef COMMENTS
        std::cout << "\tПропускная способность посчитана\n";
#endif
        //пересчет остаточных пропускных способностей
        recount(min, end, graph);
#ifdef COMMENTS
        std::cout << "\tПересчет пропускных сбособностей выполнен\n";
#endif
        //очистка from'ов
        clearFrom(graph);
#ifdef COMMENTS
        std::cout << "\tОчистка меток выполнена\n--------------------------------------------\n";
#endif
    }
#ifdef COMMENTS
    std::cout << "Конец! Макс. поток найден\n";
#endif
}
