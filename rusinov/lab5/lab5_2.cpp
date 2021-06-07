#include "vector"
#include "iostream"
#include "map"
#include "cstring"
#include "algorithm"

#define DEBUG

const int ALPHABET_LENGTH = 6;
std::map<char, int> ALPHABET = {{'A', 0},
                                {'C', 1},
                                {'G', 2},
                                {'T', 3},
                                {'N', 4}};

struct BohrVertex {
    int nextVertices[ALPHABET_LENGTH];
    bool isTerminal;
    int suffixLink;

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

        return vertex;
    }
};


class Bohr {
    std::vector<BohrVertex> vertexes;

    char joker;
    std::string text;
    std::string pattern;

    int indexOfLastFoundedPattern;
    bool isThereCrossing;

    void addStringToBohr(const std::string & string);
    int getSuffixLink(int vertex);
    int getTransition(int vertex, char symbol);
    std::string getPatternFromText(int i, int sizeOfPattern);


public:
    std::vector<char> symbolsUnderJoker;
    int numOfSymbolUnderJoker{};
    Bohr(char joker, std::string & text, std::string & pattern);
    void doAlgorithm();
};

Bohr::Bohr(char joker, std::string &text, std::string &pattern)
        : joker(joker), text(text), pattern(pattern) {
    vertexes = std::vector<BohrVertex>();
    auto root = BohrVertex::createVertex(0, '\0');
    vertexes.push_back(root);

    indexOfLastFoundedPattern = - pattern.size();
    isThereCrossing = false;
}


void Bohr::addStringToBohr(const std::string &string) {
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
            std::cout << "Для данного символа был добавлен узел в бор, его но-мер: " << vertexes[number].nextVertices[ordinal] << "\n\n";
#endif

        }
#ifdef DEBUG
        else { std::cout << "Узел для данного символа уже есть в боре, его но-мер: " << vertexes[number].nextVertices[ordinal] << "\n\n"; }
#endif
        number = vertexes[number].nextVertices[ordinal];
    }

#ifdef DEBUG
    std::cout << "-------------------------------------------" << std::endl;
#endif

    vertexes[number].isTerminal = true;
}


int Bohr::getSuffixLink(int vertex) {

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
        if (vertexes[vertex].symbol == joker) {
            char symbol = symbolsUnderJoker[numOfSymbolUnderJoker--];
#ifdef DEBUG
            std::cout << "Для вычисления суффиксной ссылки узла нужно совер-шить переход"
                         " по суффиксной ссылке родителя " << parent << " по символу ПОД ДЖОКЕРОМ в тексте '" << symbol << "'\n";
#endif
            vertexes[vertex].suffixLink = getTransition(getSuffixLink(parent), symbol);
        }
        else {
            char symbol = vertexes[vertex].symbol;
#ifdef DEBUG
            std::cout << "Для вычисления суффиксной ссылки узла нужно совер-шить переход"
                         " по суффиксной ссылке родителя " << parent << " по СТАТИЧНОМУ символу '" << symbol << "'\n";
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


#ifdef DEBUG
        std::cout << "Для данного символа из данного узла не вычислен переход\n";
#endif

    if (vertexes[vertex].nextVertices[ordinal] != -1) {
        int nextVertex = vertexes[vertex].nextVertices[ordinal];
#ifdef DEBUG
        std::cout << "Из данного узла по данному символу возможно совершить переход в узел " << nextVertex << "\n";
#endif
        vertexes[vertex].transitions[ordinal] = nextVertex;
    }

    else if (vertexes[vertex].nextVertices[5] != -1) {
        int nextVertex = vertexes[vertex].nextVertices[5];
#ifdef DEBUG
        std::cout << "Из данного узла возможно совершить переход по джокеру в узел" << nextVertex << "\n";
#endif
        vertexes[vertex].transitions[ordinal] = nextVertex;
    }

    else {
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

#ifdef DEBUG
    std::cout << "Переход по символу '" << symbol << "' из узла " << vertex << " будет выполнен в узел " << vertexes[vertex].transitions[ordinal] << "\n";
#endif

    return vertexes[vertex].transitions[ordinal];
}


void Bohr::doAlgorithm() {
    addStringToBohr(pattern);

    int vertex = 0;
    auto results = std::map<int, std::string>();

    for (int i = 0; i < text.size(); ++i) {
        char symbol = text[i];

#ifdef DEBUG
        std::cout << "-----------ПЕРЕХОД------------\n";
        std::cout << "Текущее состояние автомата: " << vertex << "\n";
        std::cout << "Рассматривается символ текста '" << symbol << "' " << "по индексу " << i << "\n";
#endif

        vertex = getTransition(vertex, symbol);

        if (vertexes[vertex].symbol == joker) {
#ifdef DEBUG
            std::cout << "Был совершен переход по Джокеру, поэтому рассматри-ваемый символ"
                         " был добавлен в список символов, которые находились 'под джокером'"
                         ", чтобы была возможность вычислить суффиксную ссыл-ку\n";
#endif
            symbolsUnderJoker.push_back(symbol);
        }
        numOfSymbolUnderJoker = (int) symbolsUnderJoker.size() - 1;

#ifdef DEBUG
        std::cout << "--------КОНЕЦ ПЕРЕХОДА--------\n";
#endif

        if (vertexes[vertex].isTerminal) {
            int indexOfPattern = i - (int) pattern.size() + 2;

#ifdef DEBUG
            std::cout << "Текущий узел является терминальным, найден шаблон по индексу " << indexOfPattern << "\n";
#endif

            if (!isThereCrossing && indexOfPattern < indexOfLastFoundedPattern + pattern.size()) {

#ifdef DEBUG
                std::cout << "Текущий шаблон пересекается с прошлым найденным шаблоном в строке поиска\n";
#endif

                isThereCrossing = true;
            }
            indexOfLastFoundedPattern = indexOfPattern;

            results[indexOfPattern] = getPatternFromText(indexOfPattern - 1, pattern.size());
        }
    }

#ifdef DEBUG
    std::cout << "Индексы найденных образцов:\n";
#endif

    for (const auto& result : results)
        std::cout << result.first << std::endl;

#ifdef DEBUG
    if (isThereCrossing)
        std::cout << "Имеются экземпляры образца " << pattern << ", которые пересекаются между собой" << std::endl;
    else
        std::cout << "Нет экземпляров образца, которые пересекаются между со-бой" << std::endl;

    std::cout << "\nКоличество узлов в автомате " << vertexes.size() << "\n";
#endif

}

std::string Bohr::getPatternFromText(int i, int sizeOfPattern) {
    auto foundPattern = std::string();
    int endOfPattern = i + sizeOfPattern;
    for (; i < endOfPattern; ++i) foundPattern += text[i];
    return foundPattern;
}


int main() {
    char joker;
    std::string text, pattern;
    std::cin >> text >> pattern >> joker;
    auto bohr = Bohr(joker, text, pattern);
    ALPHABET[joker] = 5;
    bohr.doAlgorithm();
    return 0;
}
