#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

//#define INFO

class Bor {
public:
	Bor* parent; // родительская вершина 
	Bor* SufLink; // суффиксная ссылка
	Bor* EndLink; // конечная ссылка
	char ToParent; // ребро, по которому попали из родительской вершины
	bool IsTerminal; // терминальая вершина или нет (конец шаблона)
	std::map<char, Bor*> next; //ребра по которым можно переходить
	std::vector<int> NumPattern; // номера шаблонов, в которые входит символ, по которому пришли
	int TermNumPattern;
	Bor(Bor* parent = nullptr, char ToPrnt = 0) :parent(parent), ToParent(ToPrnt), SufLink(nullptr),EndLink(nullptr), IsTerminal(false) {
		if (parent == nullptr || ToPrnt == 0) { // создание корня
			this->parent = this;
			this->SufLink = this;
		}
	}
	~Bor() {
		for (auto i : this->next) {
			delete i.second;
		}
	}
	Bor* GetLink(const char& symbol) {
		if (this->next.find(symbol) != this->next.end()) { // если есть путь по заданному символу из текущей вершины
				return this->next.at(symbol);
		}
		if (this->SufLink == this) { // если в корне и путь не найден
				return this;
		}
		return this->SufLink->GetLink(symbol); // не в корне и путь не найден
	}
};

Bor* CreateBor(const std::vector<std::pair<std::string,int>>&patterns);
void Automaton(Bor* bor);
std::pair<int, int> LongestChain(Bor* bor, Bor* root, int&depth);
void AhoCorasick(const std::string& t, const std::vector<std::pair<std::string, int>>& patterns,std::vector<std::pair<int, int>>& result);

Bor* CreateBor(const std::vector<std::pair<std::string, int>>&patterns) { //создание бора
#ifdef INFO
	std::cout << "\nНачало построения бора";
#endif
	Bor* bor = new Bor(); // корень бора
	for (auto& pt : patterns) {
		int NumPattern = find(patterns.begin(),patterns.end(), pt) - patterns.begin();
#ifdef INFO
		std::cout << "\nРассматривается " << NumPattern + 1 <<" шаблон: " << pt.first << '\n';
#endif
		Bor* current = bor; // поиск пути, начиная от корня
		for (auto& c : pt.first) {
			if (current->next.find(c) == current->next.end())
			{ //если такого ребра нет, то добавляется
				current->next.insert({ c, new Bor(current,c) });
#ifdef INFO
				std::cout << "В бор добавлена вершина, куда ведёт символ(" << c << ")\n";
#endif
			}
			else {
#ifdef INFO
				std::cout << "Ребро (" << c << ") для текущего шаблона уже существует. Выполнение перехода.\n";
#endif
			}
			current = current->next[c]; // переход по данному ребру
			current->NumPattern.push_back(NumPattern);
		}
		current->TermNumPattern = NumPattern; // для конечной вершины
		current->IsTerminal = true;
#ifdef INFO
		std::cout << "Вершина является терминальной, закончено построение ветви бора.\n";
#endif
	}
	return bor;
}

void Automaton(Bor* bor) { //вычисление суф. и конечных ссылок(построение автомата)
#ifdef INFO
	std::cout << "\n\nВычисление суффиксных и конечных ссылок:\n";
#endif
	std::queue<Bor*> front({ bor }); // вершины одного уровня в боре
	while (!front.empty()) {
		Bor* current = front.front();
		front.pop();
		Bor* CurrentLink = current->parent->SufLink; // родительская ссылка стала текущей
		const char& key = current->ToParent; // сохраняется символ, для которого ищется ссылка
		bool IsFound = true;
		while (CurrentLink->next.find(key) == CurrentLink->next.end()) {
			if (CurrentLink == bor) {
#ifdef INFO
				std::cout << "Суффиксные ссылки не найдены, ссылка установлена на корень.\n";
#endif
				current->SufLink = bor; // если из корня нет пути, то ссылка устанавливается в корень
				IsFound = false; //ссылка, не равная корню,не найдена
				break;
			}
			CurrentLink = CurrentLink->SufLink;
		}
		if (IsFound) {
			CurrentLink = CurrentLink->next.at(key);
			if (current->parent == bor) {
#ifdef INFO
				std::cout << "Текущая вершина - начало слова, поэтому ссылка установлена на корень.\n";
#endif
				current->SufLink = bor; // для вершин первого уровня ссылки ведут в корень (иначе эти вершины будут ссылаться на себя)
			}
			else {
				current->SufLink = CurrentLink;
				Bor* CurEndLink = current->SufLink;
				while (CurEndLink != bor) { // поиск конечной ссылки. если дошли до корня, значит её нет
					if (CurEndLink->IsTerminal) {
						current->EndLink = CurEndLink;
						break;
					}
					CurEndLink = CurEndLink->SufLink;
				}
			}
		}
		if (!current->next.empty()) { // добавляются новые вершины в очередь
			for (auto& i : current->next) {
				front.push(i.second);
			}
		}
	}
}

std::pair<int, int> LongestChain(Bor* bor, Bor* root, int& depth) { //вычисление длин наибольших цепочек из суф. и конечных ссылок
	std::pair<int, int> longest = { 0, 0 };
	Bor* current = bor;
	while (current->SufLink != root) {
		longest.first++;
		current = current->SufLink;
	}
	longest.first++;
	current = bor;
	while (current->EndLink != nullptr) {
		longest.second++;
		current = current->EndLink;
	}
	for (auto& i : bor->next) {
		std::pair<int, int> next = LongestChain(i.second,root, ++depth);
		if (next.first > longest.first) {
			longest.first = next.first;
		}
		if (next.second > longest.second) {
			longest.second = next.second;
		}
	}
	depth--;
	return longest;
}

void AhoCorasick(const std::string& t, const std::vector<std::pair<std::string, int>>& patterns, std::vector<std::pair<int, int>>& result) {
	int length = 0;
	Bor* bor = CreateBor(patterns);
	Automaton(bor);
#ifdef INFO
	std::cout << "\n\nПоиск самых длинных цепочек из суффиксных и конечных ссылок\n";
#endif
	int depth = 0;
	std::pair<int, int> longest = LongestChain(bor, bor, depth);
#ifdef INFO
	std::cout << "Длина наибольшей цепочки из суффиксных ссылок: " << longest.first << '\n';
	std::cout << "Длина наибольшей цепочки из конечных ссылок: " << longest.second << '\n';
#endif
	Bor* current = bor;
	for (int i = 0; i < t.length(); i++) {
		current = current->GetLink(t.at(i)); // получена ссылка для перехода
		Bor* EndLink = current->EndLink;
		while (EndLink != nullptr) { // если у этой вершины есть конечная ссылка, значит нашелся шаблон
			result.push_back({ i - patterns.at(EndLink->TermNumPattern).first.length() + 2, EndLink->TermNumPattern + 1 });
			EndLink = EndLink->EndLink; // и так, пока цепочка из конечных ссылок не прервётся
		}
		if (current->IsTerminal) // если вершина терминальная - шаблон найден
			result.push_back({ i - patterns.at(current->TermNumPattern).first.length() + 2, current->TermNumPattern + 1 });
	}
	delete bor;
}

int main() {
	setlocale(LC_ALL, "Russian");
	std::string T;
	std::vector<std::pair<std::string, int>> patterns;
	std::vector<std::pair<int, int>> result;
	int n = 0;
#ifdef INFO
	std::cout << "Введите строку T и количетсво образцов P\n";
#endif
	std::cin >> T;
	std::cin >> n;
	for (int i = 0; i < n; i++) {
#ifdef INFO
		std::cout << "Введите образец:\n";
#endif
		std::string s;
		std::cin >> s;
		patterns.push_back({ s, 0 });
	}
	AhoCorasick(T, patterns, result);
	sort(result.begin(), result.end());
#ifdef INFO
	std::cout << "Ответ:\n";
#endif
	for (auto i : result) {
		std::cout << i.first;
		std::cout << ' ' << i.second << '\n';
	}
	return 0;
}