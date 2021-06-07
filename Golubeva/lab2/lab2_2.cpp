#include <iostream>
#include <vector>
#include <queue>
#include <map>

class Edge{
public:
    int name;
    int parent;
    float heuristic;

    Edge(int n, int par, float r):name(n), parent(par), heuristic(r){}
    ~Edge() = default;
};

class Cmp{ 
public:
    Cmp() = default;
    ~Cmp() = default;
    bool operator()(const Edge& a, const Edge& b);
};

bool Cmp::operator()(const Edge& a, const Edge& b){// a compare function to priority queue
    if (a.heuristic == b.heuristic){
        return a.name < b.name;
    }
    return a.heuristic > b.heuristic;
}

void printPath(int current, int begin, std::map<int, std::pair<int, float>> way){ 
    if (begin == current){
        std::cout << current<< ' ';
        return ;
    }
    printPath(way[current].first, begin, way);
    std::cout << current<< ' ';
}

void printQueue(std:: priority_queue <Edge, std::vector<Edge>, Cmp> currentQueue){
    std::cout<<"Current queue: ";
    while (!currentQueue.empty()){
        auto vertice = currentQueue.top();
        std::cout<<vertice.parent<<':'<<" ("<< vertice.name<<", "<< vertice.heuristic <<"), ";
        currentQueue.pop();
    }
    std::cout<<"\n";
}

int main(){

    std::map<int, std::vector<std::pair<int, float> >> pathesGraph;
    std::vector <Edge> Edges;// a vector to save a pop vertice 
    std:: priority_queue <Edge, std::vector<Edge>, Cmp> currentQueue;
    std::map<int, std::pair<int, float>> shortPath; // a shortes paths to vertices
    std::map<int, bool> alreadyViewedVertices;
    
    int begin, result;//the path to find

    std::cin >> begin >> result;

    int start, finish;
    float length = 0.0;

    while (std::cin >> start){
        if (start == '&')
            break;
        std::cin >> finish >> length;
        pathesGraph[start].push_back(std::make_pair(finish, length));
    }

    currentQueue.push(Edge(begin, '\0', float(result - begin)));

    while(!currentQueue.empty()){
        printQueue(currentQueue);
        if (currentQueue.top().name == result){ // if find a result vertice
            std::cout<<"Find path! Result path: ";
            printPath(result, begin, shortPath);
            std::cout<<"\n";
            return 0;
        }

        for (int i = 0 ; i < 1 && !currentQueue.empty(); i++){ 
            Edge current = currentQueue.top();
            std::cout<<"Consider a vertice: "<<current.parent<<':'<<", ("<< current.name<<", "<< current.heuristic <<") \n";

            if (current.name == result){
                continue;
            }

            Edges.push_back(current);
            currentQueue.pop();
        }

        int size = Edges.size();
        
        for(int i = 0; i < size; i++){// consider a taken off vertices

            Edge currentEdge = Edges[i];
            alreadyViewedVertices[currentEdge.name] = true;
            std::cout<<"Consider a adjacent vertices for virtice '"<<currentEdge.name<<"'\n";
            for (int j = 0; j < pathesGraph[currentEdge.name].size(); j++){// consider all adjacent vertices  
                
                std::pair<int, float> newEdge = pathesGraph[currentEdge.name][j];
                std::cout<<"Check ("<< newEdge.first<<", "<<newEdge.second<<")\n";
                if (alreadyViewedVertices[newEdge.first]){
                    continue;
                }    
                float newPath = newEdge.second + shortPath[currentEdge.name].second;
                if (shortPath[newEdge.first].second == 0 || newPath < shortPath[newEdge.first].second){//check path from this vertice
                    std::cout<<"Find the shorter path or new path to  "<<newEdge.first<<" from "<< currentEdge.name<<" with length "<< newPath <<" \n";
                    //added if he path more short or vertice not considered earlier
                    shortPath[newEdge.first].first = currentEdge.name;
                    shortPath[newEdge.first].second = newPath;
                    currentQueue.push(Edge(newEdge.first, currentEdge.name, shortPath[newEdge.first].second + float(result-newEdge.first)));
                }
            }
        }
        std::cout<<"\n";
        Edges.clear();
    }
    return 0;
}