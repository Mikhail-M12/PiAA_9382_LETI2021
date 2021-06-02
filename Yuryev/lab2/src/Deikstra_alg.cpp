#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
//#define ADDITIONAL_INFO

class EdgeOfGraph;

class Point
{
public:
    int m_distanceFromStart = -1;
    int m_priority = -1;        // для алг. Дейкстры он будет = m_distanceFromStart
    char m_nameOfPoint = '#';
    bool m_isVisited = false;
    
    std::vector<EdgeOfGraph*> m_edgesFromPoint; // направленные ребра ИЗ этой вершины
    EdgeOfGraph* m_cameFrom = nullptr;                    // ребро, по которому пришли СЮДА
    
    Point()
    {};
    Point(char name): m_nameOfPoint(name)
    {};
    Point(char name, int distance): m_nameOfPoint(name), m_distanceFromStart(distance)
    {};

    ~Point() // с уничтожением точек удалятся и ребра
    {
        for(int k = 0; k < m_edgesFromPoint.size(); k++)
        {
            delete m_edgesFromPoint.at(k);
        }
    }
};

class EdgeOfGraph
{
public:
    Point* m_pointFrom = nullptr;
    Point* m_pointTo = nullptr;
    double m_length = 0;

    EdgeOfGraph()
    {};
    EdgeOfGraph(double lenght, Point* from, Point* to): m_length(lenght), m_pointFrom(from), m_pointTo(to)
    {};
};

class Reading
{
public:
    char startPoint, endPoint;
    std::vector<char>pointsFrom, pointsTo;
    std::vector<double>pointsLengths;

    void doTerminalReading() // считывает введенные знаки и записывает их в вектора
    {
        char from, to;
        double length;

        std::cin >> startPoint >> endPoint;

        while(std::cin >> from)
        {
            if (from == '0')
                break;

            std::cin >> to >> length;

            if( (from == to) && (length == 0) )
                continue;
            else if( ((from == to) && (length != 0)) || ((from != to) && (length == 0)) )
                throw "Incorrect data!";
            else if(length < 0)
                throw "Impossible length!";

            pointsFrom.push_back(from);
            pointsTo.push_back(to);
            pointsLengths.push_back(length);
        }
    }
};



class Graph
{
public:
    Reading m_edgesInfo = Reading();
    std::vector<Point*> m_points;
    std::string existingPoints;

    Point* findExistingPoint(char name)
    {
        for(int k = 0; k < m_points.size(); k++)
        {
            if( (m_points.at(k)->m_nameOfPoint) == name)
            {
                return m_points.at(k);
            }
        }
    }
    
private:
    void chooseEnterType() // пользователь выбирает, как ему вводить данные
    {
        if(1)
        {
            m_edgesInfo.doTerminalReading();
        }
    }
public:

    void createGraph() // просто создает структуру графа (ничего не считает)
    {
        chooseEnterType();

        for(int k = 0; k < m_edgesInfo.pointsFrom.size(); k++)
        {
            Point* pointFrom = nullptr;
            Point* pointTo = nullptr;

            // если объект стартовой вершины этого ребра еще не был создан
            if(!std::count(existingPoints.begin(), existingPoints.end(), m_edgesInfo.pointsFrom.at(k)))
            {
                existingPoints.push_back(m_edgesInfo.pointsFrom.at(k));

                pointFrom = new Point(m_edgesInfo.pointsFrom.at(k));
                m_points.push_back(pointFrom);
            }
            else
            {
                pointFrom = findExistingPoint(m_edgesInfo.pointsFrom.at(k));
            }
            
            if(!std::count(existingPoints.begin(), existingPoints.end(), m_edgesInfo.pointsTo.at(k)))
            {
                existingPoints.push_back(m_edgesInfo.pointsTo.at(k));

                pointTo = new Point(m_edgesInfo.pointsTo.at(k));
                m_points.push_back(pointTo);
            }
            else
            {
                pointTo = findExistingPoint(m_edgesInfo.pointsTo.at(k));
            }
                
            EdgeOfGraph* edge = new EdgeOfGraph(m_edgesInfo.pointsLengths.at(k), pointFrom, pointTo);
            pointFrom->m_edgesFromPoint.push_back(edge);
        }
    }

    Point* getFirstPoint()
    {
        return findExistingPoint(m_edgesInfo.startPoint);
    };

    Point* getLastPoint()
    {
        return findExistingPoint(m_edgesInfo.endPoint);
    };

    ~Graph()
    {
        for(int k = 0; k < m_points.size(); k++)
        {
            delete m_points.at(k);
        }
    }
};

class PriorityQueue
{
public:
    std::vector<Point*> m_points;

    void addPoint(Point* newP) // вставляем вершину сразу учитывая ее приоритет
    {
        for(int i = 0; i < m_points.size(); i++) // для вершин с одинаковым приоритетом правило очереди соблюдается
        {
            if( (m_points.at(i)->m_priority) > (newP->m_priority) ) // TO DO: для А* возможно здесь нужно добавить условие для одинаковых приоритетов на знчение char-а
            {
                auto it = m_points.begin();
                m_points.insert(it+i, newP);
                return;
            }
        }
        m_points.push_back(newP); // если самый большой приоритет в векторе
        return;
    };

    void replacePoint(Point* point)
    {
        for(int k = 0; k < m_points.size(); k++)
        {
            if( (m_points.at(k)->m_nameOfPoint) == point->m_nameOfPoint)
            {
                m_points.erase(m_points.begin() + k);
            }
        }
        addPoint(point);
    }

    Point* getFirst()
    {
        Point* lessPriorPoint = m_points.at(0);
        m_points.erase(m_points.begin());
        return lessPriorPoint;
    };

    bool isEmpty()
    {
        if(m_points.size())
            return false;
        
        return true;
    }
};

class DeikstraAlgorithm
{
public:
    PriorityQueue m_priorQueue = PriorityQueue();
    Graph m_graph = Graph();
    
    void findWayWithDeikstra()
    {
        Point* startPoint = m_graph.getFirstPoint();
        Point* finishPoint = m_graph.getLastPoint();

        #ifdef ADDITIONAL_INFO
            std::cout << "\nИщем путь из \'" << startPoint->m_nameOfPoint << "\' в \'" << finishPoint->m_nameOfPoint << "\'.\n" << std::endl;
        #endif

        // добавление начальной точки
        startPoint->m_distanceFromStart=0;
        startPoint->m_priority=0;
        m_priorQueue.addPoint(startPoint);

        #ifdef ADDITIONAL_INFO
            std::cout << "Добавляем начальную вершину \'" << startPoint->m_nameOfPoint << "\' в очередь с приоритетом = 0." << std::endl;
        #endif

        while(m_priorQueue.isEmpty() == false)
        {
            Point* currentPoint = m_priorQueue.getFirst();
            currentPoint->m_isVisited = true;
            
            #ifdef ADDITIONAL_INFO
                std::cout << "\nИз очереди берем вершину \'" << currentPoint->m_nameOfPoint << "\' с приоритетом = " << currentPoint->m_priority << std::endl;
            #endif

            // завершение алгоритма при нахождении конечной точки
            if(currentPoint == finishPoint)
            {
                #ifdef ADDITIONAL_INFO
                    std::cout << "Найдена искомая вершина \'" << finishPoint->m_nameOfPoint << "\'. Завершение алгоритма...\n" << std::endl;
                #endif

                break;
            }


            // добавление в очередь вершин, связанных с текущей 
            #ifdef ADDITIONAL_INFO
                std::cout << "Добавим в очередь необходимые связанные вершины: " << std::endl;
                std::cout << "(При добавлении или изменении позиции вершины в очереди - ребро, по которому мы пришли в эту вершину будет помечаться.)" << std::endl;
            #endif
            
            for(int k = 0; k < currentPoint->m_edgesFromPoint.size(); k++)
            {
                EdgeOfGraph* currentEdge = currentPoint->m_edgesFromPoint.at(k);
                if(currentEdge->m_pointTo->m_isVisited == false)
                {
                    // если вершина имеет значение по-умолчанию
                    if(currentEdge->m_pointTo->m_distanceFromStart == -1)
                    {
                        currentEdge->m_pointTo->m_distanceFromStart = currentPoint->m_distanceFromStart + currentEdge->m_length;
                        currentEdge->m_pointTo->m_priority = currentEdge->m_pointTo->m_distanceFromStart;
                        currentEdge->m_pointTo->m_cameFrom = currentEdge;

                        m_priorQueue.addPoint(currentEdge->m_pointTo);

                        #ifdef ADDITIONAL_INFO
                            std::cout << "\tCвязанная вершина \'" << currentEdge->m_pointTo->m_nameOfPoint << "\' впервые добавляется в очередь с приоритетом = " << currentEdge->m_pointTo->m_priority << '.' << std::endl;
                        #endif
                    }
                    else if( (currentPoint->m_distanceFromStart + currentEdge->m_length) <  currentEdge->m_pointTo->m_distanceFromStart) // TO DO: возможно для А* нужно будет <=
                    {
                        currentEdge->m_pointTo->m_distanceFromStart = currentPoint->m_distanceFromStart + currentEdge->m_length;
                        currentEdge->m_pointTo->m_priority = currentEdge->m_pointTo->m_distanceFromStart;
                        currentEdge->m_pointTo->m_cameFrom = currentEdge;

                        m_priorQueue.replacePoint(currentEdge->m_pointTo); // так как расстояние не -1, то значит элемент уже внутри очереди
                    
                        #ifdef ADDITIONAL_INFO
                            std::cout << "\tCвязанная вершина \'" << currentEdge->m_pointTo->m_nameOfPoint << "\' уже находится в очереди, но ее приоритет больше найденного, поэтому он меняется на новый = " << currentEdge->m_pointTo->m_priority << '.' << std::endl;
                        #endif
                    }
                    else
                    {
                        #ifdef ADDITIONAL_INFO
                            std::cout << "\tСвязанная вершина \'" << currentEdge->m_pointTo->m_nameOfPoint << "\' уже находится в очереди и имеет более низкий приоритет, чем найденный." << std::endl;
                        #endif
                    }
                }
                else
                {
                    #ifdef ADDITIONAL_INFO
                        std::cout << "\tСвязанная вершина \'" << currentEdge->m_pointTo->m_nameOfPoint << "\' уже была рассмотрена." << std::endl;
                    #endif
                }
            }
            
            #ifdef ADDITIONAL_INFO
                std::cout << "Все связанные вершины рассмотрены. Берем следущую вершину из очереди." << std::endl;
            #endif
        }
    };

    void printShortestWay()
    {
        m_graph.createGraph();

        findWayWithDeikstra();

        Point* startPoint = m_graph.getFirstPoint();
        Point* finishPoint = m_graph.getLastPoint();

        std::string way = "";
        Point* currentPoint = finishPoint;


        #ifdef ADDITIONAL_INFO
            std::cout << "\n\nИдем от конечной вершины по отмеченным ребрам:" << std::endl;
        #endif

        while(currentPoint != startPoint)
        {
            #ifdef ADDITIONAL_INFO
                std::cout << "\tРебро \'" << currentPoint->m_cameFrom->m_pointFrom->m_nameOfPoint << "\'->\'" << currentPoint->m_cameFrom->m_pointTo->m_nameOfPoint << "\' (" << currentPoint->m_cameFrom->m_length << ")." << std::endl;
            #endif
            
            way.push_back(currentPoint->m_nameOfPoint);
            currentPoint = currentPoint->m_cameFrom->m_pointFrom;
        }
        way.push_back(startPoint->m_nameOfPoint);

        std::reverse(way.begin(), way.end());

        #ifdef ADDITIONAL_INFO
            std::cout << "\n\nПолученный в результате путь:\t";
        #endif

        std::cout << way << std::endl;
    }
};


int main()
{
    setlocale(LC_ALL, "rus");

    DeikstraAlgorithm task = DeikstraAlgorithm();
    task.printShortestWay();

    return 0;
}
