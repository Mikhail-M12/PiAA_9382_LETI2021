#include <iostream>
#include <vector>
#include <algorithm>
#include <map>


struct Edge {
    char top1; // вершина из которой выходит ребро
    char top2; // вершина в которую входит ребро
    double length; // длинна ребра
};

bool Sort(const std::pair<char, double >& a, const std::pair<char, double >& b) {
    return a.second < b.second;
}

// сортирует смежные вершины по длине ребра, от меньшего к большему
void sort_length(std::map<char, std::vector<std::pair<char, double >>>& nodes) {
    for(const auto& item : nodes) {
	sort(nodes[item.first].begin(), nodes[item.first].end(), Sort);
    }
}

// формирует список смежности вершин
void make_list(std::map<char, std::vector<std::pair<char, double >>>& nodes, std::vector<Edge>& edge_mass) {
    char top1;
    char top2;
    double length;
    int d = edge_mass.size();
    for(auto& i: edge_mass) {
	top1 = i.top1;
	top2 = i.top2;
	length = i.length;
	nodes[top1].push_back(std::make_pair(top2, length));
    }
}

// рекурсивная функция поиска пути
void find_way(std::map<char, std::vector<std::pair<char, double >>>& nodes, std::vector<char>& way, char top1, char top2, bool& flag) {
    // добавляем вершину в путь
    std::cout << "Добавили вершину: " << top1 << "\n\n";
    way.push_back(top1);
    // если пришли в конечную вершину
    if (top1 == top2) {
	flag = true;
	return;
    }
    std::cout << "Список смежных вершин с " << top1  << '\n';
    for(auto& i: nodes[top1]) {
	std::cout << i.first << " " << i.second << '\n';
    }
    for(auto& j: nodes[top1]) {
	find_way(nodes, way, j.first, top2, flag);
	if (flag) {
	    return;
        }
	// удаляем вершину из пути
	std::cout << "\nУбрали вершину "<< j.first << "\n\n";
	way.pop_back();
    }
}

// считывает ввод пользователя
void user_input(std::map<char, std::vector<std::pair<char, double >>>& nodes) {
    std::vector<Edge> edge_mass;
    Edge elem;
    char top1;
    char top2;
    top1 = ' ';
    double length;
    while (std::cin >> top1) {
	if (!top1 || top1 == '/') {
	    break;
        }
	std::cin >> top2;
	std::cin >> length;
	elem.top1 = top1;
	elem.top2 = top2;
	elem.length = length;
	// добавляем в массив
	edge_mass.push_back(elem);
    }

    make_list(nodes, edge_mass);
}

int main() {
    char top1, top2;
    bool flag = false;
    std::vector<char> way;
    std::map<char, std::vector<std::pair<char, double >>> nodes;
    std::cout << "Для завершения ввода введите /" << '\n';
    std::cin >> top1;
    std::cin >> top2;

    user_input(nodes);
    sort_length(nodes);

    find_way(nodes, way, top1, top2, flag);
    for(auto& i: way) {
	std::cout << i;
    }

    return 0;
}
