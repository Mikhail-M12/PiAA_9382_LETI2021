#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

#define pointFrom char
#define pointTo char
//#define ADDITIONAL_INFO

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



int findWayWithGreedyAlg(std::vector<Point>* allPoints, char currentPoint, char endPoint, std::string* currentWay)
{
    if(std::count(currentWay->begin(), currentWay->end(), currentPoint)) // во избежание циклов вида: a->e, e->a
    {
        return 1;
    }

    int pos = findNeededPointPosition(allPoints, currentPoint);
    std::vector<Edge>* waysVector = allPoints->at(pos).getVectorOfWays();

    currentWay->push_back(currentPoint);

    // идем по всем путям из данной вершины
    for(int k = 0; k < waysVector->size(); k++)
    {
        char nextPoint = waysVector->at(k).getPointTo();

        if(nextPoint == endPoint) // нашли путь
        {
            currentWay->push_back(nextPoint);
            return 0;
        }

        if(!findWayWithGreedyAlg(allPoints, nextPoint, endPoint, currentWay)) // рекурсивный вызов
        {
            return 0;
        }
    }

    currentWay->pop_back();
    return 1;
}



int main()
{
    setlocale(LC_ALL, "rus");

    std::vector<Point> points;
    std::string way;
    char startPoint, endPoint;

    char typeOfEnter = '0';
    char from, to;
    double size;

    // считываем ввод пользователя
    #ifdef ADDITIONAL_INFO

    while(true)
    {
        std::cout << "Вы хотите ввести данные с клавиатуры или из файла? (0/1)" << std::endl;
        std::cout << "Для выхода из программы введите \"q\"." << std::endl;

        std::cin >> typeOfEnter;
        if (typeOfEnter == 'q')
        {
            std::cout << "Был введен символ\'q\'. Завершение программы..." << std::endl;
            return 0;
        }
        else if(typeOfEnter == '1')
        {
            std::ifstream file("test1.txt");
            
            if (file) 
            {
                file >> startPoint >> endPoint;
                while (!file.eof()) 
                {
                    file >> from >> to >> size;

                    int position = findNeededPointPosition(&points, from);
                    
                    if(position == -1)  // случай, когда такая вершина еще не была добавлена
                    {
                        points.push_back(Point(from));
                        points.at(points.size() - 1).addWay(size, from, to);
                    }
                    else
                    {
                        points.at(position).addWay(size, from, to);
                    }

                    if(findNeededPointPosition(&points, to) == -1) // если вершина прибытия еще не была добавлена (нужно т.к. не факт, что из нее что-то будет идти)
                    {
                        points.push_back(Point(to));
                    }
                }
                break;
            }
        }
        else if(typeOfEnter == '0')
        {
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
            break;
        }
        else
        {
            std::cin.ignore(32767,'\n');
            std::cout << "Ввод некорректен. Попробуйте еще раз.\n" << std::endl;
            continue;
        }
    }
    #endif

    #ifndef ADDITIONAL_INFO
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
    #endif

    // сортировка путей в каждой из вершин по их размеру
    for(auto k = points.begin(); k != points.end(); k++)
    {
        k->sortWaysBySize();
    }

    findWayWithGreedyAlg(&points, startPoint, endPoint, &way);

    std::cout << way << std::endl;
    return 0;
}
