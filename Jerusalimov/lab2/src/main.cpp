#include <iostream>
#include <vector>
#include <string>



int PRINT = 1;

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
        //edge = new Edge();
    }

    char name;
    bool used;
    vector<Edge *> edge;

};

class Graph {
public:
    Graph() {

    }

    ~Graph() {

    }

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

class Dijkstra {
private:

    class NodeInfo {
    public:
        char vertex;
        char prev;
        int sum;
        bool used;

        NodeInfo(char vertex) {
            this->vertex = vertex;
            used = false;
            sum = INT32_MAX;
            prev = vertex;
        }
    };

    Graph *solve;
    vector<NodeInfo *> info;
    vector<char> result;

public:

    Dijkstra(Graph *a) {
        solve = a;
    }

    string getShortestPath(char vertex1, char vertex2) {
        Init();
        if (PRINT) {
            cout << "\t\t\tDijkstra Algorithm Start!\nInitialization new struct data...\n";
            cout << "The sum of the starting vertex \'" << vertex1 << "\' is equal to zero.\n"
                 << GetNodeInfo(vertex1)->sum
                 << " = 0\n";
        }


        GetNodeInfo(vertex1)->sum = 0;
        char curr;

        if (PRINT) {
            cout << "\tLooking for the minimum unused vertex\n";
        }

        while ((curr = FindUnusedMinimalNode()) != '\0') {
            if (PRINT) {
                cout << "\t_____________" << curr << "_________________\n\t\tSet the sum to the all next neighbors.\n";
            }

            SetSumToNextNodes(curr);
            if (PRINT) {
                cout << "\t______________________________\n\n";
                cout << "\tLooking for the minimum unused vertex\n";
            }

        }
        if (PRINT) {
            cout
                    << "\n\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\tArranged paths, let's walk along them\n";
        }

        return RestorePath(vertex1, vertex2);
    }

    string RestorePath(char vertex1, char vertex2) {
        string path(1, vertex2);
        if (PRINT) {
            cout << "We write at the end the required vertex \'" << path << "\'\n";
        }

        while (vertex2 != vertex1) {
            if (PRINT) {
                cout << "Previous vertex is \'" << GetNodeInfo(vertex2)->prev << "\' From: \'" << vertex2 << "\'\n";

            }
            vertex2 = GetNodeInfo(vertex2)->prev;

            path = string(1, vertex2) + path;
            if (PRINT) {
                cout << "Writing in the start: " << path << "\n";
            }

        }
        return path;
    }

    void SetSumToNextNodes(char curr) {
        NodeInfo *currInfo = GetNodeInfo(curr);
        currInfo->used = true;

        for (int i = 0; i < solve->FindVertex(curr)->edge.size(); ++i) {
            NodeInfo *prevInfo = GetNodeInfo(solve->FindVertex(curr)->edge[i]->name);
            int newSum = currInfo->sum + solve->FindVertex(curr)->edge[i]->mass;
            if (PRINT) {
                cout << "\t\t\tFor vertex \'" << prevInfo->vertex << "\' set a new amount. Was - " << prevInfo->sum
                     << " now - " << newSum << "\n";

            }


            if (newSum < prevInfo->sum) {
                prevInfo->sum = newSum;
                prevInfo->prev = curr;
                if (PRINT) {
                    cout << "\t\t\tRetained a pointer to the previous vertex. \tCurrent vertex: " << prevInfo->vertex
                         << " Previous vertex: " << prevInfo->prev << "\n";

                }

            }


        }
        if (PRINT) {
            cout << "\t\tAll neighboring vertices have been visited.\n";

        }

    }

    char FindUnusedMinimalNode() {
        int minSum = INT32_MAX;
        char minVertex = '\0';
        for (int i = 0; i < solve->graph.size(); ++i) {
            NodeInfo *tempInfo = GetNodeInfo(solve->graph[i].name);
            if (tempInfo->used) continue;
            if (tempInfo->sum < minSum) {
                if (PRINT) {
                    cout << "\t\tFound an unused minimum node vertex: " << tempInfo->vertex << "\n";
                    cout << "\t\tPrevious minimum size is - " << minSum << ". New is - " << tempInfo->sum << "\n\n";
                }
                minSum = tempInfo->sum;
                minVertex = solve->graph[i].name;

            }
        }
        if (PRINT) {
            cout << "\n\tThe minimum unused vertex turned out to be : " << minVertex << ". Size - " << minSum << "\n";
        }
        return minVertex;
    }

    void Init() {
        for (int i = 0; i < solve->graph.size(); ++i) {
            info.push_back(new NodeInfo(solve->graph[i].name));
        }
    }

    NodeInfo *GetNodeInfo(char vertex) {
        for (int i = 0; i < info.size(); ++i) {
            if (info[i]->vertex == vertex) {
                return info[i];
            }
        }
        cout << "Can't find Node info about vertex -  " << vertex;
        exit(-1);

    }


};


int main() {
    Graph a;
    string length;
    char start, end;
    char mainVertex, secondVertex;

    int i = 0;
    int choise;
    int b;
    cout << "enable Intermediate data? 1 - Yes 0 - No\n";
    cin >> b;
    PRINT = b;
    if (PRINT) {
        cout << "Gready Algoritm - 0, Dijkstra - 1\n";
    }
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
    } else {
        Dijkstra *some = new Dijkstra(&a);
        //cout << "Shortest Path using Dijkstra algorithm:\n";
        cout << "\nAnswer is: " + some->getShortestPath(start, end) + '\n';
    }

    system("pause>nul");
    return 0;
}
