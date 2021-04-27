#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm> // std::sort

//#define COMMENTS

struct Vertex
{
    char name;
    std::map<char, double> adjacentV;
    bool seen;
    double g;
    double f;
    Vertex *prev;
};

//функции
void input(char &start, char &end1, char &end2, std::vector<Vertex *> &);
//функция считываящая ввод
double h(char start, char end1, char end2);
//эвристическая фукнция оценки расстояния между вершинами
void fAdj(char end, std::vector<Vertex *> &open, Vertex *curr, std::vector<Vertex *> vertices);
//функция поиска вершины с мин. f
Vertex *retVer(char ver, std::vector<Vertex *> vertices);
//функция возвращает указатель на вершину с именем ver
bool isIn(Vertex *vert, std::vector<Vertex *> &vertices);
//проверяет есть ли вершина в списке вершин
void printAns(Vertex *ans);
//функция вывода ответа
bool aStar(char start, char end, std::vector<Vertex *> &vertices);
//функция поиска пути при помощи алгоритма A* c 1 end

int main()
{
    char fromVer, toVer1, toVer2;   //для записи вершины, с которой начинаем, и вершин(ы), до которой(ых) нужно искать путь
    std::vector<Vertex *> vertices; //для записи вершин
    input(fromVer, toVer1, toVer2, vertices);
    if (fromVer == toVer1 || fromVer == toVer2)
    {
        std::cout << fromVer << '\n';
        return 0;
    }
    if (retVer(toVer1, vertices) == nullptr && retVer(toVer2, vertices) == nullptr)
    {
        std::cout << "Такого пути нет\n";
        return 0;
    }
    bool ans = 0;
    char name;
    char random = rand() % 2 + 1;

    if (toVer1 == toVer2 || random == 1)
    {
#ifdef COMMENTS
        std::cout << "Будем искать путь до вершины [" << toVer1 << "]\n";
#endif
        ans = aStar(fromVer, toVer1, vertices);
        name = toVer1;
    }
    else
    {
#ifdef COMMENTS
        std::cout << "Будем искать путь до вершины [" << toVer2 << "]\n";
#endif
        ans = aStar(fromVer, toVer2, vertices);
        name = toVer2;
    }
    if (!ans)
    {
        std::cout << "Такого пути нет\n";
    }
    else
    {
#ifdef COMMENTS
        std::cout << "Ответ:\n";
#endif
        printAns(retVer(name, vertices));
        std::cout << '\n';
    }
    return 0;
}

void input(char &start, char &end1, char &end2, std::vector<Vertex *> &vertices)
//функция считываящая ввод
{
    std::string in;
    getline(std::cin, in);
    //std::cout << "\t\tin [" << in << "]\n";
    start = in[0];
    end1 = in[2];
    if (in[4] >= 'a' && in[4] <= 'z')
    {
        end2 = in[4];
    }
    else
    {
        end2 = end1;
    }
    char ver1, ver2;
    double len;
    std::map<char, double> adjV;
    bool flag = 0;
    while (getline(std::cin, in))
    {
        if (in.empty())
            break;
        ver1 = in[0];
        ver2 = in[2];
        len = std::stod(in.substr(3));
        flag = 0;
        for (auto &i : vertices)
        {
            if (i->name == ver1)
            {
                flag = 1;
                i->adjacentV[ver2] = len;
            }
        }
        if (!flag)
        {
            adjV[ver2] = len;
            auto *v = new Vertex;
            v->name = ver1;
            v->adjacentV[ver2] = len;
            v->seen = 0;
            v->prev = nullptr;
            v->f = 100000;
            v->g = 0;
            vertices.push_back(v);
            adjV.clear();
        }
        flag = 0;
        for (auto &i : vertices)
        {
            if (i->name == ver2)
            {
                flag = 1;
            }
        }
        if (!flag)
        {
            adjV[ver2] = len;
            auto *v = new Vertex;
            v->name = ver2;
            v->seen = 0;
            v->f = 100000;
            v->g = 0;
            vertices.push_back(v);
            adjV.clear();
        }
    }
#ifdef COMMENTS
    std::cout << "От вершины [" << start << "] до вершины [" << end1 << "] или [" << end2 << "]\n";
    std::cout << "Vertices:\n";
    for (auto &i : vertices)
    {
        std::cout << i->name << "---------------------";
        for (auto &j : i->adjacentV)
        {
            std::cout << '[' << j.first << ',' << j.second << "] ";
        }
        std::cout << std::endl;
    }
#endif
}

double h(char start, char end)
//эвристическая фукнция оценки расстояния между вершинами
{
    return abs(end - start);
}

void fAdj(char end, std::vector<Vertex *> &open, Vertex *curr, std::vector<Vertex *> vertices)
// функция рассматривает смежные с curr вершины
//если смежной вершины нет в open, то добавляет
//меняет f, если она стала меньше
{
    Vertex *neighbour;
    double newF = 0, newG = 0;
    if (!curr->adjacentV.empty())
    {
        for (auto ver : curr->adjacentV)
        {
#ifdef COMMENTS
            std::cout << '\n';
#endif
            neighbour = retVer(ver.first, vertices);
            if (neighbour->seen != 1)
            {
                newG = curr->g + ver.second;
                newF = curr->g + ver.second + h(ver.first, end);
                if (!isIn(neighbour, open) || newF < neighbour->f)
                {
                    neighbour->prev = curr;
                    neighbour->g = newG;
                    neighbour->f = newF;
                }
                if (newF < neighbour->f)
                {
#ifdef COMMENTS
                    std::cout << "Новая f меньше старой, перезаписываем\n";
#endif
                }
                if (!isIn(neighbour, open))
                {
#ifdef COMMENTS
                    std::cout << "Добавляем вершину [" << ver.first << "] в open\n";
#endif
                    open.push_back(neighbour);
                }

                newF = curr->f + ver.second + h(neighbour->name, end);
#ifdef COMMENTS
                std::cout << "Новая f для [" << ver.first << "] равна " << newF << "\n";
#endif
                if (newF < neighbour->f)
                {
#ifdef COMMENTS
                    std::cout << "Новая f (" << newF << ") для [" << ver.first << "] меньше старой (" << neighbour->f << "). Обновление.\n";
#endif

                    neighbour->g += ver.second;
                    neighbour->f = neighbour->g + h(ver.first, end);
                    neighbour->prev = curr;
                }
#ifdef COMMENTS
                else
                {
                    std::cout << "Новая f не лучше старой\n";
                }
#endif
            }
        }
    }
}

Vertex *retVer(char ver, std::vector<Vertex *> vertices)
{
    for (auto &i : vertices)
    {
        if (i->name == ver)
        {
            return i;
        }
    }
    return nullptr;
}

bool isIn(Vertex *vert, std::vector<Vertex *> &vertices)
{
    for (auto &i : vertices)
    {
        if (i->name == vert->name)
        {
            return true;
        }
    }
    return 0;
}

void printAns(Vertex *ans)
{
    if (ans->prev == nullptr)
    {
        std::cout << ans->name;
        return;
    }
    printAns(ans->prev);
    std::cout << ans->name;
}

bool cmp(Vertex *a, Vertex *b)
// для сортировки open
{
    if (a->f == b->f)
    {
        return a->name > b->name;
    }
    return a->f < b->f;
}

bool aStar(char start, char end, std::vector<Vertex *> &vertices)
{
    std::vector<Vertex *> open;
    Vertex *curr;
    open.push_back(retVer(start, vertices));
    open[0]->g = 0;
    open[0]->f = h(start, end);
    while (!open.empty())
    {
        curr = open[0];
#ifdef COMMENTS
        std::cout << "Выбрали вершину [" << curr->name << "]\n";
#endif
        if (curr->name == end)
        {
#ifdef COMMENTS
            std::cout << "Найден путь\n";
#endif
            return 1;
        }
        fAdj(end, open, curr, vertices);
#ifdef COMMENTS
        std::cout << "Open:\n";
        for (auto &i : open)
        {
            std::cout << i->name << " ";
        }
        std::cout << '\n';
#endif
        curr->seen = 1;
        open.erase(open.begin());
        std::sort(open.begin(), open.end(), cmp);
#ifdef COMMENTS
        std::cout << "Open after delete elem and sort:\n";
        for (auto &i : open)
        {
            std::cout << "[" << i->name << ", " << i->f << "] ";
        }
        std::cout << "\n------------------\n";
#endif
    }
    return 0;
}