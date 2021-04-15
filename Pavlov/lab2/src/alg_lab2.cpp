// Метка CUSTOM_QUEUE определяет, используется ли
// std::priority_queue или пользовательский тип PriorQueue
// OUTPUT включает вывод промежуточной информации

#define CUSTOM_QUEUE
//#define OUTPUT

#include <iostream>
#include "vertex.h"

#ifdef CUSTOM_QUEUE
#include "prior_queue.h"
#else
#include <queue>
#endif

// Компаратор приоритетов вершин для std::prioirity_queue
#ifndef CUSTOM_QUEUE
class AStarPredicate {
public:
	bool operator() (Vertex* v1, Vertex* v2)
	{
		return v2->cmp(*v1);
	}
};
#endif

// Класс, содержащий список веришн графа, имена вершин старта и финиша,
// а также необходимые для работы с графом операции
class Navigator {
private:
	std::string sourceName;
	std::string destName;
	std::vector <Vertex*>* vertices;

	Navigator() : vertices(nullptr) {
		getInput();
	}

	~Navigator() {
		for (auto v : *vertices) {
			delete v;
		}
		if (vertices != nullptr) {
			delete vertices;
		}
	}

	// Вычисление пути от начальной вершины до конечной
	void retrievePath(Vertex* dest, std::string& path) {
#ifdef OUTPUT
		std::cout << "\nВосстановление пути от начальной вершины до конечной\nКонечная вершина: " << dest->name << "\n";
#endif
		path = dest->name + path;
		// Просмотр вершин, начиная с конечной и заканчивая начальной
		while (dest->parent != nullptr) {
			dest = dest->parent;
#ifdef OUTPUT
			std::cout << "Предыдущая вершина: " << dest->name << "\n";
#endif
			path = dest->name + path;
		}
	}

	void restoreGraph() {
		for (int j = 0; j < vertices->size(); j++) {
			auto i = vertices->at(j);
			i->distance = 0.0;
			i->func = 0.0;
			i->closed = false;
			i->parent = nullptr;
		}
	}

	// Поиск индекса вершины с указанным именем
	int searchIndex(std::vector<Vertex*>* arr, std::string name) {
		if (arr->empty()) {
			return -1;
		}
		std::vector<Vertex*>::iterator v;
		int i = arr->size() - 1;
		for (v = arr->end() - 1; v >= arr->begin(); v--) {
			if ((*v)->name == name) {
				return i;
			}
			i--;
			if (v == arr->begin()) {
				break;
			}
		}
		return i;
	}

	// Получение исходных данных
	void getInput() {
		// Список вершин графа - новый объект
		vertices = new std::vector<Vertex*>(0);
		// Переменные для записи информации о рёбрах
		std::string main, adj;
		double weight;
		// Добавление начальной и конечной вершин
		std::cout << "Для остановки ввода вершин нажать Ctrl+Z, затем Enter\n";
		std::cout << "\nПожалуйста, введите исходную и конечную вершины ( <вершина1> <вершина2>): ";
		std::cin >> sourceName >> destName;
		system("cls");
		vertices->push_back(new Vertex(sourceName));
#ifdef OUTPUT
		std::cout << "Вершина " << vertices->back()->name << " добавлена в граф\n";
#endif
		vertices->push_back(new Vertex(destName));
#ifdef OUTPUT
		std::cout << "Вершина " << vertices->back()->name << " добавлена в граф\n";
#endif

		while (true) {
			std::cout << "\nПожалуйста, введите ребро ( <вершина1> <вершина2> <расстояние> ): ";
			std::cin >> main >> adj >> weight;
			system("cls");
			if (!std::cin) {
				break;
			}

			int m = searchIndex(vertices, main);
			int i = searchIndex(vertices, adj);
			// Если не существует второй указанной вершины, создать её и поместить в список
			if (i < 0) {
				i = vertices->size();
				vertices->push_back(new Vertex(adj));
#ifdef OUTPUT
				std::cout << "Вершина " << vertices->back()->name << " добавлена в граф\n";
#endif
			}
			// Если не существует первой указанной вершины, создать её и поместить в список,
			// указав смежную (вторую) вершину и вес инцидентного им ребра
			if (m < 0) {
				vertices->push_back(new Vertex(main, vertices->at(i), weight));
#ifdef OUTPUT
				std::cout << "Вершина " << vertices->back()->name << " добавлена в граф\n";
#endif
			}
			// Если она уже есть в списке, только указать смежную вершину
			else {
				vertices->at(m)->adjacent.push_back({ vertices->at(i), weight });
			}
#ifdef OUTPUT
			std::cout << "Ребро " << main << "--" << adj << " с весом " << weight << " добавлено в граф\n";
#endif
		}
	}

	// Жадный алгоритм
	std::string greedy() {
		std::vector <Vertex*> stack;

		Vertex* cur = vertices->at(0);
		std::string res = "";

		// Главный цикл поиска пути
		do {
			int index = 0;
			// Возврат до первой вершины, у которой остались непосещённые смежные
			while (cur->noAdjacent()) {
				cur = stack.back();
				stack.pop_back();
			}
			stack.push_back(cur);
#ifdef OUTPUT
			std::cout << "\nПереход к вершине " << cur->name << "\n";
#endif
			// Поиск кратчайшего из путей до смежных вершин
			double shortest = 0.0;
			for (int i = 0; i < cur->adjacent.size(); i++) {
				if (!cur->adjacent.at(i).first->closed) {
					shortest = cur->adjacent.at(i).second;
					index = i;
					break;
				}
			}
			for (int i = index + 1; i < cur->adjacent.size(); i++) {
				if ( (!cur->adjacent.at(i).first->closed) && (cur->adjacent.at(i).second < shortest ||
					(cur->adjacent.at(i).second == shortest && (cur->adjacent.at(i).first->name < cur->adjacent.at(index).first->name)))) {
					shortest = cur->adjacent.at(i).second;
					index = i;
				}
			}
#ifdef OUTPUT
			std::cout << "\n\tБлижайшая вершина: " << cur->adjacent.at(index).first->name << ", длина пути: " << shortest << "\n";
#endif
			// Переход к следующей вершине
			cur->adjacent.at(index).first->closed = true;
			cur = cur->adjacent.at(index).first;
		} while (cur->name != destName);
#ifdef OUTPUT
		std::cout << "\nПуть до конечной вершины обнаружен, завершение поиска\n";
#endif
		stack.push_back(cur);

		// Запись пути в результирующую строку
		for (auto& i : stack) {
			res = res + i->name;
		}

		return res;
	}

	// Алгоритм A*
	std::string aStar() {
		clock_t beg = clock();
	// Инициализация очереди и строки с указанием пути
#ifdef CUSTOM_QUEUE
		PriorQueue openList = PriorQueue(vertices->at(0));
#else
		std::priority_queue <Vertex*, std::vector<Vertex*>, AStarPredicate> openList;
		openList.push(vertices->at(0));
#endif

		std::string res = "";
		Vertex* cur;
		// Главный цикл поиска пути
		do {
			// Извлечение самого приоритетного элемента, установление метки закрытой вершины (уже просмотрено)
			cur = openList.top();
			openList.pop();
#ifdef OUTPUT
			std::cout << "\nИз открытого списка извлечена вершина " << cur->name << "\n";
#endif
			// Просмотр списка инцидентных вершин
			for (auto& v : cur->adjacent) {
#ifdef OUTPUT
				std::cout << "\n\tРассматривается смежная вершина " << v.first->name << "\n";
#endif
				const double f = cur->distance + v.second + destName[0] - v.first->name[0];
#ifdef OUTPUT
				std::cout << "\tПриоритет данной смежной вершины равен " << f << "\n";
#endif
				// Если оценка выгодности пути меньше имеющейся для данной вершины, обновить путь
				if (f < v.first->func) {
#ifdef OUTPUT
					std::cout << "\tТекущий приоритет выше существующего (" << v.first->func << " > " << f << "), обновление пути\n";
#endif
					v.first->distance = f - (destName[0] - v.first->name[0]);
					v.first->func = f;
					v.first->parent = cur;
				}
				// Если вершины ещё нет в открытом списке, добавить её в него
				else if (v.first->func == 0) {
#ifdef OUTPUT
					std::cout << "\tВершины ещё нет в открытом списке, добавлена в него  и записана в путь\n";
#endif
					v.first->distance = f - (destName[0] - v.first->name[0]);
					v.first->func = f;
					v.first->parent = cur;
					openList.push(v.first);
				}
				else {
#ifdef OUTPUT
					std::cout << "\tТекущий путь менее выгоден, чем уже существующий, и не рассматривается\n";
#endif
				}
			}
		} while (cur->name != destName);
#ifdef OUTPUT
		std::cout << "\nПуть до конечной вершины обнаружен, завершение поиска\n";
#endif
		// Запись пути в результирующую строку
		retrievePath(vertices->at(1), res);
		clock_t end = clock();
		std::cout << end - beg<<"\n";
		return res;
	}
public:
	static void route() {
		Navigator* n = new Navigator();
		std::cout << "Жадный Путь от начальной вершины до конечной: " << n->greedy() << "\n";
		n->restoreGraph();
		std::cout << "А* Путь от начальной вершины до конечной: " << n->aStar() << "\n";
		delete n;
	}
};

int main() {
	int command = 0;
	setlocale(LC_ALL, "Ru");
	std::cout << "Проложить маршрут? 0 - нет, 1 - да\n";
	std::cin >> command;
	switch (command) {
	case 0:
		std::cout << "Завершение работы\n";
		break;
	case 1:
		Navigator::route();
		break;
	default:
		std::cout << "Неизвестная команда, выход из программы\n";
	}
	return 0;
}