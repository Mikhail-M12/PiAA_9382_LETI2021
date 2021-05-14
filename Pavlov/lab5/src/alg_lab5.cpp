#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

#define TASK 1
#define DEBUG

using namespace std;

struct Node {
	unordered_map<char, Node*> next;	// рёбра, по которым можем перейти
#if TASK == 2
	vector<int> shifts;	// сдвиги подстрок в шаблоне
#endif
	Node* parent;	// родительская вершина (откуда пришли)
	Node* link;		// суффиксная ссылка
	Node* tLink;	// конечная ссылка
	char toParent;	// ребро, по которому пришли из родительской вершины
	bool terminal;	// является ли терминальной вершиной (признак конца шаблона)
	vector<int> ptnNum;		// номера шаблонов, в которые входит символ, по которому пришли
	int termPtnNum;

	Node(Node* pr = nullptr, const char toPr = 0) : parent(pr), toParent(toPr), link(nullptr), tLink(nullptr), terminal(false) {
		if (pr == nullptr || toPr == 0) { // если создаём корень (он ссылается на себя же)
			this->parent = this;
			this->link = this;
		}
	}

	// поиск следующей (для перехода) вершины при поиске в строке
	Node* getLink(const char& c) {
		if (this->next.find(c) != this->next.end()) { // если нашли путь по заданному символу из текущей вершины
			return this->next.at(c);
		}
		if (this->link == this) { // если дошли до корня, не найдя пути
			return this;
		}
		return this->link->getLink(c); // если не нашли путь, но ещё не в корне
	}

	~Node() {
		for (auto l : this->next) {
			delete l.second;
		}
	}
};

#ifdef DEBUG
void indent(int n) {
	while (n > 0) {
		cout << "\t";
		n--;
	}
}
#endif

// создание бора
Node* createBohr(const vector<pair<string, int>>& patterns) {
#ifdef  DEBUG
	cout << "\nПостроение бора\n";
#endif
	Node* bohr = new Node;	// корень бора
	for (auto& pt : patterns) {
		int ptnNum = find(patterns.begin(), patterns.end(), pt) - patterns.begin();
#ifdef  DEBUG
		cout << "\n-> Рассматривается " << ptnNum + 1 << "-й шаблон: " << pt.first << "\n";
#endif
		Node* cur = bohr;	// ищем путь, начиная с корня
		for (auto& c : pt.first) {
			if (cur->next.find(c) == cur->next.end()) { // если такого ребра ещё нет в cur->next, добавляем
				cur->next.insert({ c, new Node(cur, c) });
#ifdef  DEBUG
				cout << "\t-> В бор добавлена вершина, в которую ведёт текущий символ шаблона (" << c << ")\n";
#endif
			}
#ifdef  DEBUG
			else {
				cout << "\t-> Ребро (" << c << ") для текущего шаблона уже существует, выполняется переход по нему\n";
			}
#endif
			cur = cur->next[c]; // переходим по данному ребру
			cur->ptnNum.push_back(ptnNum);
		}
		// для конечной вершины
		cur->termPtnNum = ptnNum;
		cur->terminal = true;
#ifdef  DEBUG
		cout << "\t-> Вершина, в которую выполнен переход, является терминальной, закончено построение ветви бора\n";
#endif
#if TASK == 2
		cur->shifts.push_back(pt.second);
#ifdef  DEBUG
		cout << "-> Данный шаблон имеет сдвиг " << pt.second << " относительно начала шаблона с маской\n";
#endif
#endif
	}
#ifdef DEBUG
	cout << "\n";
#endif
	return bohr;
}

#ifdef  DEBUG
void charInfo(Node* n) {
	cout << n->toParent << ") из ";
	if (n->ptnNum.size() == 1) cout << "шаблона № " << n->ptnNum.at(0) + 1 << "\n";
	else if (n->ptnNum.size() > 1) {
		cout << "шаблонов № " << n->ptnNum.at(0) + 1;
		for (int i = 1; i < n->ptnNum.size(); i++) {
			cout << ", " << n->ptnNum.at(i) + 1;
		}
		cout << "\n";
	}
}
#endif

// нахождение суффиксных и конечных ссылок
void writeLinks(Node* bohr) {
#ifdef  DEBUG
	cout << "\nВычисление суффиксных и конечных ссылок\n";
#endif
	queue<Node*> front({ bohr });	// вершины одного уровня в боре
	while (!front.empty()) {
		Node* cur = front.front();
		front.pop();
		Node* curLink = cur->parent->link;	// взяли родительскую ссылку в качестве текущей
		const char& key = cur->toParent;	// запомнили символ, для которого ищем ссылку
		bool foundLink = true;	// по умолчанию ссылка находится
#ifdef  DEBUG
		cout << "\n-> Текущая вершина ";
		if (cur->link == cur) cout << "- корень\n";
		else {
			cout << "- (";
			charInfo(cur);
		}
#endif
		//------------------------------------------------------
		while (curLink->next.find(key) == curLink->next.end()) { // пока из суффиксной ссылки не найден переход по key
			if (curLink == bohr) {
#ifdef  DEBUG
				cout << "\t-> Суффиксные ссылки не найдены, ссылка установлена на корень\n";
#endif
				cur->link = bohr;	// если и из корня нет пути, то ссылку устанавливаем в корень
				foundLink = false;	// и ссылка, не равная корню, не была найдена
				break;
			}
			curLink = curLink->link;
		}
		if (foundLink) {
			curLink = curLink->next.at(key);	// это ссылка для key
			if (cur->parent == bohr) {
#ifdef  DEBUG
				cout << "\t-> Текущая вершина является началом слова, ссылка установлена на корень\n";
#endif
				cur->link = bohr;	// так или иначе, для вершин первого уровня ссылки ведут в корень
									// (если это не учесть, эти вершины будут ссылаться на себя)
			}
			else {
#ifdef  DEBUG
				cout << "\t-> Для текущей вершины найдена суффиксная ссылка (";
				charInfo(curLink);
#endif
				cur->link = curLink;
				Node* curTlink = cur->link;
				while (curTlink != bohr) {	// поиск конечной ссылки, если дошли до корня - её нет
					if (curTlink->terminal) {
#ifdef  DEBUG
						cout << "\t-> Для текущей вершины найдена конечная (сжатая) ссылка (";
						charInfo(curTlink);
#endif
						cur->tLink = curTlink;
						break;
					}
					curTlink = curTlink->link;
				}
			}
		}
		//------------------------------------------------------

		if (!cur->next.empty()) {	// добавляем новые вершины в очередь
			for (auto& nxt : cur->next) {
				front.push(nxt.second);
			}
		}
	}
}

pair<int, int> longestLinks(Node* bohr, Node* root, int& depth) {
	pair<int, int> longest = { 0, 0 };
#ifdef  DEBUG
	cout << "\n";
	indent(depth);
	cout << "-> Рассматривается ";
	if (bohr == root) {
		cout << "корень\n";
	}
	else {
		cout << "вершина (";
		charInfo(bohr);
	}
#endif
	Node* cur = bohr;
	while (cur->link != root) {
#ifdef  DEBUG
		indent(depth);
		cout << "\t-> Текущая суффиксная ссылка: (";
		charInfo(cur->link);
#endif
		longest.first++;
		cur = cur->link;
	}
	longest.first++;
#ifdef  DEBUG
	indent(depth);
	cout << "\t-> Текущая суффиксная ссылка - корень, поиск закончен\n";
#endif
	cur = bohr;
	while (cur->tLink != nullptr) {
#ifdef  DEBUG
		indent(depth);
		cout << "\t-> Текущая конечная (сжатая) ссылка: (";
		charInfo(cur->tLink);
#endif
		longest.second++;
		cur = cur->tLink;
	}
#ifdef  DEBUG
	indent(depth);
	cout << "\t-> Конечных ссылок нет, поиск закончен\n";
	indent(depth);
	cout << "-> Длина цепочки из суффиксных ссылок для данной вершины : " << longest.first << "\n";
	indent(depth);
	cout << "-> Длина цепочки из конечных ссылок для данной вершины: " << longest.second << "\n";
#endif
	for (auto& n : bohr->next) {
		pair<int, int> nextLon = longestLinks(n.second, root, ++depth);
		if (nextLon.first > longest.first) {
			longest.first = nextLon.first;
		}
		if (nextLon.second > longest.second) {
			longest.second = nextLon.second;
		}
	}
	depth--;
	return longest;
}

void ahoCor(const string& t, const vector<pair<string, int>>& patterns, vector<pair<int, int>>& res, int ptnLength = 0) {
	Node* bohr = createBohr(patterns);
	writeLinks(bohr);
#ifdef  DEBUG
	cout << "\n\nПоиск самых длинных цепочек из суффиксных и конечных (сжатых) ссылок\n";
#endif
	int depth = 0;
	pair<int, int> longest = longestLinks(bohr, bohr, depth);
	cout << "\nДлина наибольшей цепочки из суффиксных ссылок: " << longest.first << "\nДлина наибольшей цепочки из конечных ссылок: " << longest.second << "\n";
#ifdef  DEBUG
	cout << "\n\nПоиск вхождений шаблонов в строке\n";
#endif
	Node* cur = bohr;
	res.clear();
#if TASK == 2
	vector<int> tInd(t.length(), 0);
#endif

	for (int i = 0; i < t.length(); i++) {
		cur = cur->getLink(t.at(i));	// получили ссылку для перехода (для текущего символа строки)
#ifdef  DEBUG
		cout << "\n-> Текущий символ строки: " << t[i] << ", текущая вершина: ";
		if (cur == bohr) {
			cout << "корень\n";
		}
		else {
			cout << "(";
			charInfo(cur);
		}
#endif
		Node* tLink = cur->tLink;
		while (tLink != nullptr) {	// если у этой вершины есть конечная ссылка, записываем, что нашли соответствующий ей шаблон
#ifdef  DEBUG
			cout << "\t-> Найдена конечная ссылка (";
			charInfo(tLink);
#endif
#if TASK == 1
			res.push_back({ i - patterns.at(tLink->termPtnNum).first.length() + 2, tLink->termPtnNum + 1 });
#ifdef  DEBUG
			cout << "\t-> Вхождение " << patterns.at(tLink->termPtnNum).first << " добавлено в список результатов\n";
#endif
#elif TASK == 2
			for (auto& sh : tLink->shifts) {
				int idx = i - patterns.at(tLink->termPtnNum).first.length() - sh + 1;
				if (!(idx < 0)) {
					tInd.at(idx)++;
				}
			}
#ifdef  DEBUG
			cout << "\t-> Текущее состояние массива, в котором записано количество наложений шаблонов:\n\n\t-> ";
			for (auto& e : tInd) {
				cout << e << " ";
			}
			cout << "\n\n";
#endif
#endif
			tLink = tLink->tLink;	// и так, пока цепочка из конечных ссылок не прервётся
		}
		if (cur->terminal) {	// если терминальная - шаблон найден
#ifdef  DEBUG
			cout << "\t-> Текущая вершина - терминальная\n";
#endif
#if TASK == 1
			res.push_back({ i - patterns.at(cur->termPtnNum).first.length() + 2, cur->termPtnNum + 1 });
#ifdef  DEBUG
			cout << "\t-> Вхождение " << patterns.at(cur->termPtnNum).first << " добавлено в список результатов\n";
#endif
#elif TASK == 2
			for (auto& sh : cur->shifts) {
				int idx = i - patterns.at(cur->termPtnNum).first.length() - sh + 1;
				if (!(idx < 0)) {
					tInd.at(idx)++;
				}
			}
#ifdef  DEBUG
			cout << "\t-> Текущее состояние массива, в котором записано количество наложений шаблонов:\n\n\t-> ";
			for (auto& e : tInd) {
				cout << e << " ";
			}
			cout << "\n\n";
#endif
#endif

		}
	}

#if TASK == 2
	for (int i = 0; i < tInd.size(); i++) {
		if (tInd[i] == patterns.size() && i + ptnLength <= t.length()) {
			res.push_back({ i + 1, 0 });
		}
	}
#endif

	delete bohr;
}

#if TASK == 2
void preparePts(const string& p, const char& j, vector<pair<string, int>>& patterns) {
	int prev = 0;
	size_t delim;
	do {
		delim = p.find(j, prev);
		if (delim != prev && prev != p.length()) {
#ifdef  DEBUG
			cout << "Найден безмасочный шаблон: " << p.substr(prev, delim - prev) << ", смещение относительно начала исходного шаблона: " << prev << "\n";
#endif
			patterns.push_back({ p.substr(prev, delim - prev), prev });
		}
		prev = delim + 1;
	} while (delim != string::npos);
}
#endif

int main() {
	setlocale(LC_ALL, "rus");

	string t, p;
	char j;
	vector<pair<string, int>> pts;
	vector<pair<int, int>> res;
	int num = 0;

	cin >> t;
#if TASK == 1
	std::cin >> num;
	for (int i = 0; i < num; i++) {
		string s;
		std::cin >> s;
		pts.push_back({ s, 0 });
	}
	ahoCor(t, pts, res);
#elif TASK == 2
	cin >> p;
	cin >> j;
	preparePts(p, j, pts);
	ahoCor(t, pts, res, p.length());
#endif

	if (res.empty()) {
		cout << "\nПоиск не дал результатов\n";
	}
	else {
		sort(res.begin(), res.end());
		cout << "\nРезультаты поиска (индекс вхождения ";
#if TASK == 1
		cout << "в строке, порядковый номер шаблона):\n";
#elif TASK == 2
		cout << "шаблона в строку):\n";
#endif
		for (auto r : res) {
			cout << r.first;
#if TASK == 1
			cout << " " << r.second;
#ifdef DEBUG
			cout << " - " << pts.at(r.second - 1).first;
#endif
#endif
			cout << "\n";
		}
	}
	system("pause");
	return 0;
}