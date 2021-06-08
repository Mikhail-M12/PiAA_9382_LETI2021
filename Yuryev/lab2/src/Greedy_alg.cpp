#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

#define pointFrom char
#define pointTo char
#define ADDITIONAL_INFO

class Edge // ориентированный путь между 2 вершинами
{
    double m_len;
    pointFrom m_start;
    pointTo m_end;

public:
    Edge(double length, pointFrom a, pointTo b): m_len(length), m_start(a), m_end(b)
    {};

    double getLen() {return m_len;};
    pointFrom getPointFrom() {return m_start;};
    pointTo getPointTo() {return m_end;};
};

bool compareEdgesLen(Edge a, Edge b)
{
    if(a.getLen() >= b.getLen())
    {
        return false;
    }
    return true;
}

class Point // вершина
{
    char m_nameOfPoint;
    std::vector<Edge> m_waysFromPoint; // все пути которые идут ИЗ этой вершины

public:
    Point(char name): m_nameOfPoint(name)
    {};

    void addWay(int length, char from, char to)
    {
        m_waysFromPoint.push_back(Edge(length, from, to));
    }

    char getPointName() {return m_nameOfPoint;};
    std::vector<Edge>* getVectorOfWays() {return &m_waysFromPoint;};

    void sortWaysBySize() { std::sort(m_waysFromPoint.begin(), m_waysFromPoint.end(), compareEdgesLen); };
};

int findNeededPointPosition(std::vector<Point>* allPoints, char pointName) // находит позицию указанной вершины в векторе всех вершин
{
    for(int k = 0; k < allPoints->size(); k++)
    {
        if( (allPoints->at(k).getPointName()) == pointName )
        {
            return k;
        }
    }
    return -1;
}

void printTabs(int n)
{
    for(int k = 0; k < n; k++)
    {
        std::cout << "  ";
    }
}


int findWayWithGreedyAlg(std::vector<Point>* allPoints, char currentPoint, char endPoint, std::string* currentWay, int depth)
{
#ifdef ADDITIONAL_INFO
    printTabs(depth);
    std::cout << "Рассмотрим вершину \'" << currentPoint << "\':" << std::endl;
#endif


    if(std::count(currentWay->begin(), currentWay->end(), currentPoint)) // во избежание циклов вида: a->e, e->a
    {
#ifdef ADDITIONAL_INFO
    printTabs(depth);
    std::cout << "Эта вершина уже была рассмотрена -> возвращаемся на прошлый уровень." << std::endl;
#endif

        return 1;
    }

    int pos = findNeededPointPosition(allPoints, currentPoint);
    std::vector<Edge>* waysVector = allPoints->at(pos).getVectorOfWays();

#ifdef ADDITIONAL_INFO
    printTabs(depth);
    std::cout << "Укажем, что мы ее посетили." << std::endl;
#endif
    currentWay->push_back(currentPoint);


#ifdef ADDITIONAL_INFO
    printTabs(depth);
    std::cout << "Пройдем по всем путям из данной вершины:" << std::endl;
#endif

    // идем по всем путям из данной вершины
    for(int k = 0; k < waysVector->size(); k++)
    {
        char nextPoint = waysVector->at(k).getPointTo();

#ifdef ADDITIONAL_INFO
    printTabs(depth);
    std::cout << "Рассмотрим путь \'" << currentPoint << "\'->\'" << nextPoint << "\':" << std::endl;
#endif

        if(nextPoint == endPoint) // нашли путь
        {
#ifdef ADDITIONAL_INFO
    printTabs(depth);
    std::cout << "Этот путь привел нас к искомой вершине. Завершаем алгоритм." << std::endl;
#endif
            currentWay->push_back(nextPoint);
            return 0;
        }

        if(!findWayWithGreedyAlg(allPoints, nextPoint, endPoint, currentWay, depth + 1)) // рекурсивный вызов
        {
            return 0;
        }
    }

#ifdef ADDITIONAL_INFO
    printTabs(depth);
    std::cout << "Ни один из путей не привел нас к искомой вершине -> возвращаемся на прошлый уровень." << std::endl;
#endif

    currentWay->pop_back();
    return 1;
}



int main()
{
    setlocale(LC_ALL, "rus");

    std::vector<Point> points;
    std::string way;
    char startPoint, endPoint;

    char from, to;
    double size;

    #ifdef ADDITIONAL_INFO
      std::cout << "Введите данные:" << std::endl;
    #endif

    std::cin >> startPoint >> endPoint;
    while(std::cin >> from) 
    {
        if (from == '0')
            break;

        std::cin >> to >> size;

        int position = findNeededPointPosition(&points, from);

        if(position == -1)
        {
            points.push_back(Point(from));
            points.at(points.size() - 1).addWay(size, from, to);
        }
        else
        {
            points.at(position).addWay(size, from, to);
        }

        if(findNeededPointPosition(&points, to) == -1)
        {
            points.push_back(Point(to));
        }
    }

    // сортировка путей в каждой из вершин по их размеру
    for(auto k = points.begin(); k != points.end(); k++)
    {
        k->sortWaysBySize();
    }

    findWayWithGreedyAlg(&points, startPoint, endPoint, &way, 0);

    #ifdef ADDITIONAL_INFO
      std::cout << "\n\nИтоговый путь:" << std::endl;
    #endif

    std::cout << way << std::endl;
    return 0;
}