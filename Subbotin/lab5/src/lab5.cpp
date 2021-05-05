#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

#define TASK1
//#define TASK2
#define DEBUG

class Korasik {
public:
    std::vector<std::unordered_map<char, int>> trie;
    std::vector<std::vector<int>> term;
    std::vector<int> fail;
    int curPos;

    Korasik(std::vector<std::string> &words);
    std::vector<int> check(char letter);
};

Korasik::Korasik(std::vector<std::string> &words) {
#ifdef DEBUG
    std::cout << "Строим префиксное дерево для набора паттернов: " << std::endl;
    for (std::string word : words) {
        std::cout << word << " ";
    }
    std::cout << std::endl;
#endif
    int total = 0;
    std::unordered_map<char, int> root;
    this->trie.push_back(root);
    this->fail.push_back(0);
    this->term.emplace_back(std::vector<int>());

    int wordCounter = 0;
    for (std::string word : words) {
#ifdef DEBUG
        std::cout << "Рассматриваем паттерн: " << word << std::endl;
#endif
        int cur = 0;
        for (char ch : word) {
#ifdef DEBUG
            std::cout << "Текущий индекс в дереве: " << cur << std::endl;
            std::cout << "Рассматриваем символ: " << ch << std::endl;
#endif
            // если ch - уже ребенок для cur, то просто переходим в сh
            if (this->trie[cur].find(ch) != this->trie[cur].end()) {
                cur = this->trie[cur][ch];
#ifdef DEBUG
                std::cout << ch << " - ребенок для текущего элемента, переходим в " << ch << std::endl;
#endif
            }
            // если сh не ребенок для cur
            else {
                //создаем новую вершину
                std::unordered_map<char, int> newNode;
                this->trie.push_back(newNode);
                this->term.emplace_back(std::vector<int>());
                //инкрементируем кол-во вершин в trie
                total++;
                //помечаем вершину соответствующему ей номеру
                this->trie[cur][ch] = total;
                cur = total;
#ifdef DEBUG
                std::cout << ch << " не ребенок для текущего элемента, добавили новую вершину с индексом " << cur << std::endl;
#endif
            }
        }
#ifdef DEBUG
        std::cout << "Паттерн закончился, помечаем последнюю символ-вершину паттерна с индексом " << cur << std::endl;
#endif
        //помечаем вершину, которая является концом слова
        this->term[cur].push_back(wordCounter++);
    }

#ifdef DEBUG
    std::cout << "Заполняем связи неудач. " << std::endl;
    std::cout << "Проходимся по дереву в ширину: " << std::endl;
#endif

    // заполняем failing pointers нулями (указывают пока на корневую вершину)
    for (int i = 0; i < this->trie.size(); i++) {
        this->fail.push_back(0);
    }

    std::queue<std::pair<int, std::unordered_map<char, int>>> nodes;
    //рассматриваем всех детей корня
    for (auto child : this->trie[0]) {
        // first - номер вершинки, second - unordered_map соотв. этой вершине, в которой хранятся ее дети
        auto o = std::make_pair(child.second, this->trie[child.second]);
        nodes.push(o);
    }

    while (!nodes.empty()) {
#ifdef DEBUG
        std::cout << "Состояние очереди поиска в ширину: " << std::endl;
        auto printQueue = nodes;
        while (!printQueue.empty()) {
            std::cout << printQueue.front().first << " ";
            printQueue.pop();
        }
        std::cout << std::endl;
#endif
        //рассматриваем следующую вершину из очереди
        auto vertex = nodes.front();
        nodes.pop();
        auto i = vertex.first;
        auto node = vertex.second;
#ifdef DEBUG
        std::cout << "Рассматриваем вершину " << i << " и ее детей: " << std::endl;
#endif
        //каждого ребенка текущей вершины записываем в очередь для BFS.
        for (auto child : node) {
            auto bfsNode = std::make_pair(child.second, this->trie[child.second]);
            nodes.push(bfsNode);
        }
        //рассматриваем каждого ребенка для текущей вершины
        for (auto child : node) {

            char childName = child.first;
            int childPos = child.second;

#ifdef DEBUG
            std::cout << "Ребенок " << childName << " с индексом " << childPos << std::endl;
#endif

            // failing pointer - индекс вершины, в которую нужно перейти в случае, если дальше по trie идти некуда
            int fPointer = this->fail[i];
#ifdef DEBUG
            std::cout << "Связь неудачи для индекса " << i << " - это " << fPointer << std::endl;
#endif

            // пока failing pointer не указывает на корень и пока среди детей failing pointer
            // нет вершины схожей с текущим ребенком
            // просто передвигаемся по поинтерам вверх
            while (fPointer != 0 && this->trie[fPointer].find(childName) == trie[fPointer].end()) {
#ifdef DEBUG
                std::cout << "Среди детей текущего failing pointer'a " << fPointer << " нет " << childName << " идем выше" << std::endl;
#endif
                fPointer = this->fail[fPointer];
            }

            //если loop выше закончился из-за встречи схожего ребенка
            // то failing pointer для текущего ребенка будет индексом
            //схожего ребенка для 'сдвинутого' failing pointer'а
            if (this->trie[fPointer].find(childName) != trie[fPointer].end()) {
                fPointer = this->trie[fPointer][childName];
            }
            this->fail[childPos] = fPointer;
#ifdef DEBUG
            std::cout << "Теперь fpointer для " << childName << " - это вершина с индексом " << fPointer << std::endl;
#endif

            //если же еще этот failing pointer указывает на терминированную вершину
            //то пометим текущего ребенка тоже терминированным
            if (!this->term[this->fail[childPos]].empty()) {
#ifdef DEBUG
                std::cout << "Ребенок наследует от failing pointer терминированные метки: ";
#endif
                for (auto item : this->term[this->fail[childPos]]) {
                    std::cout << item << " ";
                    this->term[childPos].push_back(item);
                }
                std::cout << std::endl;
            }
        }
    }
    this->curPos = 0;

#ifdef DEBUG
    std::cout << "\n Построенный автомат: \n";
    for (int i = 0; i < trie.size(); i++) {
        std::cout << "Вершина с индексом " << i << std::endl;
        if (term[i].empty()) {
            std::cout << "Не терминированная" << std::endl;
        } else {
            std::cout << "Вершина терминированна, номера паттернов, которые заканчиваются на этой вершине: " << std::endl;
            for (auto item : term[i]) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Суффиксная ссылка указывает на вершину с индексом: " << fail[i] << std::endl;
        if (trie[i].empty()) {
            std::cout << "Детей нет";
        } else {
            std::cout << "Дети: ";
            for (auto item : trie[i]) {
                std::cout << "{" << item.first << ", " << item.second << "} ";
            }
        }
        std::cout << std::endl
                  << "-------------------------------" << std::endl;
    }
#endif
}

std::vector<int> Korasik::check(char letter) {
#ifdef DEBUG
    std::cout << "Ищем вхождение символа " << letter << std::endl;
#endif
    while (this->curPos > 0 && this->trie[this->curPos].find(letter) == this->trie[this->curPos].end()) {
#ifdef DEBUG
        std::cout << "Для индекса " << curPos << " среди детей не нашлось " << letter << std::endl;
#endif
        this->curPos = this->fail[this->curPos];
#ifdef DEBUG
        std::cout << "Переходим дальше по fpointer'у в " << curPos << std::endl;
#endif
    }
    if (this->trie[this->curPos].find(letter) != this->trie[this->curPos].end()) {

        this->curPos = this->trie[this->curPos][letter];
#ifdef DEBUG
        std::cout << "Один из детей совпал с " << letter << ", текущий индекс " << curPos << std::endl;
#endif
    }
#ifdef DEBUG
    std::cout << "Возвращаем паттерны, которые заканчиваются на индексе " << curPos << " : " << std::endl;
    for (auto item : this->term[this->curPos]) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
#endif
    return this->term[this->curPos];
}

int main() {
    std::string seq;
    std::cin >> seq;

#ifdef TASK1
    int n = 0;
    std::cin >> n;

    std::vector<std::string> words;
    for (int i = 0; i < n; i++) {
        std::string word;
        std::cin >> word;
        words.push_back(word);
    }
    auto obj = new Korasik(words);
    std::vector<std::pair<int, int>> res;
    for (int pos = 0; pos < seq.length(); pos++) {
        std::vector<int> pats = obj->check(seq[pos]);
        if (!pats.empty()) {
            for (auto point : pats) {
                int patternLength = words[point].length();
                res.emplace_back(std::make_pair(pos - patternLength + 2, point + 1));
            }
        }
    }

    std::sort(res.begin(), res.end());
    for (auto item : res) {
        std::cout << item.first << " " << item.second << std::endl;
    }
#endif


#ifdef TASK2
    std::string word;
    std::cin >> word;
    char joker;
    std::cin >> joker;

    std::vector<int> C(seq.length(), 0);
    std::vector<std::string> patterns;
    std::vector<int> index;
    int j = 0;
    while (j < word.length()) {
        std::string small;
        int i = j;
        while (j < word.length() && word[j] != joker) {
            small += word[j];
            j++;
        }
        if (!small.empty()) {
            patterns.push_back(small);
            index.push_back(i);
        }
        j++;
    }
    auto obj = new Korasik(patterns);
    for (int pos = 0; pos < seq.length(); pos++) {
        std::vector<int> pats = obj->check(seq[pos]);
        if (!pats.empty()) {
            for (auto point : pats) {
                int firstLetter = pos - patterns[point].length() + 1;
                int idx = firstLetter - index[point] + 1;
                if (idx >= 0 && idx < C.size()) {
                    C[firstLetter - index[point] + 1]++;
                }
            }
        }
    }

    for (int i = 0; i < C.size() - word.size() + 2; i++) {
        if (C[i] == patterns.size()) {
            std::cout << i << std::endl;
        }
    }
#endif
    return 0;
}