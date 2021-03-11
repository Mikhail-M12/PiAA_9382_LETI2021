#include <iostream>
#include <vector>


// Данная переменная отвечает за уровень рекурсии в данный момент времени
int RECURSION_LEVEL = 0;


class MessagePrinter {

    // Класс предназначен для вывода промежуточных данных
    // Чтобы избавиться от некрасивого кода в алгоритме решения задачи

    static void _printMsgWithRecursionLevel(const std::string& message) {
        std::string recursionLevelString = std::string();
        for (int i = 0; i < RECURSION_LEVEL; ++i) recursionLevelString += "_";
        std::cout << recursionLevelString << message;
    }

public:

    static void optimalLessCurrent(int x, int y, int countInOptimal, int countInCurrent) {
        std::string msg = "Кол-во квадратов оптимальной карты [" + std::to_string(countInOptimal) + "] ";
        msg += "<=, чем кол-во квадратов карты перебора [" + std::to_string(countInCurrent) + "]";
        msg += ", далее смысла перебирать нет.\n";
        msg = "[" + std::to_string(x) + "; " + std::to_string(y) + "] " + msg;
        _printMsgWithRecursionLevel(msg);
    }

    static void enterInRecursion(int x, int y) {
        std::string msg = "Вход в рекурсию\n";
        msg = "[" + std::to_string(x) + "; " + std::to_string(y) + "] " + msg;
        _printMsgWithRecursionLevel(msg);
    }

    static void tryToSetSquare(int x, int y, int size, bool canSetSq) {
        std::string msg = "Попытка поставить квадрат размера " + std::to_string(size);
        if (canSetSq) msg += " была удачной\n";
        else msg += " была неудачной\n";
        msg = "[" + std::to_string(x) + "; " + std::to_string(y) + "] " + msg;
        _printMsgWithRecursionLevel(msg);
    }

    static void isThereEmpty(int x, int y, bool isThereEmpty, int newX = 0, int newY = 0) {
        std::string msg = "В квадрате";
        if (isThereEmpty)
            msg += " есть пустая клетка по координатам (" + std::to_string(newX) + "; " + std::to_string(newY) + ")\n";
        else
            msg += " нет пустых клеток. Он полностью заполнен\n";

        msg = "[" + std::to_string(x) + "; " + std::to_string(y) + "] " + msg;
        _printMsgWithRecursionLevel(msg);
    }

    static void currentLessOptimal(int x, int y, bool isLess, int countInCurrent, int countInOptimal) {
        std::string msg = "Кол-во квадратов в карте перебора [" + std::to_string(countInCurrent) + "]";
        if (isLess) {
            msg += " <, чем в оптимальной карте [" + std::to_string(countInOptimal) + "].";
            msg += " Поэтому текущая карта перебора становится оптимальной.\n";
        }
        else msg += " >=, чем в оптимальной карте [" + std::to_string(countInOptimal) + "].\n";
        msg = "[" + std::to_string(x) + "; " + std::to_string(y) + "] " + msg;
        _printMsgWithRecursionLevel(msg);
    }

    static void removeSquare(int x, int y, int size) {
        std::string msg = "Был удален квадрат размера [" + std::to_string(size) + "] ";
        msg += "для дальнейшего перебора.\n";
        msg = "[" + std::to_string(x) + "; " + std::to_string(y) + "] " + msg;
        _printMsgWithRecursionLevel(msg);
    }
};


class SquareMap {
    int _size;
    int _compression;
    int _countSquares = 0;
    std::vector<std::vector<int>> _array;

public:
    SquareMap(int size, int compression) : _size(size), _compression(compression) {
        _array.resize(size);
        for (int i = 0; i < size; ++i) _array[i].resize(size, 0);
    }

    int countSquares() const {
        return _countSquares;
    }

    void insertSquare(int x, int y, int size) {

        // Метод вставки квадрата в карту
        // по координатам x и y размера size

        _countSquares++;
        for (int i = y; i < y + size; ++i) {
            for (int j = x; j < x + size; ++j)
                _array[i][j] = size;
        }
    }

    void removeSquare(int x, int y, int size) {

        // Метод удаления квадрата из карты
        // по координатам x и y размера size

        _countSquares -= 1;
        for (int i = y; i < y + size; ++i) {
            for (int j = x; j < x + size; ++j)
                _array[i][j] = 0;
        }
    }

    bool canSetSquare(int x, int y, int size) {

        // Метод проверки, можно ли вставить квадрат размера size в карту
        // по координатам x и y

        if (x + size > _size || y + size > _size)
            return false;

        for (int i = y; i < y + size; ++i) {
            for (int j = x; j < x + size; ++j)
                if (_array[i][j]) return false;
        }

        return true;
    }

    bool isThereEmpty(int& x, int& y) {

        // Метод проверки, есть ли пустое пространство в квадрате
        // Поскольку 75 % площади квадрата уже заняты изначально
        // то есть смысл искать пустое пространство только в 25% площади всего квадрата

        while (_array[y][x]) {
            if (x == _size - 1) {
                if (y == _size - 1) return false;
                x = _size / 2;
                y++;
                continue;
            }
            x++;
        }
        return true;
    }

    explicit operator std::string() {

        // Метод преобразования карты в тип std::string

        auto text = std::string();
        text.append(std::to_string(_countSquares) + "\n");
        for (int y = 0; y < _size; ++y) {
            for (int x = 0; x < _size; ++x) {
                if (_array[y][x]) {
                    // координаты и размер домножаются на коэф. сжатия, чтобы получить
                    // их верное значение
                    auto size = std::to_string(_array[y][x] * _compression);
                    auto transformedX = std::to_string(x * _compression + 1);
                    auto transformedY = std::to_string(y * _compression + 1);
                    auto row = std::string();
                    row.append(transformedX + " ");
                    row.append(transformedY + " ");
                    row.append(size + "\n");
                    text.append(row);
                    removeSquare(x, y, _array[y][x]);
                }
            }
        }
        return text;
    }

    void print(const std::string& innerMessage) {

        std::cout << "-------" << innerMessage <<  "-------" << std::endl;

        // вывод элементов матрицы

        for (int i = 0; i < _size; ++i) {
            for (int j = 0; j < _size; ++j)
                std::cout << _array[i][j] << " ";
            std::cout << std::endl;
        }

        auto closeMsg = std::string();
        // формирование закрывающего сообщения
        for (int i = 0; i < 14 + innerMessage.length(); ++i) closeMsg += "-";

        std::cout << closeMsg << std::endl;

    }
};


class SquareSolver {
    int _size;
    int _compression;
    SquareMap* _currentMap;
    SquareMap* _optimalMap;

    static std::pair<int, int> _doCompression(int size) {

        // Метод поиска наименьшего общего делителя в размере квадрата
        // Это одна из оптимизаций

        int compression = 1;
        int compressedSize = size;

        for (int delimiter = size / 2; delimiter > 1; --delimiter) {
            // перебор делителей от большего к меньшему
            if (!(size % delimiter)) {
                compression = delimiter;
                compressedSize = size / delimiter;
                // если нашли делитель, то он будет максимальным
                break;
            }
        }

        std::cout << "Было выполненино сжатие размера квадрата" << std::endl;
        std::cout << "Текущий размер квадрата - " << std::to_string(compressedSize) << std::endl;
        std::cout << "Коэффициент сжатия - " << std::to_string(compression) << std::endl;

        return {compressedSize, compression};
    }

    void _initOptimalMap() {

        // Метод инициализации первоначальной оптимальной карты
        // Ставим квадрат N-1 размера и окружаем его квадратами единичного размера

        std::cout << "Инициализация оптимальной карты" << std::endl;

        _optimalMap->insertSquare(0, 0, _size - 1);
        // окружаем квадрат N-1 единичными квадратами по правой части
        for (int y = 0; y < _size; ++y)
            _optimalMap->insertSquare(_size - 1, y, 1);
        // окружаем квадрат N-1 единичными квадратами по нижней части
        for (int x = 0; x < _size - 1; ++x)
            _optimalMap->insertSquare(x, _size - 1, 1);

        std::cout << "Начальная оптимальная карта имеет " << _optimalMap->countSquares() << " квадратов\n";

    }

    void _initCurrentMap() {

        // Метод инициализации карты перебора
        // Используется оптимизация, что 75% площади квадрата можно сразу покрыть 3-мя квадратами

        std::cout << "Инициализация карты для перебора" << std::endl;

        _currentMap->insertSquare(0, 0, _size / 2 + 1);
        _currentMap->insertSquare(_size / 2 + 1, 0, _size / 2);
        _currentMap->insertSquare(0, _size / 2 + 1, _size / 2);

        std::cout << "Карта для перебора заполнена на 75%" << std::endl;

    }

    void _solveEvenSquare() {

        // Квадрат с четной стороной имеет заранее определенное значение
        // Его сразу можно покрыть 4-мя квадратами

        std::cout << "Квадрат имеет четную сторону, поэтому его оптимальное решение - 4 квадрата" << std::endl;

        _optimalMap->insertSquare(0, 0, 1);
        _optimalMap->insertSquare(0, 1, 1);
        _optimalMap->insertSquare(1, 0, 1);
        _optimalMap->insertSquare(1, 1, 1);
    }

    void _solve(int x, int y) {

        // Рекурсивная функция перебора квадратов в карте перебора

        // Если кол-во квадратов в оптимальной карте уже меньше, чем в карте перебора
        // То дальше перебор квадратов не имеет смысла и можно откинуть эту ветку

        if (_optimalMap->countSquares() <= _currentMap->countSquares()) {
            MessagePrinter::optimalLessCurrent(x, y, _optimalMap->countSquares(), _currentMap->countSquares());
            return;
        }

        MessagePrinter::enterInRecursion(x, y);

        // Перебор размера квадрата, который будет поставлен по координатам X и Y от большего к меньшему

        for (int size = _size / 2; size > 0; --size) {
            if (_currentMap->canSetSquare(x, y, size)) {
                // если можем поставить квадрат, ставим его
                _currentMap->insertSquare(x, y, size);

                MessagePrinter::tryToSetSquare(x, y, size, true);

                int copyX = x;
                int copyY = y;

                bool isThereEmpty = _currentMap->isThereEmpty(copyX, copyY);
                // ищем свободную клетку
                if (!isThereEmpty) {
                    // если нет свободной клетки, квадрат заполнен
                    MessagePrinter::isThereEmpty(x, y, false);

                    if (_currentMap->countSquares() < _optimalMap->countSquares()) {
                        // смотрим, больше ли квадратов в оптимальной карте
                        // если больше, то ставим текущую карту как оптимальную
                        MessagePrinter::currentLessOptimal(x, y, true, _currentMap->countSquares(), _optimalMap->countSquares());
                        *_optimalMap = *_currentMap;
                    } else MessagePrinter::currentLessOptimal(x, y, false, _currentMap->countSquares(), _optimalMap->countSquares());
                } else {

                    MessagePrinter::isThereEmpty(x, y, true, copyX, copyY);

                    RECURSION_LEVEL ++ ;
                    _solve(copyX, copyY);
                    RECURSION_LEVEL -- ;
                }

                // удаляем квадрат, который поставили, чтобы перебирать дальше
                _currentMap->removeSquare(x, y, size);
                MessagePrinter::removeSquare(x, y, size);
            } else MessagePrinter::tryToSetSquare(x, y, size, false);
        }
    }

public:
    explicit SquareSolver(int size) {
        auto resultOfCompression = _doCompression(size);
        _size = resultOfCompression.first;
        _compression = resultOfCompression.second;
        _currentMap = new SquareMap(_size, _compression);
        _optimalMap = new SquareMap(_size, _compression);
    }

    SquareMap* solve() {
        if (!(_size % 2)) _solveEvenSquare(); // если квадрат с четной стороной, то для него подготовлено
                                              // отдельное решение
        else {

            // инициализируем карты перебора и оптимальную
            _initOptimalMap();
            _initCurrentMap();

            std::cout << "Визуализация карты перебора и оптимальной карты после инициализации:" << std::endl;
            _optimalMap->print("Optimal Map");
            _currentMap->print("Current Map");

            _solve(_size / 2 + 1, _size / 2);

            std::cout << "Визуализация оптимальной карты после решения" << std::endl;
            _optimalMap->print("Optimal Map");

        }
        return _optimalMap;
    }
};


int main() {
    int size;
    std::cin >> size;
    std::cout << (std::string) *SquareSolver(size).solve() << std::endl;
    return 0;
}
