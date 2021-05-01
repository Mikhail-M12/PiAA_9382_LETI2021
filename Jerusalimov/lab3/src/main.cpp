#include <iostream>
#include <map>
#include <vector>

using namespace std;

bool choise = 0;


class Graph {
public:
    void inputGraph() {
        int size;
        cin >> size >> source >> runoff;
        while (size != 0) {
            char u, v;
            int mass;
            cin >> u >> v >> mass;
            graphK1[u][v] = mass;
            --size;
        }
    }

    template<class T>
    vector<pair<char, int>> findNeighbor(char sought, T whereSearch) {

        vector<pair<char, int>> answ;
        pair<char, int> edge;
        for (auto vertex : whereSearch[sought]) {
            edge.first = vertex.first;
            edge.second = vertex.second;
            answ.push_back(edge);
        }
        return answ;
    };


    bool depthSearch() { //поиск в ширину
        if (choise) {
            cout << "\n__________________Breadth First Search___________________\n";
        }

        vector<char> tempVert; //Список вершин которые хотим посетить
        map<char, bool> visitedVertex; // Пара: имя - состояние. Список посещеных вершин
        char sought; //искомая вершина. Используется для поиска соседей этой вершины
        visitedVertex[source] = true;
        tempVert.push_back(source);

        //Пока есть вершины в стеке, продолжаем расматривать.
        while (!tempVert.empty()) {

            if (choise) {
                cout << "\n\tThere are unconsidered vertices in the stack\n";
            }

            //Смотрим соседей текущей вершины
            sought = tempVert.back();
            tempVert.pop_back();
            if (choise) {
                cout << "\tWe are looking for neighbors, for the vertex: " << sought << "\n\n";
            }

            for (auto enumEdge: findNeighbor(sought, graphK1)) {

                if (choise) {
                    cout << "\t\t\t\t " << sought << "->" << enumEdge.first << " \n";
                }

                //Если вес ребра больше нуля и мы не посещали ее
                if (enumEdge.second > 0 && !visitedVertex[enumEdge.first]) {
                    if (choise) {
                        cout << "\t\t The weight > 0 && not been visited\n"
                             << "\t\tWrite to the path...\n\n";
                    }
                    tempVert.push_back(enumEdge.first); //Добавляем имя вершины для просмотра на след.иттерации
                    path[enumEdge.first] = sought; // Сохраняем путь.
                    visitedVertex[enumEdge.first] = true; // Посетили текущую

                    //Если текущая вершина равна истоку, выходим и возвращаем true, путь найден.
                    if (enumEdge.first == runoff) {

                        if (choise) {
                            char a, b;
                            cout << "\t\tCurrent vertex is equal to source, search is complete!\n \t\tPath is equal: "
                                 << runoff;
                            for (a = this->runoff; a != this->source; a = b) {
                                b = path[a];
                                cout << "<-" << path[a];
                            }
                            cout << "\n\n";
                        }

                        return true;
                    }
                }

            }
        }
        if (choise) {
            cout << "\n\tPATH NOT FOUND!!!\n";
        }
        //Все вершины расмотрены, путь не найден.
        return false;
    }

    int findK2(vector<pair<char, int>> k2, char b) {
        for (auto i : k2) {
            if (i.first == b) {
                return i.second;
            }
        }
    }

    /*

     (1) С помощью поиска в ширину мы находим путь до истока,
     параллельно записываем его.

         (1.1) Если путь до истока был найден,
         мы проходимся по записанному пути и находим минимальную пропускную способность.
            (1.1.1) После чего мы снова проходимся по всему пути и от К1 отнимаем найденую
            минимальную величину, а к К2 прибавляем её. (K1 -= min) (K2 += min)
            K1 - вес ребра, пропускная способность ребра. K2 - вес ребра в другую сторону, проходимый поток.
            (1.1.2) Прибавляем минимальную пропускную способность к максимальному потоку

    (2) Путь не найден. Выходим из цикла и выводим результат.
     */
    void fulkerson() {
        char tempRunoff, tempSource;
        int maxFlow = 0;

        //(1)
        while (depthSearch()) {
            if (choise) {
                cout << "_______________________________________________\n\t\t\tFulkerson\n";
            }
            int flowInEdge = INT32_MAX;

            //(1.1)
            if (choise) {
                cout << "Minimum bandwidth for path: " << runoff;
            }
            for (tempRunoff = this->runoff; tempRunoff != this->source; tempRunoff = tempSource) {
                tempSource = path[tempRunoff];
                flowInEdge = min(flowInEdge, graphK1[tempSource][tempRunoff]);

                if (choise) {
                    cout << "<-" << path[tempRunoff];
                }
            }

            if (choise) {
                cout << ", is equal: " << flowInEdge << "\n";
            }

            //(1.1.1)
            if (choise) {
                cout<< "We go all the path and change the values of K1 and K2.\n"
                       "For the throughput of the rib and for the throughput in the opposite direction, respectively\n";
            }
            for (tempRunoff = this->runoff; tempRunoff != this->source; tempRunoff = tempSource) {
                tempSource = path[tempRunoff];
                if (choise) {
                    cout<<"\t      "<<graphK1[tempSource][tempRunoff]<<"/"<<graphK2[tempRunoff][tempSource]<<"\n";
                    cout<<"\t   "<<tempSource<<" <-> "<< tempRunoff<<"\n";
                    cout << "\tK1 = " << graphK1[tempSource][tempRunoff] << " - " << flowInEdge << "\n";
                    cout << "\tK2 = " << graphK2[tempRunoff][tempSource] << " + " << flowInEdge << "\n";
                }
                graphK1[tempSource][tempRunoff] -= flowInEdge;
                graphK2[tempRunoff][tempSource] += flowInEdge;
                if (choise) {

                    cout << "\tK1 {" << tempSource << " -> " << tempRunoff << " = " << graphK1[tempSource][tempRunoff]
                         << "}\n";
                    cout << "\tK2 {" << tempSource << " <- " << tempRunoff << " = " << graphK2[tempRunoff][tempSource]
                         << "}\n\n";

                }

            }


            //(1.1.2)
            if (choise) {
                cout<<"Add the minimum bandwidth to the maximum flow: "<<maxFlow<<" + "<< flowInEdge<<"\n";
            }

                maxFlow += flowInEdge;

        }
        //(2)
        cout << maxFlow << "\n";
        for (auto &vertex : graphK1) {
            for (auto neighbor : graphK1[vertex.first]) {
                auto temp = findNeighbor(neighbor.first, graphK2);
                cout << vertex.first << " " << neighbor.first << " " << findK2(temp, vertex.first) << "\n";
            }
        }

    }


private:
    char source, runoff; //исток, сток
    map<char, map<char, int>> graphK1; //исходный граф
    map<char, map<char, int>> graphK2; // граф с инвертироваными ребрами и проходящий через ребро поток. с->f стало f->c
    map<char, char> path;
};


int main() {
    cout << "enable Intermediate data? 1 - Yes 0 - No\n";
    cin >> choise;

    if (choise) {
        cout << "_____________Input graph__________________\n";
    }

    Graph g;
    g.inputGraph();

    if (choise) {
        cout << "\n____________Fulkerson___________________\n";
    }
    g.fulkerson();
    system("pause>nul");
    return 0;
}
