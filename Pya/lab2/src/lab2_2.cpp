#include <iostream>
#include <list>
#include <fstream>

#define Vertical char
#define Weight float

struct Point {//структура для вершины графа
    Vertical ver; //название вершины
    bool isVisited;//посещена ли вершина или нет
    Weight heurEst, wei;//эвристическая оценка и вес от начальной вершины до текущей
    Point* parent;//начальная вершина в ребре

    Point() = default;
    Point(Vertical ver, bool isVisited) {
        this->ver = ver;
        parent = nullptr;
        this->isVisited = isVisited;
        this->heurEst = 0;
        this->wei = 0;
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

class PathSearching {//класс для нахождения кратчайшего пути в графе по алгоритму А*
private:
    Vertical initDestVer, finDestVer;//начальная и конечная вершины
public:
    std::list<Edge> listOfEdge;//список ребер
    std::list<Point*> listOfVer;//список вершин
    PathSearching(Vertical initDestVer, Vertical finDestVer) {
        this->initDestVer = initDestVer;
        this->finDestVer = finDestVer;
    }
    void readEdgeAndVer(std::istream& fin, bool isCin) {//ввод ребер и формирование списков
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
        for (auto i : listOfVer) {
            if (isCin) {
                std::cout << "Введите эвристическую функцию для вершины " << i->ver << "\n";
            }
            fin >> i->heurEst;
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

    void showAnswer() {//вывод кратчайшего пути
        Point* currentPoint = findVer(finDestVer);
        std::string answer;
        while (currentPoint != nullptr) {
            answer.push_back(currentPoint->ver);
            currentPoint = currentPoint->parent;
        }
        std::cout << "Результат работы алгоритма:\n";
        for (int i = answer.length() - 1; i >= 0;i--) {
            std::cout << answer[i];
        }
        std::cout << "\n";
    }

    void showListOfNotVisitedVerAndHeur() {//вывод списка непосещенных вершин и ее характеристик
        std::cout << "Список непосещенных вершин:\nвершина эвриситическая оценка вес\n";
        for (auto i : listOfVer) {
            if (!i->isVisited) {
                std::cout << i->ver << " " << i->heurEst << " " << i->wei << "\n";
            }
        }
        std::cout << "Конец списка.\n";
    }

    void defineWeightAndHeur(Point* currentPoint) {//определение эвристической оценки и веса от начальной вершины до текущей для смежных вершин
        std::cout << "Считаем вес для смежных вершин\n";
        for (auto i : listOfEdge) {
            if (i.initVer == currentPoint) {
                if (i.finVer->wei > i.weightVer + currentPoint->wei || i.finVer->wei == 0) {//при меньшей сумме характеристик вершины или их отсутствии присвоим подсчитанные раннее
                    i.finVer->wei = i.weightVer + currentPoint->wei;
                    i.finVer->isVisited = false;
                    std::cout << "Вес ребер от начальной до текущей для вершины " << i.finVer->ver << " посчитан: " << i.finVer->wei << "\n";
                }
            }
        }
    }

    Point* findNextVertical() {//поиск следующей вершины с наименьшими характеристиками
        Point* min = nullptr;
        std::cout << "Выбирается вершина с наименьшей суммой эвристической оценки и веса ребер от начальной вершины до текущей\n";
        for (auto i : listOfVer) {
            if ((min == nullptr || (min->heurEst + min->wei) > (i->heurEst + i->wei) ||
                 (min->heurEst + min->wei) == (i->heurEst + i->wei) && i->heurEst < min->heurEst) &&
                !i->isVisited && !(i->wei == 0)) {//при равенстве сумм характеристик вершин выбирается та, у которой эвристическая оценка меньше
                min = i;
            }
        }
        std::cout << "Наименьшая вершина " << min->ver << " с " << min->heurEst + min->wei << "\n";
        return min;
    }
    Point* defineNextCurrentVer(Point* min) {//определение следующей рассматриваемой вершины
        for (auto i : listOfEdge) {
            if (!i.finVer->isVisited && i.finVer->ver == min->ver && min->wei == i.initVer->wei + i.weightVer) {//находим начальную вершину ребра для построения пути
                std::cout << "Выбранная часть пути:\n";
                std::cout << i.initVer->ver << " " << min->ver << "\n";
                min->parent = i.initVer;
                break;
            }
        }
        return min;
    }

    void doAlgoritmA() {//построение кратчайшего пути с помощью метода А*
        Point *currentPoint = findVer(initDestVer);
        while (currentPoint->ver != finDestVer) {//пока рассматриваемая вершина не окажется конечной
            currentPoint->isVisited = true;//отмечаем рассмотренную вершину
            std::cout << "Рассматриваемая вершина " << currentPoint->ver << "\n";
            defineWeightAndHeur(currentPoint);
            showListOfNotVisitedVerAndHeur();
            Point *min = findNextVertical();
            currentPoint = defineNextCurrentVer(min);
        }
    }
};

int main() {
    char answ = 'y';
    while (answ == 'y') {
        std::cout << "Хотите считать данные из файла или ввести самостоятельно?(1/2)\n";
        std::cin >> answ;
        Vertical initDestVer, finDestVer;
        std::ifstream fin("test2.txt");
        if (answ == '2') {
            std::cout << "Введите начальную и конечную вершину\n";
            std::cin >> initDestVer >> finDestVer;
        } else {
            fin >> initDestVer >> finDestVer;
        }
        auto answer = new PathSearching(initDestVer, finDestVer);
        if (answ == '2') {
            std::cout << "Введите ребра и вес\n";
            answer->readEdgeAndVer(std::cin, true);
        } else answer->readEdgeAndVer(fin, false);
        fin.close();
        answer->doAlgoritmA();
        answer->showAnswer();
        std::cout << "Хотите продолжить?(y/n)\n";
        std::cin >> answ;
    }
    return 0;
}