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
	std::map<char, Bor*> next;
	std::vector<int> NumPattern; // номера шаблонов, в которые входит символ, по которому пришли
	int TermNumPattern;
	std::vector<int> shifts; // сдвиги подстрок в шаблоне
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
		if (this->SufLink == this) { // если в корне и путь ненайден
			return this;
		}
		return this->SufLink->GetLink(symbol); // не в корне ипуть не найден
	}
};

Bor* CreateBor(const std::vector<std::pair<std::string,int>>&patterns);
void Automaton(Bor* bor);
std::pair<int, int> LongestChain(Bor* bor, Bor* root, int&depth);
void AhoCorasick(const std::string& t, const std::vector<std::pair<std::string, int>>& patterns,std::vector<std::pair<int, int>>& result, int length);
void Partitioning(const std::string& p, const char& joker,std::vector<std::pair<std::string, int>>& patterns);

Bor* CreateBor(const std::vector<std::pair<std::string,int>>&patterns) { //создание бора
#ifdef INFO
	std::cout << "\nНачало построения бора:";
#endif
	Bor* bor = new Bor(); // корень бора
	for (auto& pt : patterns) {
		int NumPattern = find(patterns.begin(),
			patterns.end(), pt) - patterns.begin();
#ifdef INFO
		std::cout << "\nРассматривается " << NumPattern + 1 << " шаблон: " << pt.first << '\n';
#endif
		Bor* current = bor; // поиск пути, начиная от корня
		for (auto& c : pt.first) {
			if (current->next.find(c) == current->next.end())
			{ //если такого ребра нет, то добавляется
				current->next.insert({ c, new Bor(current, c) });
#ifdef INFO
				std::cout << "В бор добавлена вершина, куда ведёт символ(" << c << ")\n";
#endif
			}
			else
			{
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
		current->shifts.push_back(pt.second);
#ifdef INFO
		std::cout << "Данный шаблон имеет сдвиг " << pt.second << " относительно начала шаблона с маской.\n";
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
		front.pop(); Bor* CurrentLink = current->parent->SufLink; // родительская ссылка стала текущей
		const char& key = current->ToParent; // сохраняется символ, для которого ищется ссылка
		bool IsFound = true;
		while (CurrentLink->next.find(key) == CurrentLink->next.end()) {
			if (CurrentLink == bor) {
#ifdef INFO
				std::cout << "Суффиксные ссылки не найдены, ссылка установлена на корень.\n";
#endif
				current->SufLink = bor; // если из корня нет пути, то ссылка устанавливается в корень
				IsFound = false; //ссылка, не равная корню, не найдена
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
				current->SufLink = bor; // для вершин первого уровня ссылки ведут в корень (иначе эти вершины будут ссылаться на себя)
			}
			else {
				current->SufLink = CurrentLink;
				Bor* CurEndLink = current->SufLink;
				while (CurEndLink != bor) { // поиск конечной ссылки. если дошли до корня, значит её нет
					if (CurEndLink->IsTerminal) {
						current->EndLink = CurEndLink;
						break;
					}CurEndLink = CurEndLink->SufLink;
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
	while (current->SufLink != root) 
	{
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
		if (next.first > longest.first) 
		{
			longest.first = next.first;
		}
		if (next.second > longest.second) 
		{
			longest.second = next.second;
		}
	}
	depth--;
	return longest;
}

void AhoCorasick(const std::string& t, const
	std::vector<std::pair<std::string, int>>& patterns,
	std::vector<std::pair<int, int>>& result, int length) {
	Bor* bor = CreateBor(patterns);
	Automaton(bor);
#ifdef INFO
	std::cout << "\n\nПоиск самых длинных цепочек из суффиксных и конечных ссылок:\n";
#endif
	int depth = 0;
	std::pair<int, int> longest = LongestChain(bor, bor,depth);
#ifdef INFO
	std::cout << "Длина наибольшей цепочки из суффиксных ссылок: " << longest.first << '\n';
	std::cout << "Длина наибольшей цепочки из конечных ссылок: " << longest.second << '\n';
#endif
	Bor * current = bor;
	std::vector<int> index(t.length(), 0);
	for (int i = 0; i < t.length(); i++) {
		current = current->GetLink(t.at(i)); // получена ссылка для перехода
		Bor* EndLink = current->EndLink;
		while (EndLink != nullptr) { // если у этой вершины есть конечная ссылка, значит нашелся шаблон
				for (auto& s : EndLink->shifts) {
					int id = i - patterns.at(EndLink -> TermNumPattern).first.length() - s + 1;
					if (!(id < 0)) {
						index.at(id)++;
					}
				}
			EndLink = EndLink->EndLink; // и так, пока цепочка из конечных ссылок не прервётся
		}
		if (current->IsTerminal) // если вершина терминальная - шаблон найден
			for (auto& s : current->shifts) {
				int id = i - patterns.at(current -> TermNumPattern).first.length() - s + 1;
				if (!(id < 0)) {
					index.at(id)++;
				}
			}
	}

	for (int i = 0; i < index.size(); i++) {
		if (index[i] == patterns.size() && i + length <= t.length()) {
			result.push_back({ i + 1, 0 });
		}
	}
	delete bor;
}

void Partitioning(const std::string& p, const char& joker, std::vector<std::pair<std::string, int>>& patterns) { //разбиение шаблона с маской на безмасочные подшаблоны
	int prev = 0;
	int div;
	do {
		div = p.find(joker, prev);
		if ((div != prev) && (prev != p.length())) 
		{
			patterns.push_back({ p.substr(prev, div - prev),prev });
		}
		prev = div + 1;
	} while (div != std::string::npos); //пока не пусто
}

int main() {
	setlocale(LC_ALL, "Russian");
	std::string T, P;
	char joker;
	std::vector<std::pair<std::string, int>> patterns;
	std::vector<std::pair<int, int>> result;
#ifdef INFO
	std::cout << "Введите текст, шаблон и символ джокера:\n";
#endif
	std::cin >> T;
	std::cin >> P;
	std::cin >> joker;
	Partitioning(P, joker, patterns);
	AhoCorasick(T, patterns, result, P.length());
	sort(result.begin(), result.end());
#ifdef INFO
	std::cout << "Ответ:\n";
#endif
	for (auto i : result) {
		std::cout << i.first << '\n';
	}
	return 0;
}