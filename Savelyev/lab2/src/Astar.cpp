#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <ctime>



struct Edge {
    char top1; // вершина из которой выходит ребро
    char top2; // вершина в которую входит ребро
    double lenght; // длинна ребра
};

// формирует список смежности вершин
void make_list(std::map<char, std::vector<std::pair<char, double >>>& nodes, std::vector<Edge>& edge_mass) {
	char top1;
	char top2;
	double lenght;
    // edge_mass.size() - кол-во ребер
	for (int i = 0; i < edge_mass.size(); i++) {
		top1 = edge_mass[i].top1;
		top2 = edge_mass[i].top2;
		lenght = edge_mass[i].lenght;
		nodes[top1].push_back(std::make_pair(top2, lenght));
	}
}

// суммирует все ребра в графе
int sum_edge(std::map<char, std::vector<std::pair<char, double >>>& nodes) {
    int sum = 0;
    for(const auto& item : nodes) {
        int a = nodes[item.first].size();
        for (int j = 0; j < a; j++) {
            sum += nodes[item.first][j].second;
        }
    }
    return sum + 1;
}

// выводит ответ
void print_answer(std::map <char, char> previous_top, char top) {
    char last = top;
    std::vector<char> my_vector;
    // перебираем вершина от последней к первой
    while (previous_top[top] != '*') {
        //заносим в массив
        my_vector.push_back(previous_top[top]);
        top = previous_top[top];
    }
    // выводи вершины в правильном порядке
    for (auto it = my_vector.rbegin(); it != my_vector.rend(); ++it) {
        std::cout << *it;
    }
    // последняя вершина
    std::cout << last << '\n';

}

// переназначеам расстояние до вершин
void change_distance(std::map<char, std::vector<std::pair<char, double >>> nodes, std::priority_queue<std::pair<double, char>>& top_queue, std::map <char, double>& distance , std::map <char, char>& previous_top, char top, char top2) {
    // перебираем все вершины смежные с top
    for (int i = 0; i < nodes[top].size(); i++) {
        // если нашли путь короче предыдущего, заменяем
        if (distance[nodes[top][i].first] > distance[top] + nodes[top][i].second) {
            distance[nodes[top][i].first] = distance[top] + nodes[top][i].second;
            // меняем предыдущую вершину
            std::cout << "Длинна кратайшего пути до " << nodes[top][i].first << " изменена на " << distance[top] + nodes[top][i].second  <<'\n';
            previous_top[nodes[top][i].first] = top;
            // добовляем в очередь
            top_queue.push(std::make_pair(-(distance[nodes[top][i].first] + (int)top2 - (int)nodes[top][i].first), nodes[top][i].first));
            std::cout << "Занесли в очередь " << nodes[top][i].first << " f(x) = " << (distance[nodes[top][i].first] + (int)top2 - (int)nodes[top][i].first) << "\n\n";
        }
    }
}

// Алгорит А*
void AStar(std::map<char, std::vector<std::pair<char, double >>>& nodes, char top1, char top2) {
    std::priority_queue<std::pair<double, char>> top_queue;
    std::map <char, double> distance ;
    std::map <char, char> previous_top;
    std::map <char, bool> visited_top;
    int current_top;
    int min;

    double max_distance  = sum_edge(nodes);

    // заполняем map
    for(const auto& item : nodes) {
        distance[item.first] = max_distance ;
    }

    for(const auto& item : nodes) {
        visited_top[item.first] = false;
    }

    for(const auto& item : nodes) {
        previous_top[item.first] = '!';
    }

    distance[top2] = max_distance ;
    visited_top[top2] = false;
    previous_top[top2] = '!';
    distance [top1] = 0;
    previous_top[top1] = '*';

    // добовляем стартовую вершину в очередь
    top_queue.push(std::make_pair(0, top1));
    std::cout << "Занесли в очередь " << top1 << " f(x) = "<< 0 << "\n\n";
    // пока очередь не опустеет
    while (!top_queue.empty()) {
        while (1) {
            // если очередь пуста
            if (top_queue.empty()) {
                break;
            }
            // считываем вершину из очереди
            std::pair<double, char> current_min = top_queue.top();
            top_queue.pop();
            current_top = current_min.second;
            min = -current_min.first;
            std::cout << "Извлекли из очереди " << (char)current_top << " f(x) = " << min << "\n\n";
            // если вершина была посещена
            if (!visited_top[current_top]) {
                break;
            }
        }
        if (distance[top2] < min) {
            break;
        }
        // отмечаем вершину как посещенную
        visited_top[current_top] = true;
        change_distance (nodes, top_queue, distance , previous_top, current_top, top2);
    }
    // выводим ответ
    print_answer(previous_top, top2);
}

// считывает ввод пользователя
void user_input(std::map<char, std::vector<std::pair<char, double >>>& nodes) {
    std::vector<Edge> edge_mass;
	Edge elem;
	char top1;
	char top2;
	top1 = ' ';
	double lenght;
	while (std::cin >> top1) {
		if (!top1 || top1 == '/') {
			break;
        }
		std::cin >> top2;
		std::cin >> lenght;
		elem.top1 = top1;
		elem.top2 = top2;
		elem.lenght = lenght;
        // добавляем в массив
		edge_mass.push_back(elem);
	}
	make_list(nodes, edge_mass);
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    char top1, top2, top3;
    std::cout << "Для завершения ввода введите /" << '\n';
    std::cin >> top1;
    std::cin >> top2;
    std::cin >> top3;
    std::map<char, std::vector<std::pair<char, double >>> nodes;
    user_input(nodes);
    // выбираеv какая вершина будет финишом
    std::cout << rand() % 2 << '\n';
    if(rand() % 2) {
        AStar(nodes, top1, top2);
    }
    else {
        AStar(nodes, top1, top3);
    }
    return 0;
}
