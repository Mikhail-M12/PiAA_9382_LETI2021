#include "vector"
#include "iostream"
#include "map"
#include "cstring"
#include "algorithm"
#include "set"


#define DEBUG

const int ALPHABET_LENGTH = 5;
std::map<char, int> ALPHABET = {{'A', 0},
                                {'C', 1},
                                {'G', 2},
                                {'T', 3},
                                {'N', 4}};

struct BohrVertex {
    int nextVertices[ALPHABET_LENGTH];
    int patternNumber;
    bool isTerminal;
    int suffixLink;
    int goodSuffixLink;
    int parent;
    char symbol;
    int transitions[ALPHABET_LENGTH];

    static BohrVertex createVertex(int parentNumber, char symbolOfEdge) {
        auto vertex = BohrVertex();
        memset(vertex.nextVertices, 255, sizeof(vertex.nextVertices));
        memset(vertex.transitions, 255, sizeof(vertex.transitions));
        vertex.isTerminal = false;
        vertex.suffixLink = -1;
        vertex.parent = parentNumber;
        vertex.symbol = symbolOfEdge;
        vertex.goodSuffixLink = -1;
        return vertex;
    }
};


class Bohr {

    std::vector<BohrVertex> vertexes {};
    std::vector<std::string> patterns {};
    std::string & text;

    int indexOfLastPattern;
    int numOfLastPattern;
    std::set<int> crosses {};

public:

    Bohr(std::string & text) : text(text) {
        auto root = BohrVertex::createVertex(0, '\0');
        vertexes.push_back(root);

        indexOfLastPattern = 0;
        numOfLastPattern = -1;
    }

    void addStringToBohr(const std::string & string, int indexOfPattern) {
        int number = 0;

#ifdef DEBUG
        std::cout << "-------------------------------------------\n" << std::endl;
        std::cout << "Добавление шаблона " << string << " в бор\n";
#endif

        for (char symbol : string) {

#ifdef DEBUG
            std::cout << "Рассматривается символ " << "'" << symbol << "'\n";
#endif

            int ordinal = ALPHABET[symbol];
            if (vertexes[number].nextVertices[ordinal] == -1) {

                auto vertex = BohrVertex::createVertex(number, symbol);
                vertexes.push_back(vertex);
                vertexes[number].nextVertices[ordinal] = (int) vertexes.size() - 1;

#ifdef DEBUG
                std::cout << "Для данного символа был добавлен узел в бор, его номер: " << vertexes[number].nextVertices[ordinal] << "\n\n";
#endif

            }
#ifdef DEBUG
            else { std::cout << "Узел для данного символа уже есть в боре, его номер: " << vertexes[number].nextVertices[ordinal] << "\n\n"; }
#endif
            number = vertexes[number].nextVertices[ordinal];
        }

#ifdef DEBUG
        std::cout << "-------------------------------------------" << std::endl;
#endif

        vertexes[number].isTerminal = true;
        vertexes[number].patternNumber = indexOfPattern;
        patterns.push_back(string);
    }

    int getSuffixLink(int vertex);
    int getTransition(int vertex, char symbol);
    int getGoodSuffixLink(int vertex);
    void doAlgorithm();
};


int Bohr::getSuffixLink(int vertex) {

    if (vertexes[vertex].suffixLink == -1) {

#ifdef DEBUG
        std::cout << "\nВычисляется суффиксная ссылка для узла: " << vertex << "\n";
#endif

        if (vertex == 0 || vertexes[vertex].parent == 0) {

#ifdef DEBUG
            std::cout << "Узел корневой или первый после корня, поэтому суффиксная ссылка - 0\n";
#endif

            vertexes[vertex].suffixLink = 0;
        }

        else {
            int parent = vertexes[vertex].parent;
            char symbol = vertexes[vertex].symbol;
#ifdef DEBUG
            std::cout << "Для вычисления суффиксной ссылки узла нужно совершить переход"
                         " по суффиксной ссылке родителя " << parent << " по символу '" << symbol << "'\n";
#endif
            vertexes[vertex].suffixLink = getTransition(getSuffixLink(parent), symbol);
        }

    }

#ifdef DEBUG
    std::cout << "■ Суффиксная ссылка узла " << vertex << " - " << vertexes[vertex].suffixLink << "\n";
#endif

    return vertexes[vertex].suffixLink;
}


int Bohr::getTransition(int vertex, char symbol) {

    char ordinal = ALPHABET[symbol];

#ifdef DEBUG
    std::cout << "\nВыполняется переход по символу '" << symbol << "'" << " из узла " << vertex << "\n";
#endif

    if (vertexes[vertex].transitions[ordinal] == -1) {

#ifdef DEBUG
        std::cout << "Для данного символа из данного узла не вычислен переход\n";
#endif

        if (vertexes[vertex].nextVertices[ordinal] != -1) {

            int nextVertex = vertexes[vertex].nextVertices[ordinal];
#ifdef DEBUG
            std::cout << "Из данного узла по данному символу возможно совершить переход в узел " << nextVertex << "\n";
#endif
            vertexes[vertex].transitions[ordinal] = nextVertex;

        } else {

            if (vertex == 0) {
#ifdef DEBUG
                std::cout << "Данный узел является корнем, и невозможно совершить переход по заданному символу, поэтому переход будет в узел 0\n";
#endif
                vertexes[vertex].transitions[ordinal] = 0;
            }

            else {
#ifdef DEBUG
                std::cout << "Невозможно совершить переход по заданному символу из заданного узла, поэтому необходимо вычислить переход по суффиксной ссылке\n";
#endif
                vertexes[vertex].transitions[ordinal] = getTransition(getSuffixLink(vertex), symbol);
            }
        }
    }

#ifdef DEBUG
    std::cout << "Переход по символу '" << symbol << "' из узла " << vertex << " будет выполнен в узел " << vertexes[vertex].transitions[ordinal] << "\n";
#endif

    return vertexes[vertex].transitions[ordinal];
}


int Bohr::getGoodSuffixLink(int vertex) {

#ifdef DEBUG
    std::cout << "\nВычисляется хорошая суффиксная ссылка для узла: " << vertex << "\n";
#endif

    if (vertexes[vertex].goodSuffixLink == -1) {
        int suffixLink = getSuffixLink(vertex);

        if (suffixLink == 0) {
#ifdef DEBUG
            std::cout << "Суффиксная ссылка указывает на 0, поэтому хорошая суффиксная ссылка тоже 0\n";
#endif
            vertexes[vertex].goodSuffixLink = 0;
        }
        else {
            if (vertexes[suffixLink].isTerminal) {
#ifdef DEBUG
                std::cout << "Узел по суффиксной ссылке является терминальным, поэтому суффиксная ссылка хорошая\n";
#endif
                vertexes[vertex].goodSuffixLink = suffixLink;
            }
            else {
#ifdef DEBUG
                std::cout << "Узел по суффиксной ссылке не терминальный, поэтому вычисляется хорошая суффиксная ссылка узла" << suffixLink << "\n";
#endif
                vertexes[vertex].goodSuffixLink = getGoodSuffixLink(suffixLink);
            }
        }
    }

#ifdef DEBUG
    std::cout << "■ Хорошая суффиксная ссылка узла " << vertex << " - " << vertexes[vertex].goodSuffixLink << "\n";
#endif
    return vertexes[vertex].goodSuffixLink;
}


bool resultComparator(std::pair<int, int> first, std::pair<int, int> second) {
    if (first.first < second.first) return true;
    if (first.first == second.first)
        return first.second < second.second;
    return false;
}


void Bohr::doAlgorithm() {

    int vertex = 0;
    auto result = std::vector<std::pair<int, int>>();

    for (int i = 0; i < text.size(); ++i) {

        char symbol = text[i];

#ifdef DEBUG
        std::cout << "-----------ПЕРЕХОД------------\n";
        std::cout << "Текущее состояние автомата: " << vertex << "\n";
        std::cout << "Рассматривается символ текста '" << symbol << "' " << "по индексу " << i << "\n";
#endif

        vertex = getTransition(vertex, symbol);

#ifdef DEBUG
        std::cout << "--------КОНЕЦ ПЕРЕХОДА--------\n";
#endif

        if (vertexes[vertex].isTerminal) {

            int indexOfCurrentPattern = i - (int) patterns[vertexes[vertex].patternNumber].size() + 1;
            if (numOfLastPattern != - 1 && indexOfCurrentPattern < indexOfLastPattern + patterns[numOfLastPattern].size()) {

#ifdef DEBUG
                std::cout << "Индекс прошлого найденного образца: " << indexOfLastPattern << "\n";
                std::cout << "Размер прошлого найденного образца: " << patterns[vertexes[vertex].patternNumber].size() << "\n";
                std::cout << "Индекс текущего найденного образца: " << indexOfCurrentPattern << "\n";
                std::cout << "Прошлый и текущий образцы пересекаются, так как индекс"
                             " текущего образца попал в область прошлого образца" << "\n";
#endif

                crosses.insert(numOfLastPattern);
                crosses.insert(vertexes[vertex].patternNumber);
            }

            numOfLastPattern = vertexes[vertex].patternNumber;
            indexOfLastPattern = i - (int) patterns[numOfLastPattern].size() + 1;

#ifdef DEBUG
            std::cout << "--------ПОИСК ХОРОШИХ ССЫЛОК--------\n";
            std::cout << "Так как новый узел терминальный, то необходимо вычислить хорошие суффиксные ссылки,"
                         " поскольку в образец могут входить другие образцы\n";
#endif

            auto innerPatterns = std::vector<int>();

            // Все внутренние шаблоны (хорошие суффиксные ссылки) точно пересекаются
            // Если в innerPatterns будет > 1 элемента, значит нужно добавить все эти элементы в множество пересекающихся

            for (int link = vertex; link; link = getGoodSuffixLink(link)) {
                int patternNumber = vertexes[link].patternNumber;

                innerPatterns.push_back(patternNumber);

                int indexOfPattern = i - (int) patterns[patternNumber].size() + 2;
#ifdef DEBUG
                std::cout << "■ По индексу " << indexOfPattern << " располагается шаблон " << patterns[patternNumber] << "\n";
#endif
                result.emplace_back(indexOfPattern, patternNumber + 1);
            }

            if (innerPatterns.size() > 1)
                for (auto p : innerPatterns) crosses.insert(p);


#ifdef DEBUG
            std::cout << "-----КОНЕЦ ПОИСКА ХОРОШИХ ССЫЛОК-----\n";
#endif
        }
    }

    std::sort(result.begin(), result.end(), resultComparator);

    for (auto pair : result) {
        std::cout << pair.first << " " << pair.second << std::endl;
    }

    std::cout << "Пересекающиеся шаблоны: ";
    for (auto cross : crosses) {
        std::cout << patterns[cross] << ", ";
    }

    if (crosses.empty()) std::cout << " нет\n";

    std::cout << "\nКоличество узлов в автомате " << vertexes.size() << "\n";

}


int main() {
    int countPatterns;
    std::string text, pattern;
    std::cin >> text >> countPatterns;

    auto bohr = Bohr(text);

    auto patterns = std::vector<std::string>();
    for (int i = 0; i < countPatterns; ++i) {
        std::cin >> pattern;
        patterns.push_back(pattern);
        bohr.addStringToBohr(pattern, i);
    }

    bohr.doAlgorithm();
    return 0;
}