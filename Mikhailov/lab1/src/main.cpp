/**
 * Подсчёт количества вариантов покрытия прямоугольника
 * минимальным числом квадратов с использованием итеративного бэктрекинга.
 * Рёбра квадратов меньше рёбер поля.
 */

#include <iostream>
#include <vector>

#define DEBUG

using namespace std;

class SquareDivision
{
private:
    vector<vector<size_t>> squareDivArr;
    vector<vector<size_t>> optimalSquareDivArr;
    size_t size{};
    size_t currDivCount{};
    size_t optimalDivCount{};
    size_t compression{};

public:
    vector<vector<size_t>> finalData;
    size_t totalDivisions = 0;

    explicit SquareDivision(size_t size_) : size(size_), optimalDivCount(size * size)
    {
        // Инициализация массивов для хранения разбиений
        squareDivArr.resize(size);
        // Оптимизация квадрата (если это возможно, он масштабируется)
        for (auto i = size_; i > 0; --i) {
            if (size % i == 0 && size != i) {
                size /= i;
                compression = i;
                break;
            }
        }
        for (auto &i : squareDivArr) {
            i.resize(size_);
        }
        optimalSquareDivArr = squareDivArr;
    }

    void createOptimalDivision()
    {
        prepareDivSetup();
#ifdef DEBUG
        cout << "=== Завершение первоначальной расстановки ===" << endl
             << "++++++++ Поиск оптимального варианта ++++++++" << endl;
#endif
        updateOptimalDivision(size / 2, size / 2 + 1);
        saveOptimalDivString();
    }

private:

    /* Процедуры квадрирования */

    // Добавление нового квадрата
    void insertDivision(size_t x, size_t y, size_t size_)
    {
        for (auto i = x; i < x + size_; ++i) {
            for (auto j = y; j < y + size_; ++j) {
                squareDivArr[i][j] = size_;
            }
        }
        currDivCount++;
#ifdef DEBUG
        cout << "~~~~~~~~~~ Добавление квадрата № " << currDivCount << " ~~~~~~~~~~" << endl;
        if (compression > 1)
            cout << "Применен масштаб: " << compression << endl;
        for (auto & i : squareDivArr) {
            for (auto j : i) {
                cout << j << "  ";
            }
            cout << endl;
        }
#endif
    }

    // Проверка возможности разбить фрагмент
    bool checkDivision(size_t x, size_t y, size_t size_) {
        if (x + size_ > size || y + size_ > size)
            return false;
        for (auto i = x; i < x + size_; i++) {
            for (auto j = y; j < y + size_; j++) {
                if (squareDivArr[i][j] != 0)
                    return false;
            }
        }
        return true;
    }

    // Поиск свободного фрагмента поля
    bool findEmpty(size_t &x, size_t &y) {
        while (squareDivArr[x][y] != 0) {
            if (y == size - 1) {
                if (x == size - 1) {
                    return false;
                } else {
                    x++;
                    y = size / 2;
                    continue;
                }
            }
            y++;
        }
        return true;
    }

    // Удаление разбиения
    void removeDivision(size_t x, size_t y, size_t size_, bool optimal)
    {
#ifdef DEBUG
        if (!optimal)
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << "Удаляем квадрат (" << x << ", " << y << ", " << size_ << ")" << endl;
#endif
        for (auto i = x; i < x + size_; ++i)
            for (auto j = y; j < y + size_; ++j) {
                if (optimal)
                    optimalSquareDivArr[i][j] = 0;
                else
                    squareDivArr[i][j] = 0;
            }
        if (!optimal)
            currDivCount--;
    }

    // Начальное разбиение и обработка специальных случаев
    void prepareDivSetup()
    {
        size_t halfSize = size / 2;
        if (size % 2 == 0) {
            insertDivision(0, 0, halfSize);
            insertDivision(0, halfSize, halfSize);
            insertDivision(halfSize, 0, halfSize);
            insertDivision(halfSize, halfSize, halfSize);
            currDivCount = 4;
            optimalDivCount = currDivCount;
            optimalSquareDivArr = squareDivArr;
        } else {
            insertDivision(0, 0, halfSize + 1);
            insertDivision(0, halfSize + 1, halfSize);
            insertDivision(halfSize + 1, 0, halfSize);
        }
    }

    // Основной алгоритм квадрирования
    void updateOptimalDivision(size_t x, size_t y, int deep = 0)
    {
        if (currDivCount >= optimalDivCount)
            return;
        for (auto n = size / 2; n > 0; --n) {
            if (checkDivision(x, y, n)) {
                insertDivision(x, y, n);
                size_t copyX = x;
                size_t copyY = y;
                if (findEmpty(copyX, copyY)) {
                    updateOptimalDivision(copyX, copyY, deep++);
                } else {
                    if (currDivCount < optimalDivCount) {
                        optimalSquareDivArr = squareDivArr;
                        optimalDivCount = currDivCount;
                    }
                    removeDivision(x, y, n, false);
                    return;
                }
                removeDivision(x, y, n, false);
            }
        }
    }

    // Cохранение получившегося оптимального разбиения квадрата в выходной массив
    void saveOptimalDivString()
    {
        for (auto i = 0; i < size; ++i) {
            for (auto j = 0; j < size; ++j) {
                if (optimalSquareDivArr[i][j] != 0) {
                    finalData.push_back({i * compression + 1, j * compression + 1, optimalSquareDivArr[i][j] * compression});
                    removeDivision(i, j, optimalSquareDivArr[i][j], true);
                }
            }
        }
        totalDivisions = optimalDivCount;
    }
};

class RectangleDivision
{
    size_t width, height;

public:

    RectangleDivision(size_t width, size_t height)
    {
        this->width = width;
        this->height = height;
    }

    void createDivision() const
    {
        size_t sqX = 1, sqY = 1; // координаты текущего квадрата
        size_t nWidth = width, nHeight = height; // размеры неразбитой области
        size_t totalDivisions = 0;
        vector<vector<size_t>> finalData;

        // Квадрирование основной части прямоугольника - квадрата с максимально возможной стороной, который можно вписать в прямоугольник
        size_t size = nWidth < nHeight ? nWidth : nHeight;
        auto *cutter = new SquareDivision(size);
        cutter->createOptimalDivision();
        totalDivisions += cutter->totalDivisions;
        finalData.insert(finalData.end(), cutter->finalData.begin(), cutter->finalData.end());

        if (nWidth != nHeight) {
            // Если заданная фигура - прямоугольник, разбиваем оставшуюся часть на квадраты с максимально возможной стороной
            while (size > 0) {
                if (nWidth > nHeight) {
                    // разбиение по горизонтали
                    sqX += size;
                    nWidth -= size;
                } else {
                    // разбиение по вертикали
                    sqY += size;
                    nHeight -= size;
                }
                if (sqX > width || sqY > height)
                    break;
                size = nWidth > nHeight ? nHeight : nWidth;
                if (size == width || size == height) {
                    // если оставшаяся часть кратна квадрату с максимально возможной стороной, копируются данные первого квадрирования со смещением координат
                    for (auto &d: cutter->finalData) {
                        finalData.push_back({d[0] + sqX - 1, d[1] + sqY - 1, d[2]});
                    }
                    totalDivisions += cutter->totalDivisions;
                } else {
                    finalData.push_back({sqX, sqY, size});
                    totalDivisions++;
                }
            }
        }

        delete cutter;

#ifdef DEBUG
        cout << "~~~~~ Оптимальное покрытие прямоугольника ~~~~~" << endl;
        vector<vector<size_t>> rectArr;
        rectArr.resize(height);
        for (auto &a : rectArr) {
            a.resize(width);
        }
        for (auto &data: finalData) {
            for (auto i = data[1] - 1; i < data[1] + data[2] - 1; ++i) {
                for (auto j = data[0] - 1; j < data[0] + data[2] - 1; ++j) {
                    rectArr[i][j] = data[2];
                }
            }
        }
        for (auto & i : rectArr) {
            for (auto j : i) {
                cout << j << "  ";
            }
            cout << endl;
        }
#endif

        cout << "Количество вариантов оптимального покрытия квадратами: " << endl << totalDivisions << endl;
        cout << "Финальный результат:" << endl;
        for (auto &data: finalData) {
            cout << data[0] << " " << data[1] << " " << data[2] << endl;
        }
    };

};

int main()
{
    size_t width, height;
    cout << "Введите размеры прямоугольника" << endl;
    cout << "Ширина: ";
    cin >> width;
    cout << "Высота: ";
    cin >> height;
    if (height < 2 || width < 2) {
        cout << "Размер ребра прямоугольника не может быть меньше 2 по условиям задачи" << endl;
        return 0;
    }

    auto *cutter = new RectangleDivision(width, height);
    cutter->createDivision();
    delete cutter;

    return 0;
}
