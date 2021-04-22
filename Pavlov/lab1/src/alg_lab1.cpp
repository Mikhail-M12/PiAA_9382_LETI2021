#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <climits>
#include <ctime>

#define OUTPUT

struct Square {
    int x, y;
    int side;
};

class SquareTable {
private:
    int side;
public:
    std::vector<Square> track;
    std::vector<Square> bestTrack;
    std::array<std::array<int, 40>, 40> surface = { 0 };
    std::array<std::array<int, 40>, 40> bestSurface;

    explicit SquareTable(const int n) {
        side = n;
    }

    void printTrack(const int& scale) {
        for (auto& i : bestTrack) {
            std::cout << i.x * scale + 1 << " " << i.y * scale + 1 << " " << i.side * scale << "\n";
        }
    }

    void printSurface(const int& scale) {
        for (int i = 0; i < side * scale; ++i) {
            for (int j = 0; j < side * scale; ++j) {
                std::cout.width(2);
                std::cout << bestSurface[i/scale][j/scale] << " ";
            }
            std::cout << "\n\n";
        }
    }

    Square priorSquare() const {
        Square pos{ -1, -1, 0 };
        for (int i = 0; i < side; ++i) {
            bool found = false;
            for (int j = 0; j < side; ++j) {
                if (surface.at(i).at(j) == 0) {
                    pos.x = i;
                    pos.y = j;
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }

        do {
            ++pos.side;
            if ((pos.x == -1) || (pos.x + pos.side - 1 >= side) || (pos.y + pos.side - 1 >= side)) {
                pos.side = -pos.side;
            }
            for (int l = pos.y; l < pos.y + pos.side; ++l) {
                if (surface[pos.x][l] != 0) {
                    pos.side = -pos.side;
                }
            }
        } while (pos.side > 0);
        pos.side = - (++pos.side);
        return pos;
    }

    void writeToTrack(const Square& pos) {
        track.push_back(pos);
        for (int i = 0; i < pos.side; ++i) {
            for (int j = 0; j < pos.side; ++j) {
                surface.at(pos.x + i).at(pos.y + j) = track.size();
            }
        }
    }

    Square getFromTrack() {
        if (track.size() < 1) {
            return Square{ 0, 0, 0 };
        }
        Square pos = track.back();
        
        for (int i = 0; i < pos.side; ++i) {
            for (int j = 0; j < pos.side; ++j) {
                surface.at(pos.x + i).at(pos.y + j) = 0;
            }
        }

        track.pop_back();
        return pos;
    }

    void display(const int& scale) const {
        for (int i = 0; i < side * scale; ++i) {
            for (int j = 0; j < side * scale; ++j) {
                std::cout.width(2);
                std::cout << surface[i / scale][j / scale] << " ";
            }
            std::cout << "\n\n";
        }
    }
};

inline int scaled(const int& n) {
    for (int i = 2; i <= sqrt(n); ++i) {
        if (!(n % i)) {
            return i;
        }
    }
    return n;
}

void findLeastNumberOfSquares(const int& globalSide) {
    int scaledSide = scaled(globalSide);
    SquareTable* table = new SquareTable(scaledSide);
    int leastNum = INT_MAX;
    Square squareFragment;

    if (scaledSide == 2) {
        leastNum = 4;
#ifdef OUTPUT
        std::cout << "Число чётное, выполнить разбиение столешницы на 4 квадрата со стороной " << globalSide / 2 << ".\n\n";
#endif
        squareFragment = { 0, 0, scaledSide / 2 };
        table->writeToTrack(squareFragment);
        squareFragment = { 0, scaledSide / 2, scaledSide / 2 };
        table->writeToTrack(squareFragment);
        squareFragment = { scaledSide / 2, 0, scaledSide / 2 };
        table->writeToTrack(squareFragment);
        squareFragment = { scaledSide / 2, scaledSide / 2, scaledSide / 2 };
        table->writeToTrack(squareFragment);
        table->bestTrack = table->track;
        table->bestSurface = table->surface;
    }
    else {
        int curNum = 3;
#ifdef OUTPUT
        std::cout << "Обнаружено простое нечётное число.\nЗапись трёх начальных квадратов.\n\n";
#endif
        squareFragment = { 0, 0, scaledSide / 2 + 1 };
        table->writeToTrack(squareFragment);

        squareFragment = { 0, scaledSide / 2 + 1, scaledSide / 2 };
        table->writeToTrack(squareFragment);

        squareFragment = { scaledSide / 2 + 1, 0, scaledSide / 2 };
        table->writeToTrack(squareFragment);
#ifdef OUTPUT
        table->bestSurface = table->surface;
        std::cout << "\n\nНачальный вид столешницы:\n\n";
        table->printSurface(globalSide / scaledSide);
#endif

        while (true) {
            squareFragment = table->priorSquare();
            if (squareFragment.x < 0 || curNum >= leastNum) {

                if (curNum < leastNum) {
#ifdef OUTPUT
                    std::cout << "\nОбнаружено лучшее заполнение.\n\nПредыдущий набор (" <<
                        (leastNum==INT_MAX ? 0 : leastNum) << " квадратов):\n";
                    table->printTrack(1);
#endif
                    table->bestTrack = table->track;
                    leastNum = curNum;
#ifdef OUTPUT
                    std::cout << "\nНовый набор (" << leastNum << " квадратов):\n";
                    table->printTrack(1);
                    std::cout << "\n\nЛучшее заполнение столешницы:\n\n\tДо:\n\n";
                    table->printSurface(globalSide / scaledSide);
#endif
                    table->bestSurface = table->surface;
#ifdef OUTPUT
                    std::cout << "\n\tПосле:\n";
                    table->printSurface(globalSide / scaledSide);
#endif
                }
#ifdef OUTPUT
                else {
                    std::cout << "\nТекущее число квадратов достигло минимального.\nВыполнить возврат.\n";
                }
                std::cout << "\nИзъятие из трека всех квадратов вплоть до первого с длиной, отличной от 1 (с учётом масштаба)...\n";
#endif
                do {
                    squareFragment = table->getFromTrack();
                    --curNum;
                } while (squareFragment.side == 1);
#ifdef OUTPUT
                std::cout << "Столешница после возврата:\n\n";
                table->display(globalSide / scaledSide);
#endif
                if (++curNum < 4) {
#ifdef OUTPUT
                    std::cout << "\nПопытка удалить один начальный квадрат или более.\nПрекратить поиск.\n\n";
#endif
                    break;
                }
                --squareFragment.side;
                table->writeToTrack(squareFragment);
#ifdef OUTPUT
                std::cout << "\nДекремент длины последнего изъятого квадрата и запись на исходные координаты.\nТекущий вид столешницы:\n\n";
                table->display(globalSide / scaledSide);
#endif
            }
            else {
                ++curNum;
                table->writeToTrack(squareFragment);
#ifdef OUTPUT
                std::cout << "\nЗапись квадрата максимально допустимого размера с приоритетом (верх, лево). Текущий вид столешницы:\n\n";
                table->display(globalSide / scaledSide);
#endif
            }
        }
    }
    std::cout << "\nВ итоге " << leastNum << " квадратов. Финальный набор:\n";
    table->printTrack(globalSide / scaledSide);
    std::cout << "\n\nНаилучшее заполнение столешницы:\n\n";
    table->printSurface(globalSide / scaledSide);
}

int main()
{
    setlocale(LC_ALL, "Rus");
    int n;
    std::cout << "Пожалуйста, введите длину стороны квадрата: ";
    std::cin >> n;
    clock_t begin = clock();
    findLeastNumberOfSquares(n);
    clock_t end = clock();
    std::cout << "Время выполнения: " << end - begin << "\n";
    system("pause");
    return 0;
}