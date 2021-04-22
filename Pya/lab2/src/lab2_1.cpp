#include <iostream>
#include <list>
#include <fstream>

#define Vertical char
#define Weight float

struct Point {//структура для вершины графа
    Vertical ver; //название вершины
    bool isVisited;//посещена ли вершина или нет

    Point() = default;
    Point(Vertical ver, bool isVisited) {
        this->ver = ver;
        this->isVisited = isVisited;
    }
};
struct Edge {//стуктура для ребра графа
    Point* initVer;//начальная вершина
    Point* finVer;//конечная вершина
    Weight weightVer;//вес ребра

    Edge() = default;
    Edge(Point* init, Point* fin, Weight weight) {
        initVer = init;
        finVer = fin;
        weightVer = weight;
    }
};
bool compForSort (const Edge& first, const Edge& second)//функция для сортировки ребер
{
    if (first.initVer->ver < second.initVer->ver)//по первой вершине
        return true;
    if (first.initVer->ver > second.initVer->ver)
        return false;
    if (first.weightVer < second.weightVer)//по весу
        return true;
    return false;
}

class PathSearching {//класс для поиска пути в графе
private:
    Vertical initDestVer, finDestVer;//начальная и конечная вершины
    std::string answer;//строка для хранения пути
public:
    std::list<Edge> listOfEdge;//список ребер
    std::list<Point*> listOfVer;//список вершин
    PathSearching(Vertical initDestVer, Vertical finDestVer) {
        this->initDestVer = initDestVer;
        this->finDestVer = finDestVer;
    }
    void readEdgeAndVer(std::istream& fin) {//ввод ребер и формирование списков
        Vertical initVer, finVer;
        Weight weight = 0;
        while (fin >> initVer >> finVer >> weight) {
            if (initVer == '0')//для удобства был введен символ для остановки ввода
                break;
            Point* initPointVer = findVer(initVer);
            Point* finPointVer = findVer(finVer);
            if (initPointVer == nullptr) {
                initPointVer = new Point(initVer, false);
                listOfVer.push_back(initPointVer);//формирование списка вершин
            }
            if (finPointVer == nullptr) {
                finPointVer = new Point(finVer, false);
                listOfVer.push_back(finPointVer);
            }
            listOfEdge.emplace_back(initPointVer, finPointVer, weight);//формирование списка ребер
        }
    }
    void showAnswer() {//вывод пути
        std::cout << "Результат работы алгоритма:\n";
        std::cout << answer << "\n";
    }

    void showListOfEdge() {//вывод списка ребер
        std::cout << "Отсортированный список ребер\n";
        for (auto i : listOfEdge) {
            std::cout << i.initVer->ver << " " << i.finVer->ver << " " << i.weightVer << "\n";
        }
    }

    Point* findVer(Vertical vertical) {//поиск структуры вершины по ее названию
        for (auto i : listOfVer) {
            if (i->ver == vertical) {
                return i;
            }
        }
        return nullptr;
    }

    void doGreedyAlgoritm() {//построение пути с помощью жадного метода
        listOfEdge.sort(compForSort);//сортировка списка ребер по первой вершине и весу
        showListOfEdge();
        Point *currentPoint;
        for (auto i : listOfVer) {//поиск начальной вершины
            if (i->ver == initDestVer) {
                currentPoint = i;
                break;
            }
        }
        while (currentPoint->ver != finDestVer) {//пока рассматриваемая вершина не является конечной
            Vertical curVer = currentPoint->ver;//сохраняем название рассматриаемой вершины
            std::cout << "Рассматриваемая вершина " << currentPoint->ver << "\n";
            answer += curVer;//и добавляем ее в ответ
            currentPoint->isVisited = true;//и добавляем в список рассмотренных
            for (auto i : listOfEdge) {
                if (i.initVer == currentPoint) {
                    if (!i.finVer->isVisited) {
                        currentPoint = i.finVer;//выбор следующей рассматриваемой вершины, как первой найденной благодаря сортировке по весу
                        break;
                    }
                }
            }
            if (currentPoint->ver == curVer) {//если рассматриваемая вершина не поменялась
                std::cout << "Следующая вершина не найдена, переходим к предыдущей ";
                for (auto i : listOfEdge) {
                    if (i.finVer == currentPoint) {
                        currentPoint = i.initVer;//тогда переходим к предпоследней
                        std::cout << currentPoint->ver << "\n";
                        answer.pop_back();
                        answer.pop_back();
                        break;
                    }
                }
            } else std::cout << "Следующая вершина, выбранная по минимальному весу ребра " << currentPoint->ver << "\n";
        }
        answer += finDestVer;//в ответ добавляем конечную вершину
    }
};
int main() {
    char answ = 'y';
    while (answ == 'y') {
        std::cout << "Хотите считать данные из файла или ввести самостоятельно?(1/2)\n";
        std::cin >> answ;
        Vertical initDestVer, finDestVer;
        std::ifstream fin("test1.txt");
        if (answ == '2') {
            std::cout << "Введите начальную и конечную вершину\n";
            std::cin >> initDestVer >> finDestVer;
        } else {
            fin >> initDestVer >> finDestVer;
        }
        auto answer = new PathSearching(initDestVer, finDestVer);
        if (answ == '2') {
            std::cout << "Введите ребра и вес\n";
            answer->readEdgeAndVer(std::cin);
        } else answer->readEdgeAndVer(fin);
        fin.close();
        answer->doGreedyAlgoritm();
        answer->showAnswer();
        std::cout << "Хотите продолжить?(y/n)\n";
        std::cin >> answ;
    }
    return 0;
}