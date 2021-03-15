#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <queue>
#include <fstream>

//couts
#define DEBUG
//astar algorithm if 1, if 0 -- greedy
#define ASTAR 1
//int -- main task, char -- stepik
#define TYPE int
//1 -- input from file, 0 - input from console
#define FILE_INPUT 1
//for ints
#define TAB


//структура-компаратор, для определения приоритета в очереди
//если 'стоимость' двух вершин одна и та же, то возвращается меньшая в алфавитном порядке
//если же разная, то большая
template <typename T>
struct CustomCompare{
    bool operator() (std::pair<T, double> a, std::pair<T, double> b){
        return (a.second == b.second) ? (a.first < b.first) : (a.second > b.second);
    }
};

//класс поиска
template <typename T>
class ShortestPathFinder{
public:
    std::vector<T> greedySearch();
    std::vector<T> aStarSearch();
    void inputGraph(std::istream& stream);
    void printQueue(std::priority_queue<std::pair<T, double>, std::vector<std::pair<T, double>>, CustomCompare<T>> queue);

private:
    std::map<T, std::vector<std::pair<T, double>>> graph;
    std::map<T, bool> visited;
    T start;
    T end;
    int numberOfEdges;
};


template <typename T>
void ShortestPathFinder<T>::inputGraph(std::istream& stream) {
    T start,end;
    stream >> start >> end;
    this->start = start;
    this->end = end;
    int counter = 0;

    while(stream >> start && start != '#'){
        double weight;
        stream >> end >> weight;
        graph[start].push_back({end,weight});
        visited[start] = false;
        visited[end] = false;
        counter++;
    }
    this->numberOfEdges = counter;
}

template <typename T>
void ShortestPathFinder<T>::printQueue(std::priority_queue<std::pair<T, double>, std::vector<std::pair<T, double>>, CustomCompare<T>> queue) {
    while(!queue.empty()){
        std::cout << "{ " << queue.top().first << ", " << queue.top().second << " } ";
        queue.pop();
    }
}

template <typename T>
std::vector<T> ShortestPathFinder<T>::aStarSearch() {
    std::map<T, std::pair<std::vector<T>, double>> shortestPaths;
    std::priority_queue<std::pair<T, double>, std::vector<std::pair<T, double>>, CustomCompare<T>> priorityQueue;

    priorityQueue.push({start, 0});
    std::vector<T> vec;
    vec.push_back(start);
    shortestPaths[start].first = vec;

    while(!priorityQueue.empty()){
#ifdef DEBUG
        std::cout << std::endl << "Состояние очереди: " << std::endl;
        printQueue(priorityQueue);
        std::cout << std::endl;

        std::cout << "Проверяем, является ли приоритетный элемент в очереди конечным пунктом: ";
#endif

        if(priorityQueue.top().first == end){
#ifdef DEBUG
            std::cout << "Да, является "<< end <<" Заканчиваем поиск." << std::endl;
#endif
            return shortestPaths[end].first;
        }
#ifdef DEBUG
        std::cout << "Нет, приоритетный " << priorityQueue.top().first << ", а конечный " << end << std::endl;
#endif

        auto tempVertex = priorityQueue.top();
#ifdef DEBUG
        std::cout << "Достаем приоритетный элемент из очереди: {" << tempVertex.first << ", " << tempVertex.second << "}" << std::endl;
#endif
        priorityQueue.pop();

#ifdef DEBUG
        std::cout << "Начинаем рассматривать соседей этой вершины: " << std::endl;
#endif
        //проходимся по всем вершинам, которые соединяются с текущей вершиной
        for(auto &item : graph[tempVertex.first]){
            double currentPathLength = shortestPaths[tempVertex.first].second + item.second;
#ifdef DEBUG
            std::cout << "Вершина: {" << item.first << " ," << item.second << " }" << std::endl;
            std::cout << "Текущее расстояние: " << currentPathLength << std::endl;
#endif

            //если раньше пути не существовало или найденный путь лучше, меняем на него
            if(shortestPaths[item.first].second == 0 || shortestPaths[item.first].second > currentPathLength){
                //добавляем в путь родительской вершины еще и текущую вершину, для которой расстояние оказалось меньше
                std::vector<T> path = shortestPaths[tempVertex.first].first;
                path.push_back(item.first);

                //меняем путь и расстояние на новые
                shortestPaths[item.first] = {path,currentPathLength};
#ifdef DEBUG
                std::cout<<"Текущее расстояние либо меньше предыдущего, либо предыдущего не было , записываем его:";
                for(auto &el : path){
                    std::cout << el << " ";
                }
                std::cout << std::endl;
#endif
                //эвристическая функция, для целых чисел и букв одинакова
                double euristic = abs(end - item.first);
                //записываем в очередь текущую вершину и соответствующую ей функцию f = g + h
                priorityQueue.push({item.first, euristic + shortestPaths[item.first].second});

#ifdef DEBUG
                std::cout << "Записываем в очередь вершину " << item.first << " с оценочной функцией равной f = g + h = ";
                std::cout << shortestPaths[item.first].second << " + " << euristic << " = " << euristic + shortestPaths[item.first].second;
                std::cout << std::endl;
#endif
            }

        }

    }
#ifdef DEBUG
    std::cout << "Очередь опустела, заканчиваем поиск." << std::endl;
#endif

    return shortestPaths[end].first;
}

template <typename T>
std::vector<T> ShortestPathFinder<T>::greedySearch() {
    double min;
    std::vector<T> result;
    result.reserve(this->numberOfEdges);
    //добавляем начальную вершину в стек
    result.push_back(this->start);

    T currentVertex = this->start;

    //проходимся по графу, пока не дойдем до конечной вершины
    while(currentVertex != this->end){
        T nextVertex;
        min = std::numeric_limits<double>::max();
        bool found = false;
#ifdef DEBUG
        std::cout << std::endl << "Состояние стека: ";
        for(auto &el : result){
            std::cout << el << " ";
        }
        std::cout << std::endl;
        std::cout << "Текущая вершина: " << currentVertex << std::endl;
        std::cout << "Начинаем рассматривать ее соседей: " << std::endl;
#endif

        //рассматриваем соседей текущей вершины
        for(auto &item : this->graph[currentVertex]){
#ifdef  DEBUG
            std::cout << "Смежная вершина: " << item.first << std::endl;
#endif
            //если сосед раньше не посещался и у него минимальный вес среди всех остальных соседей
            if(!visited[item.first] && item.second < min){
                min = item.second;
                nextVertex = item.first;
                found = true;
#ifdef DEBUG
                std::cout << "Ребро "<<currentVertex << "-" << item.first <<" имеет пока минимальный вес " << item.second << std::endl;
#endif
            }
#ifdef DEBUG
            else if(visited[item.first]){
                std::cout << "Вершина " << item.first << " уже посещалось" << std::endl;
            }
            else {
                std::cout << "Текущий минимальный вес " << min << " меньше веса " << item.second << " ребра " << currentVertex << "-" << item.first << std::endl;
            }
#endif
        }

        //посетили вершину
        visited[currentVertex] = true;

        //если нет смежных вершин или все они уже были посещены
        if(!found){
            //если стек не пуст, то берем следующий элемент
            if(!result.empty()){
                result.pop_back();
                currentVertex = result.back();
            }
#ifdef DEBUG
            std::cout << "Не посещенных смежных вершин не было найдено, возвращаемся к прошлой вершине" << std::endl;
#endif
            continue;
        }
#ifdef DEBUG
        std::cout << "Добавляем вершину " << nextVertex << " в стек c минимальным весом " << min << " и рассматриваем в следующий раз" << std::endl;
#endif
        //если же все таки смежную вершину нашли, то в следующий раз начинаем с неё
        currentVertex = nextVertex;
        result.push_back(currentVertex);
    }
#ifdef DEBUG
    std::cout << "Мы дошли до конечной вершины, заканчиваем поиск." << std::endl;
#endif
    return result;
}


int main() {
#ifdef DEBUG
    std::cout << "Введите данные, для окончания ввода #: " << std::endl;
#endif

    std::ifstream file("test.txt");
    ShortestPathFinder<TYPE> finder;
    finder.inputGraph(FILE_INPUT ? file : std::cin);
    std::vector<TYPE> out = ASTAR ? finder.aStarSearch() : finder.greedySearch();
    for(auto &item : out){
        std::cout << item;
#ifdef TAB
        std::cout << " ";
#endif
    }

    return 0;
}
