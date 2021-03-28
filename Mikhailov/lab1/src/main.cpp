/**
 * Подсчёт количества вариантов покрытия прямоугольника
 * минимальным числом квадратов
 * с использованием рекурсивного бэктрекинга.
 * Рёбра квадратов меньше рёбер поля.
 */

#include <iostream>
#include <vector>

#define DEBUG

struct fragment
{
    size_t x;
    size_t y;
    bool isEmpty;
};

class SquareDivision
{
private:
    std::vector<std::vector<size_t>> squareDivArr;
    std::vector<std::vector<size_t>> optimalSquareDivArr;
    size_t size{};
    size_t currDivCount{};
    size_t optimalDivCount{};
    size_t compression{};

public:
    std::vector<std::vector<size_t>> finalData;
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
        std::cout << "=== Завершение первоначальной расстановки ===" << std::endl;
#endif
        updateOptimalDivision(size / 2, size / 2 + 1);
        saveOptimalDivString();
    }

private:

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
        std::cout << "~~~~~ Добавление квадрата № " << currDivCount << " (" << y + 1 << ", " << x + 1 << ", " << size_ << ") ~~~~~" << std::endl;
        if (compression > 1)
            std::cout << "Применен масштаб: " << compression << std::endl;
        for (auto & i : squareDivArr) {
            for (auto j : i) {
                std::cout << j << "  ";
            }
            std::cout << std::endl;
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
    fragment findEmpty(size_t x, size_t y) {
        while (squareDivArr[x][y] != 0) {
            if (y == size - 1) {
                if (x == size - 1) {
                    return {x, y, false};
                } else {
                    x++;
                    y = size / 2;
                    continue;
                }
            }
            y++;
        }
        return {x, y, true};
    }

    // Удаление разбиения
    void removeDivision(size_t x, size_t y, size_t size_, bool optimal)
    {
#ifdef DEBUG
        if (!optimal)
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl << "Удаляем квадрат (" << y + 1 << ", " << x + 1 << ", " << size_ << ")" << std::endl;
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
                fragment f = findEmpty(x, y);
                if (f.isEmpty) {
                    updateOptimalDivision(f.x, f.y, deep++);
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
        std::vector<std::vector<size_t>> finalData;

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
        std::cout << "~~~~~ Оптимальное покрытие прямоугольника ~~~~~" << std::endl;
        std::vector<std::vector<size_t>> rectArr;
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
                std::cout << j << "  ";
            }
            std::cout << std::endl;
        }
#endif

        std::cout << "Количество вариантов оптимального покрытия квадратами: " << std::endl << totalDivisions << std::endl;
        std::cout << "Финальный результат:" << std::endl;
        for (auto &data: finalData) {
            std::cout << data[0] << " " << data[1] << " " << data[2] << std::endl;
        }
    };

};

int main()
{
    size_t width, height;
    std::cout << "Введите размеры прямоугольника" << std::endl;
    std::cout << "Ширина: ";
    std::cin >> width;
    std::cout << "Высота: ";
    std::cin >> height;
    if (height < 2 || width < 2) {
        std::cout << "Размер ребра прямоугольника не может быть меньше 2 по условиям задачи" << std::endl;
        return 0;
    }

    auto *cutter = new RectangleDivision(width, height);
    cutter->createDivision();
    delete cutter;

    return 0;
}
