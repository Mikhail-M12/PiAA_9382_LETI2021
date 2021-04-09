#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>

using namespace std;


struct Edge {
	char top1; // вершина из которой выходит ребро
	char top2; // вершина в которую входит ребро
	double lenght; // длинна ребра
};

bool Sort(const pair<char, double >& a, const pair<char, double >& b) {
	return (a.second < b.second);
}

// сортирует смежные вершины по длине ребра, от меньшего к большему
void sort_lenght(map<char, vector<pair<char, double >>>& nodes) {
    for(auto& item : nodes) {
		sort(nodes[item.first].begin(), nodes[item.first].end(), Sort);
	}
}

// формирует список смежности вершин
void make_list(map<char, vector<pair<char, double >>>& nodes, vector<Edge>& edge_mass) {
	char top1;
	char top2;
	double lenght;
    // edge_mass.size() - кол-во ребер
	for (int i = 0; i < edge_mass.size(); i++) {
		top1 = edge_mass[i].top1;
		top2 = edge_mass[i].top2;
		lenght = edge_mass[i].lenght;
		nodes[top1].push_back(make_pair(top2, lenght));
	}
}

// рекурсивная функция поиска пути
void find_way(map<char, vector<pair<char, double >>>& nodes, vector<char>& way, char top1, char top2, bool& flag) {
	// добавляем вершину в путь
	std::cout << "Добавили вершину: " << top1 << "\n\n";
	way.push_back(top1);
	// если пришли в конечную вершину
	if (top1 == top2) {
		flag = true;
		return;
	}
	std::cout << "Список смежных вершин с " << top1  << '\n';
	for (int i = 0; i < nodes[top1].size(); i++) {
		std::cout << nodes[top1][i].first << " " << nodes[top1][i].second << '\n';
	}
	for (int i = 0; i < nodes[top1].size(); i++) {
		find_way(nodes, way, nodes[top1][i].first, top2, flag);
		if (flag) {
			return;
        }
		// удаляем вершину из пути
		std::cout << "\nУбрали вершину "<< nodes[top1][i].first << "\n\n";
		way.pop_back();
	}
}

// считывает ввод пользователя
void user_input(map<char, vector<pair<char, double >>>& nodes) {
	vector<Edge> edge_mass;
	Edge elem;
	char top1;
	char top2;
	top1 = ' ';
	double lenght;
	while (cin >> top1) {
		if (!top1 || top1 == '/') {
			break;
        }
		cin >> top2;
		cin >> lenght;
		elem.top1 = top1;
		elem.top2 = top2;
		elem.lenght = lenght;
		// добавляем в массив
		edge_mass.push_back(elem);
	}

	make_list(nodes, edge_mass);
}

int main() {
	char top1;
	char top2;
	bool flag = false;
	vector<char> way;
	map<char, vector<pair<char, double >>> nodes;
	std::cout << "Для завершения ввода введите /" << '\n';
	cin >> top1;
	cin >> top2;

	user_input(nodes);
	sort_lenght(nodes);

	find_way(nodes, way, top1, top2, flag);

	for (int i = 0; i < way.size(); i++) {
		cout << way[i];
	}

	return 0;
}
