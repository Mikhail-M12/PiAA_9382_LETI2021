#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#define COMMENTS
#define PATH

class Point         // класс вершины
{
public:
    char m_name;            // имя вершины
    bool m_isVisited;       // флаг "посещения" (для поиска пути)
    std::pair<int, Point *> m_cameFrom;                 // указывает, как мы добрались до этой вершины
    std::map<char, std::pair<int, int>> m_neighbours;   // хранит информацию о смежных вершинах

    Point(): m_isVisited{false}, m_cameFrom{0, nullptr}
    {}
};


// компаратор для сортировки вершин в графе
bool cmp(Point *a, Point *b);

// ввод и инициализация списка ребер и списка вершин
void doReadingAndInitialization(int numberOfEdges, std::vector<Point *> &graph, std::vector<std::pair<char, char>> &edges);

// проверяет, было ли дано такое ребро
bool isEdgeInEdgesList(std::vector<std::pair<char, char>> edges, char ver1, char ver2);

// выводит ответ
void writeAnswer(std::vector<Point *> graph, char from, std::vector<std::pair<char, char>> &edges);

// выбирает очередную вершину
std::pair<Point *, Point *> choosePoint(std::vector<Point *> graph, std::vector<char> vertices);

// находит вершину в графе, если она есть (в ином случае вернет nullptr)
Point* findPointInGraph(char name, std::vector<Point *> graph);

// находит очередной путь в графе
bool findFlow(std::vector<Point *> &graph, char start, char end);

// находит пропускную способность данного пути
int findMinWayCapacity(char end, std::vector<Point *> graph);

// делает пересчет остаточных пропускных способностей
void recountResidualCapacities(int min, char end, std::vector<Point *> &graph);

// очистка меток, проставленных во время поиска пути (откат значений полей вершин)
void clearUnwantedMarks(std::vector<Point *> &graph);

// функция поиска максимального потока
void findMaxFlow(std::vector<Point *> &graph, char start, char end);

// очищает выделенную под список вершин память
void freeMemory(std::vector<Point *> &graph);

// функция-менеджер (чтобы main() был пустым)
void doTheTask();




int main()
{
    doTheTask();

    return 0;
}

bool cmp(Point *a, Point *b)
{
    return a->m_name < b->m_name;
}

void doReadingAndInitialization(int numberOfEdges, std::vector<Point *> &graph, std::vector<std::pair<char, char>> &edges)
{
    char from, to;
    int len;
    Point *ver;

    for (int i = 0; i < numberOfEdges; i++)
    {
        std::cin >> from >> to >> len;  // считали ребро
        edges.push_back({from, to});    // и добавили его в список ребер

        ver = findPointInGraph(from, graph);

        if (ver != nullptr) // если вершина уже есть в графе
        {
            ver->m_neighbours[to] = {len, 0}; // добавим смежную вершину к списку смежных
        }
        else    // иначе создадим и добавим вершину в список
        {
            ver = new Point;
            ver->m_name = from;
            ver->m_neighbours[to] = {len, 0};
            graph.push_back(ver);
        }

        if (!isEdgeInEdgesList(edges, to, from)) // если обратное ребро не было уже задано
        {
            ver = findPointInGraph(to, graph);      // снова ищем вершину в графе, но уже для обратного ребра

            if (ver == nullptr) // если вершины еще нет, то добавим ее
            {
                ver = new Point;
                ver->m_name = to;
                ver->m_neighbours[from] = {0, 0};
                graph.push_back(ver);
            }
            else    // если же вершина уже есть в графе
            {
                ver->m_neighbours[from] = {0, 0}; // добавим смежную вершину к списку смежных
            }
        }
    }
}

bool isEdgeInEdgesList(std::vector<std::pair<char, char>> edges, char ver1, char ver2)
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

void writeAnswer(std::vector<Point *> graph, char from, std::vector<std::pair<char, char>> &edges)
{
    auto start = findPointInGraph(from, graph);
    int max = 0;

    for (auto i : start->m_neighbours)
    {
        max += i.second.second;
    }
    std::cout << max << '\n';

    for (auto ver : graph)
    {
        for (auto neib : ver->m_neighbours)
        {
            if (isEdgeInEdgesList(edges, ver->m_name, neib.first))
            {
                if (neib.second.second > 0)
                {
                    std::cout << ver->m_name << " " << neib.first << " " << neib.second.second << '\n';
                }
                else
                {
                    std::cout << ver->m_name << " " << neib.first << " 0\n";
                }
            }
        }
    }
}

std::pair<Point *, Point *> choosePoint(std::vector<Point *> graph, std::vector<char> vertices)
{
    int min = 26, check;
    Point *minV;
    Point *prev;
    Point *ver;

#ifdef COMMENTS
    std::cout << "\t\t\tВыбираем новую дугу\n";
#endif
    for (auto name : vertices)
    {
        ver = findPointInGraph(name, graph);
        for (auto neib : ver->m_neighbours)
        {
            if (findPointInGraph(neib.first, graph)->m_isVisited == 1) // если в вершину уже "заходили", то пропускаем ее
            {
                continue;
            }

            check = abs(neib.first - name); // расстояние между именами вершин (индивидуализация)
            if ((check < min || check == min && neib.first < minV->m_name) && neib.second.first > 0)
            {
                prev = ver;
                min = check;
                minV = findPointInGraph(neib.first, graph);
#ifdef COMMENTS
                std::cout << "\t\t\t\tНовая дуга выбрана: [" << prev->m_name << "," << neib.first << "]\n";
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

Point* findPointInGraph(char name, std::vector<Point *> graph)
{
    for (auto i : graph)
    {
        if (i->m_name == name)
        {
            return i;
        }
    }
    return nullptr;
}

bool findFlow(std::vector<Point *> &graph, char start, char end)
{
    std::vector<char> vertices; // для хранения пройденных вершин
    vertices.push_back(start);  // внесли стартовую точку

    auto ver = findPointInGraph(start, graph);
    ver->m_isVisited = true;

#ifdef COMMENTS
    std::cout << "\t\tПоиск пути:\n";
#endif
    while (ver->m_name != end)
    {
#ifdef COMMENTS
        std::cout << "\t\t\tУже использованные вершины:\n\t\t\t[";
        for (auto q : vertices)
        {
            std::cout << q << " ";
        }
        std::cout << "]\n";
#endif
        auto newVers = choosePoint(graph, vertices); // выбор следующего
        auto newV = newVers.first;
        if (newV == nullptr)
        {
            return 0; // если потоков больше нет
        }
#ifdef COMMENTS
        std::cout << "\t\t\tВыбранная вершина: [" << newV->m_name << "]\n";
#endif
        auto prev = newVers.second;
        vertices.push_back(newV->m_name);
        newV->m_isVisited = 1;
        newV->m_cameFrom = {prev->m_neighbours[newV->m_name].first, prev};
        ver = newV;
    }
    return 1; // если дуга нашлась, то данные о вершинах в графе будут изменены, и можно будет восстановить путь
}

int findMinWayCapacity(char end, std::vector<Point *> graph)
{
    auto ver = findPointInGraph(end, graph);
    int min = ver->m_cameFrom.first;

    while (ver->m_cameFrom.second != nullptr) // пока не дойдем до начала
    {
        if (min > ver->m_cameFrom.first)
        {
            min = ver->m_cameFrom.first;
        }
        ver = ver->m_cameFrom.second;
    }
    return min;
}

void recountResidualCapacities(int min, char end, std::vector<Point *> &graph)
{
    auto ver = findPointInGraph(end, graph)->m_cameFrom.second;
    char prev = end;
#ifdef PATH
    std::cout << "\t\tПропускная способность данного пути: " << min << "\n";
    std::vector<char> path;
#endif
#ifdef COMMENTS
    std::cout << "\t\tНачало пересчета пропускных способностей:\n";
#endif
    while (ver->m_cameFrom.second != nullptr) //пока не дойдем до начала
    {
#ifdef PATH
        path.push_back(ver->m_name);
#endif
#ifdef COMMENTS
        std::cout << "\t\t\tИз пропускной способности ребра [" << ver->m_name << ", " << prev << "] было вычтено " << min << "\n";
        std::cout << "\t\t\tК пропускной способности ребра [" << prev << ", " << ver->m_name << "] было прибавлено " << min << "\n\n";
#endif
        ver->m_neighbours[prev].first -= min; //вычитаем из использованного ребра
        ver->m_neighbours[prev].second += min;
        auto prevVer = findPointInGraph(prev, graph);
        prevVer->m_neighbours[ver->m_name].first += min; //добавляем к обратному ребру
        prevVer->m_neighbours[ver->m_name].second -= min;
        prev = ver->m_name;
        ver = ver->m_cameFrom.second;
    }
#ifdef COMMENTS
    std::cout << "\t\t\tИз пропускной способности ребра [" << ver->m_name << ", " << prev << "] было вычтено " << min << "\n";
    std::cout << "\t\tПересчет пропускных способностей окончен\n";
#endif
#ifdef PATH
    path.push_back(ver->m_name);
    std::cout << "\t\tПуть:\n\t\t";
    for (char i = path.size() - 1; i >= 0; i--)
    {
        std::cout << path[i];
    }
    std::cout << end;
#endif
    ver->m_neighbours[prev].first -= min;
    ver->m_neighbours[prev].second += min;
#ifdef COMMENTS
    std::cout << "\n\n";
#endif
}

void clearUnwantedMarks(std::vector<Point *> &graph)
{
    for (auto ver : graph)
    {
        ver->m_cameFrom = {0, nullptr};
        ver->m_isVisited = false;
    }
}

void findMaxFlow(std::vector<Point *> &graph, char start, char end)
{
    std::vector<char> vertices; // для хранения пройденных вершин
    int min;
#ifdef COMMENTS
    std::cout << "Начат поиск макс. потока\n";
#endif
    while (findFlow(graph, start, end)) // пока можем - находим пути в графе
    {
        // находим пропускную способность найденного пути
        min = findMinWayCapacity(end, graph);
#ifdef COMMENTS
        std::cout << "\tПропускная способность посчитана\n";
#endif
        // пересчитываем остаточные пропускные способности
        recountResidualCapacities(min, end, graph);
#ifdef COMMENTS
        std::cout << "\tПересчет пропускных сбособностей выполнен\n";
#endif
        // очищаем метки, поставленные во время поиска пути
        clearUnwantedMarks(graph);
#ifdef COMMENTS
        std::cout << "\tОчистка меток выполнена\n--------------------------------------------\n";
#endif
    }

#ifdef COMMENTS
    std::cout << "Конец! Макс. поток найден\n";
#endif
}

void freeMemory(std::vector<Point *> &graph)
{
    for(int k = 0; k < graph.size(); k++)
    {
        delete graph.at(k);
    }
}

void doTheTask()
{
    int numberOfEdges;
    char start, end;

    std::cin >> numberOfEdges >> start >> end;                  // считали кол-во ребер, исток и сток

    std::vector<Point*> graph; 
    std::vector<std::pair<char, char>> edges;

    doReadingAndInitialization(numberOfEdges, graph, edges);    // считали введенные ребра и инициализировали список ребер и список вершин(граф)

    std::sort(graph.begin(), graph.end(), cmp);                 // отсортировали список вершин

    findMaxFlow(graph, start, end);                             // нашли максимальный поток

    writeAnswer(graph, start, edges);                           // вывели ответ

    freeMemory(graph);                                          // очистили выделенную для хранения вершин память
}