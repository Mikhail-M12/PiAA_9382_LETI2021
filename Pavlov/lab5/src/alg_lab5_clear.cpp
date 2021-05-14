#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

#define TASK 1

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

// создание бора
Node* createBohr(const vector<pair<string, int>>& patterns) {
	Node* bohr = new Node;	// корень бора
	for (auto& pt : patterns) {
		int ptnNum = find(patterns.begin(), patterns.end(), pt) - patterns.begin();
		Node* cur = bohr;	// ищем путь, начиная с корня
		for (auto& c : pt.first) {
			if (cur->next.find(c) == cur->next.end()) { // если такого ребра ещё нет в cur->next, добавляем
				cur->next.insert({ c, new Node(cur, c) });
			}
			cur = cur->next[c]; // переходим по данному ребру
			cur->ptnNum.push_back(ptnNum);
		}
		// для конечной вершины
		cur->termPtnNum = ptnNum;
		cur->terminal = true;
#if TASK == 2
		cur->shifts.push_back(pt.second);
#endif
	}
	return bohr;
}

// нахождение суффиксных и конечных ссылок
void writeLinks(Node* bohr) {
	queue<Node*> front({ bohr });	// вершины одного уровня в боре
	while (!front.empty()) {
		Node* cur = front.front();
		front.pop();
		Node* curLink = cur->parent->link;	// взяли родительскую ссылку в качестве текущей
		const char& key = cur->toParent;	// запомнили символ, для которого ищем ссылку
		bool foundLink = true;	// по умолчанию ссылка находится

		//------------------------------------------------------
		while (curLink->next.find(key) == curLink->next.end()) { // пока из суффиксной ссылки не найден переход по key
			if (curLink == bohr) {
				cur->link = bohr;	// если и из корня нет пути, то ссылку устанавливаем в корень
				foundLink = false;	// и ссылка, не равная корню, не была найдена
				break;
			}
			curLink = curLink->link;
		}
		if (foundLink) {
			curLink = curLink->next.at(key);	// это ссылка для key
			if (cur->parent == bohr) {
				cur->link = bohr;	// так или иначе, для вершин первого уровня ссылки ведут в корень
									// (если это не учесть, эти вершины будут ссылаться на себя)
			}
			else {
				cur->link = curLink;
				Node* curTlink = cur->link;
				while (curTlink != bohr) {	// поиск конечной ссылки, если дошли до корня - её нет
					if (curTlink->terminal) {
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
	Node* cur = bohr;
	while (cur->link != root) {
		longest.first++;
		cur = cur->link;
	}
	longest.first++;
	cur = bohr;
	while (cur->tLink != nullptr) {
		longest.second++;
		cur = cur->tLink;
	}
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
	int depth = 0;
	pair<int, int> longest = longestLinks(bohr, bohr, depth);
	cout << "\nДлина наибольшей цепочки из суффиксных ссылок: " << longest.first << "\nДлина наибольшей цепочки из конечных ссылок: " << longest.second << "\n";
	Node* cur = bohr;
	res.clear();
#if TASK == 2
	vector<int> tInd(t.length(), 0);
#endif

	for (int i = 0; i < t.length(); i++) {
		cur = cur->getLink(t.at(i));	// получили ссылку для перехода (для текущего символа строки)
		Node* tLink = cur->tLink;
		while (tLink != nullptr) {	// если у этой вершины есть конечная ссылка, записываем, что нашли соответствующий ей шаблон
#if TASK == 1
			res.push_back({ i - patterns.at(tLink->termPtnNum).first.length() + 2, tLink->termPtnNum + 1 });
#elif TASK == 2
			for (auto& sh : tLink->shifts) {
				int idx = i - patterns.at(tLink->termPtnNum).first.length() - sh + 1;
				if (!(idx < 0)) {
					tInd.at(idx)++;
				}
			}
#endif
			tLink = tLink->tLink;	// и так, пока цепочка из конечных ссылок не прервётся
		}
		if (cur->terminal) {	// если терминальная - шаблон найден
#if TASK == 1
			res.push_back({ i - patterns.at(cur->termPtnNum).first.length() + 2, cur->termPtnNum + 1 });
#elif TASK == 2
			for (auto& sh : cur->shifts) {
				int idx = i - patterns.at(cur->termPtnNum).first.length() - sh + 1;
				if (!(idx < 0)) {
					tInd.at(idx)++;
				}
			}
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
#endif
			cout << "\n";
		}
	}
	system("pause");
	return 0;
}