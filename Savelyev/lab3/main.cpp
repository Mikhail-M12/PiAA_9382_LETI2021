#include <iostream>
#include <vector>
#include <limits.h>
#include <algorithm>
#include <stack>

using namespace std;

// компраратор для сортировки по второй вершине
bool compare(pair <pair <int, int>, int> a, pair <pair <int, int>, int> b){
    if (a.first.first == b.first.first) {
		if (a.first.second < b.first.second) {
            return true;
        }
	}
    return false;
}

// Выводит ответ
void print_answer(string node, vector<vector<int>>& graph, int N, string from, string to, vector<int> edge_weight) {
    std::vector<pair<pair<int, int>, int> > result;
    for (int q = 0; q < node.length(); q++) { // перебор вершин
        vector <int> edge_mass; // сколько ребер выходит из node[q] и их индексы
        for (int j = 0; j < N; j++) {
            if (from[j] == node[q]) {
                edge_mass.push_back(j);
            }
        }
        vector <int> mass_to; // в какую верш идет ребро исходящие из node[q] отсортир в алф
        // ищем вершины в которые входят ребра node[q]
        for (int i = 0; i < edge_mass.size(); i++) {
            for (int j = 0; j < node.length(); j++) {
                if (node[j] == to[edge_mass[i]]) {
                    mass_to.push_back(j);
                }
            }
        }
        // заносим результат в массив
        for (int i = 0; i < edge_mass.size(); i++) {
            if (graph[q][mass_to[i]] < 0) {
                result.push_back(make_pair(make_pair(q, mass_to[i]), 0));
            }
            else {
                result.push_back(make_pair(make_pair(q, mass_to[i]), graph[q][mass_to[i]]));
            }
        }
    }
    sort(result.begin(), result.end(), compare); // сортируем
    // выводим результа
    for (auto i: result) {
        std::cout << node[i.first.first] << ' ';
        std::cout << node[i.first.second] << ' ';
        std::cout << i.second << '\n';
    }
}

// Поиск в глубину
bool dfs(vector<vector<int>> seti, int start, int finish, vector<int>&previus_node, string node){
    // массив посещенных вершин
    vector<bool> vis_mass(node.length(), 0);
    // стек
    stack <int> node_stack;
    // кладем исходную вершину в стек
    node_stack.push(start);
    // отмечаем стартовую вершину как посещенную
    vis_mass[start] = true;
    previus_node[start] = -1;
    // пока стек не пуст и не найден финиш
    std::cout << '\n';
    bool flag = false;
   while (!node_stack.empty() && (vis_mass[finish] == false)) {
		int i = node_stack.top();
		node_stack.pop();
        std::cout << "Рассматриваемая вершина: " << node[i] << '\n';
        // проверяем все смежные с  node[i] не посещенные вершины
        for(int j = 0 ; j < node.length(); j++){
            if(seti[i][j] > 0 && vis_mass[j] == false) {
                node_stack.push(j); // добавляем в стек
                previus_node[j] = i; // хранит своего предка
                vis_mass[j] = true; // отмечаем как посещенную
                std::cout << "\tНе посещенная вершина смежная c рассматриваемой по которой можно пустить поток: " << node[j] << '\n';
                flag = true;
            }
        }
        if(!flag) {
            std::cout << "\tНет вершин смежных с рассматриваемой по которым можно пустить поток" << "\n";
        }
        else {
            flag = false;
        }
    }
    // если финиш был посещен
    if(vis_mass[finish] == true) {
        std::cout << "Найденный путь: " << '\n';
        string Way;
        for (int i = finish; i != start; i = previus_node[i]) {
            Way = node[i] + Way;
        }
        Way = node[start] + Way;
        std::cout << Way << "\n\n";
        return true;
    }
    // если финиш не был достигнут

    std::cout << "Путь не найден" << "\n\n";
    return false;
}

// Заполняем начальную сеть
void init_graph(string node, vector<vector<int>>& graph, int N, string from, string to, vector<int> edge_weight) {
    // поиск всех ребер, ведущих из вершины node[q]
	for (int q = 0; q < node.length(); q++) { // перебор вершин
		vector <int> edge_mass; // сколько ребер выходит из node[q] и их индексы
		for (int j = 0; j < N; j++) {
			if (from[j] == node[q]) {
				edge_mass.push_back(j);
			}
		}
        //поиск в строке node[q] вершины, в которую ведут ребра из вектора edge_mass
		vector <int> mass_to; // в какую верш идет ребро исходящие из node[q] отсортир в алф
        // номер верш в отсортированном node
        // ищем вершины в которые входят ребра node[q]
		for (int i = 0; i < edge_mass.size(); i++) {
			for (int j = 0; j < node.length(); j++) {
				if (node[j] == to[edge_mass[i]]) {
					mass_to.push_back(j);
                }
			}
		}
        // заполняем список смежности отсортированный в алф порядке
		for (int i = 0; i < edge_mass.size(); i++) {
			graph[q][mass_to[i]] = edge_weight[edge_mass[i]];
		}

	}
    // на выходе двумерный массив отсортированный в алф порядке где каждой вершине соответствует вектор ребер с проходимостью
}

// Алгоритм Форда Фалкерсона
int f_f(vector<vector<int>>& graph, vector<vector<int>>& seti, int start, int finish, string node) {
	for (int u = 0; u < node.length(); u++)
		for (int v = 0; v < node.length(); v++) {
			seti[u][v] = graph[u][v];
			graph[u][v] = 0;
		}

	int max_flow = 0;
    // массив для хранения пути
	vector <int> previus_node(node.length(), 0);
    // увеличивается поток, пока есть путь от истока к стоку
	while (dfs(seti, start, finish, previus_node, node)) {
		int path_flow = INT_MAX;
        // находим поток через найденный путь
		for (int v = finish; v != start; v = previus_node[v]) {
			path_flow = min(path_flow, seti[previus_node[v]][v]); // ребро между родителем и потомком
            std::cout << "Максимальный поток который можно пустить между вершинами " << node[previus_node[v]] << " и " << node[v] << ": " << path_flow <<'\n';
		}
		std::cout << "Максимальный поток по данному пути : " << path_flow << "\n";
		// обновление пропускной способности каждого ребра
		for (int v = finish; v != start; v = previus_node[v]) {
			seti[previus_node[v]][v] -= path_flow; // ребро между родителем и сыном
			seti[v][previus_node[v]] += path_flow; // ребро между родителем и сыном в обратном направлении
			graph[previus_node[v]][v] += path_flow; // ребро между родителем и сыном
			graph[v][previus_node[v]] -= path_flow; // ребро между родителем и сыном в обратном направлении
		}
		max_flow += path_flow;
	}

	return max_flow;
}

int main() {
    char start;  //исток
	char finish; //сток
	int N = 0; //количество ориентированных рёбер графа
	string from; // вершины из которых выходят ребра
	string to; // вершины в которые входят ребра
	string node; // все вершины графа
    cin >> N >> start >> finish; // Кол-во, исток, сток
	vector <int> edge_weight; // веса ребер
	node = node + start;
    char node_from;
	char node_to;
    int weigth;
    // Ввод пользователя
	for (int i = 0; i < N; i++) {
		cin >> node_from; // откуда
		cin >> node_to; // куда
		cin >> weigth; // пропускная способность
		from = from + node_from;
		to = to + node_to;
		edge_weight.push_back(weigth);
        // если вершины нет в node
		if (node.find(node_to) == string::npos) {
		    node = node + node_to;
        }
	}
    // отсортировали в алфавитном порядке
    sort(node.begin(), node.end());

	vector<vector<int>> graph(node.length(), vector<int>(node.length(), 0));
    init_graph(node, graph, N, from, to, edge_weight);

	int start_ind = 0;
	int finish_ind = 0;

	for (int i = 0; i < node.length(); i++) {
        // находим индекс старта в отсортированном массиве
		if (node[i] == start) { // равен истору
		      start_ind = i;
        }
        // находим индекс финиша в отсортированном массиве
		else if (node[i] == finish) { // равен стоку
		      finish_ind = i;
        }
	}

	vector<vector<int> > seti(node.length(), vector<int>(node.length(), 0)); // двумерный массив
    // алгоритм Форда Фалкерсона
	int max_flow = f_f(graph, seti, start_ind, finish_ind, node);

    std::cout <<"Максимальный поток в сети: " << max_flow << '\n';
    print_answer(node, graph, N, from, to, edge_weight);
	return 0;
}
