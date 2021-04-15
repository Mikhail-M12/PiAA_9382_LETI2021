#include <iostream>
#include <vector>
#include <string>
#include <queue>


int PRINT = 0;

using namespace std;


struct Edge {
    char name;
    int mass;

    Edge(char name, int mass) {
        this->name = name;
        this->mass = mass;

    }


};


struct Vertex {
    Vertex(char name) {
        this->name = name;
        used = false;
        prev = nullptr;
        sum = 0;
        priority = 0;
    }

    double sum;
    double priority;
    char name;
    bool used;
    Vertex *prev;
    vector<Edge *> edge;

};

struct CmpAstar {
    bool operator()(Vertex *v1, Vertex *v2) {//for queue
        if (v1->priority == v2->priority) {
            return v2->name > v1->name;
        }
        return (v2->priority) < (v1->priority);
    }

    bool cmpAstar(Vertex *num1, double num2) {//for nums
        return num1->priority > num2;
    }
};

struct CmpDijkstra {
    bool operator()(Vertex *v1, Vertex *v2) { //for queue
        return (v2->sum) < (v1->sum);
    }

    bool cmpDijkstra(double num1, double num2) { //for nums
        return num1 > num2;
    }
};


class Graph {
public:


    void AddVertex(char name) {
        if (FindVertex(name) == nullptr) {
            Vertex currVertextex(name);
            graph.push_back(currVertextex);
        }

    }

    void AddEdge(char vertex1, char vertex2, int mass) {
        FindVertex(vertex1)->edge.push_back(new Edge(vertex2, mass));
        // FindVertex(vertex2)->edge.push_back(new Edge(vertex1, mass));
    }

    Vertex *FindVertex(char name) {
        for (int i = 0; i < graph.size(); ++i) {
            if (graph[i].name == name) {
                return &graph[i];
            }
        }
        return nullptr;
    }

    vector<Vertex> graph;
};


class GreedyAlgorithm {
private:
    Graph *solve;
    vector<Vertex *> result;

public:

    GreedyAlgorithm(Graph *a) {
        solve = a;
    }

    void getShortestPath(char vertex1, char vertex2) {
        if (PRINT) {
            cout << "\t\t\tGreedy Algorithm Start!";
        }
        int minSize;
        char cur = vertex1;
        Vertex *currVertex;
        result.push_back(FindVertex(vertex1));
        currVertex = FindVertex(cur);
        int i = 0;
        while (cur != vertex2) {
            if (PRINT) {
                cout << "\n\n________________________________" << i
                     << "___________________________________\n\t\tCurrent Vertex != answer. \n\t\t\t" << cur << " != "
                     << vertex2 << "\n\t\t\tcontinue...\n\n";
            }

            ++i;
            bool found = false;
            //result.push_back(FindVertex(cur));
            Vertex *nextVert;
            minSize = INT32_MAX;
            if (PRINT) {
                cout << "\tWe look at the neighboring vertices at the current node - " << cur << "\n";
                cout << "\tAnd choose the smallest path.\n";
            }


            for (int i = 0; i < currVertex->edge.size(); ++i) {
                if (PRINT) {
                    cout << "\tAdjacent vertex: " << cur << " -> " << currVertex->edge[i]->name << "\n";

                }
                if (!FindVertex(currVertex->edge[i]->name)->used && currVertex->edge[i]->mass < minSize) {

                    minSize = currVertex->edge[i]->mass;
                    nextVert = FindVertex(currVertex->edge[i]->name);
                    found = true;
                    if (PRINT) {
                        cout << "\t\tThe edge is minimal " << minSize << ", we keep the path.";
                        cout << "\t\tTemp answer is: " << cur << " -> " << currVertex->edge[i]->name << "\n";
                    }

                } else if (FindVertex(currVertex->edge[i]->name)->used) {
                    if (PRINT) {
                        cout << "\t\tThe neighbor has already been visited.\n";
                    }
                } else {
                    if (PRINT) {
                        cout << "\t\tCurrent weight is less than new " << minSize << " < " << currVertex->edge[i]->mass
                             << "\n";
                    }
                }

            }


            if (!(currVertex->name == vertex1)) {
                currVertex->used = true;
            }


            if (!found) {

                if (!result.empty()) {
                    result.pop_back();
                    currVertex = result.back();
                    cur = currVertex->name;
                }
                if (PRINT) {
                    cout << "\nAll neighboring vertices have been visited, we return to the previous vertex - "
                         << currVertex->name << ".\n";
                }
                continue;
            }
            if (PRINT) {
                cout << "\nAdd vertex \"" << nextVert->name << "\" to the response stack with minimum weight \""
                     << minSize
                     << "\"\n";
            }

            currVertex = nextVert;
            cur = currVertex->name;
            result.push_back(currVertex);
            if (PRINT) {
                cout << "Answer in the curren elment is : ";


                for (int i = 0; i < result.size(); ++i) {
                    cout << result[i]->name << "->";
                }
            }
        }


        if (PRINT) {
            cout
                    << "\nWe reached the final vertices. \nCompleting the algorithm.\n\n//////////////////////////////////////////////////////////////////////////////\n";
        }
        cout << "Shortest path using greedy algorithm:\n";


        for (int i = 0; i < result.size(); ++i) {
            cout << result[i]->name;
        }
        return;

    }


    Vertex *FindVertex(char nameVer) {
        for (int i = 0; i < solve->graph.size(); ++i) {
            if (nameVer == solve->graph[i].name) {
                return &solve->graph[i];
            }
        }
        cout << "Can't find vertex - " << nameVer << "\n";
        exit(-1);
    }


};


class Dijkstra_and_Astar {
private:
    void RestorePath(const char start, char end) {
        string path(1, end);
        while (end != start) {
            end = graph->FindVertex(end)->prev->name;
            path = string(1, end) + "->" + path;
        }
        cout << path << "\n";
        cout << "Answer is :" << path << "\n";
    }

    Graph *graph;

public:
    Dijkstra_and_Astar(Graph *a) {
        this->graph = a;
    }

    template<typename T>
    void printQueue(priority_queue<Vertex *, std::vector<Vertex *>, T> queue) {
        while (!queue.empty()) {
            if (typeid(T) != typeid(CmpDijkstra))
                cout << "{ " << queue.top()->name << ", " << queue.top()->priority << "}";
            else
                cout << "{ " << queue.top()->name << ", " << queue.top()->sum << "}";
            queue.pop();
        }
    }

    template<typename T>
    void AstarOrDijkstra(char start, char end) {
        priority_queue<Vertex *, std::vector<Vertex *>, T> quequeAstar;

        quequeAstar.push(graph->FindVertex(start));

        Vertex *current;
        Vertex *temp;
        int newSum;
        double tempHeuristic;
        bool choiseAlgorithm = 1;
        CmpDijkstra cmp;
        CmpAstar cmpA;
        if (typeid(T) == typeid(CmpDijkstra)) {
            choiseAlgorithm = 0;

            cout << "Using Dijkstra algorithm\n______________________________________\n";
            cout << "\tCalculate the summ of path for each neighbor\n\n";

        } else {

            cout << "Using A* algorithm\n______________________________________\n";
            cout << "\tCalculating the priority for each neighbor\n\n";

        }
        do {

            current = quequeAstar.top();

            if (PRINT) {
                cout << "//////////////////////////////////////////////////////////////\n";
                cout << "\n\tCurrent queue ( ";
                printQueue<T>(quequeAstar);
                cout << " )\n";
                cout << "\tWe took the vertices out of the queue" << "{ " << quequeAstar.top()->name << ", ";
                if (!choiseAlgorithm)
                    cout<<quequeAstar.top()->sum << "}" << " and put it in a temporary variable\n";
                else
                    cout<<quequeAstar.top()->priority << "}" << " and put it in a temporary variable\n";


            }

            quequeAstar.pop();
            current->used = true;

            if (PRINT) {
                cout << "\tWe go through all the neighbors of the vertex  \'" << current->name << "\'\n";
                if (current->edge.empty() && current->name != end) {
                    cout << "Vertex \'" << current->name << "\' has no neighbors\nTake the next item in the queue\n\n";

                }
            }

            for (auto &neighbour : current->edge) {

                tempHeuristic = current->sum + neighbour->mass + (end - neighbour->name);
                temp = graph->FindVertex(neighbour->name);
                if (PRINT) {
                    cout << "\t\t__________________________" << current->name << " -> " << neighbour->name
                         << "__________________________\n";
                    if (choiseAlgorithm) {
                        cout << "\t\t\tCalculating the priority f = g + h = " << current->sum + neighbour->mass
                             << " + " << (end - neighbour->name) << " = " << tempHeuristic << "\n";
                    } else {
                        cout << "\t\t\tcalculate the sum for \'" << neighbour->name << "\', summ = current + next = "
                             << current->sum << " + " << neighbour->mass << " = " << current->sum + neighbour->mass
                             << "\n";
                    }

                }

                if (choiseAlgorithm ? cmpA.cmpAstar(temp, tempHeuristic) : cmp.cmpDijkstra(temp->sum, neighbour->mass+current->sum)) {
                    if (PRINT) {
                        if (choiseAlgorithm) {
                            cout << "\t\t\tThe current  heuristic function is less than the previous one ("
                                 << temp->name << "), write a new value\n\t\t\t" << temp->priority << " > "
                                 << tempHeuristic << " ; " << temp->name << " = " << tempHeuristic << "\n";
                        } else {
                            cout << "\t\t\tThe current summ is less than the previous one (" << temp->name
                                 << "), write a new value\n\t\t\t" << temp->sum << " < "
                                 << current->sum + neighbour->mass << " ; " << temp->name << " = "
                                 << current->sum + neighbour->mass << "\n";
                        }
                    }
                    temp->priority = tempHeuristic;
                    temp->sum = current->sum + neighbour->mass;
                    temp->prev = current;
                    if (PRINT) {

                        cout << "\t\t\tAdd prev vertex in parent ";
                        while (temp->prev != nullptr&& temp->name != start && temp->name != end) {
                            cout << temp->prev->name << "->";
                            temp = temp->prev;
                        }
                        cout << "\n";

                    }

                } else if (choiseAlgorithm ? temp->priority == 0 : temp->sum == 0) {

                    temp->priority = tempHeuristic;
                    temp->sum = current->sum + neighbour->mass;
                    temp->prev = current;
                    quequeAstar.push(temp);
                    if (PRINT) {
                        if (choiseAlgorithm) {
                            cout << "\t\t\tnew priority for \'" << temp->name << "\' = " << temp->priority
                                 << " \n";
                        } else {
                            cout << "\t\t\tThe new sum of paths for \'" << temp->name << "\' = " << temp->sum << " \n";
                        }
                        cout << "\t\t\tAdd in queue \'" << temp->name << "\'. (";
                        printQueue<T>(quequeAstar);
                        cout << " )\n\n";

                        cout << "\t\t\tAdd prev vertex in parent ";
                        while (temp->prev != nullptr && temp->name != start && temp->name != end) {
                            cout << temp->prev->name << "<-";
                            temp = temp->prev;
                        }
                        cout << "\n";

                    }
                }else{
                    if (PRINT) {
                        cout<<"\t\t\tSum of top \'"<<temp->name<<"\' is better than new. "<<temp->sum<<" < "<<current->sum + neighbour->mass <<", go next.\n";
                    }
                }
            }
            if (PRINT) {
                if (current->name != end)
                    cout << "\n\t\t\t\t" << current->name << " != " << end << "\n \t\t\t\tContinue...\n\n";
                else cout << "\n\t\t\t\t" << current->name << " = " << end << "\n \t\t\t\tEnd\n\n";
            }


        } while (current->name != end);


        RestorePath(start, end);
        return;
    }
};


int main() {
    Graph a;
    string length;
    char start, end;
    char mainVertex, secondVertex;

    int i = 0;
    int choise = 1;
    int b;
    cout << "enable Intermediate data? 1 - Yes 0 - No\n";
    cin >> b;
    PRINT = b;
    //if (PRINT) {
    cout << "Gready Algoritm - 0, Astar - 1, Dijkstra - 2\n";
    //}
    cin >> choise;
    if (PRINT) {
        cout << "Input data with ')' on end: \n";
    }
    cin >> start >> end;
    while (mainVertex != ')' && cin >> mainVertex) {

        if (mainVertex != ')') {
            cin >> secondVertex >> length;
            a.AddVertex(mainVertex);
            a.AddVertex(secondVertex);
            a.AddEdge(mainVertex, secondVertex, stoi(length));
        }

    }

    if (choise == 0) {
        GreedyAlgorithm *some = new GreedyAlgorithm(&a);

        some->getShortestPath(start, end);
    } else if (choise == 1) {
        Dijkstra_and_Astar *some = new Dijkstra_and_Astar(&a);
        //cout << "Shortest Path using Dijkstra algorithm:\n";
        // cout << some->getShortestPath(start, end);
        some->AstarOrDijkstra<CmpAstar>(start, end);
    } else {
        Dijkstra_and_Astar *some = new Dijkstra_and_Astar(&a);
        some->AstarOrDijkstra<CmpDijkstra>(start, end);
    }

    system("pause>nul");
    return 0;
}
