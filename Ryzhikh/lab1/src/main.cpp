#include <iostream>
#include <vector>

//#define INFO


class Lab1
{
public:
    int N;
    int minCounter;
    //int depth = 0;
    std::vector<std::pair<int, std::pair<int, int>>> resArr;

    void printAnswer(int scale = 1)
    {
        std::cout << minCounter << '\n';
        for (int i = 0; i < minCounter; i++)
        {
            std::cout << resArr[i].second.first * scale + 1 << ' ' << resArr[i].second.second * scale + 1 << ' ' << resArr[i].first * scale << '\n';
        }
    }

    void insertBlock(std::vector<std::vector<bool>>& mainArr, int m, int x, int y) //Вставка квадрата размером m * m с левым верхним углов в точке (x, y)
    {
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < m; j++)
            {
                mainArr[x + i][y + j] = true;
            }
        }
    }

    void removeBlock(std::vector<std::vector<bool>>& mainArr, int m, int x, int y) //Вставка квадрата размером m * m с левым верхним углов в точке (x, y)
    {
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < m; j++)
            {
                mainArr[x + i][y + j] = false;
            }
        }
    }

    std::pair<int, int> findEmpty(std::vector<std::vector<bool>>& mainArr) //Поиск первой свободной ячейки для вставки
    {
        for (int i = N / 2; i < N; i++)
        {
            for (int j = N / 2; j < N; j++)
            {
                if (!mainArr[i][j])
                    return std::make_pair(i, j);
            }
        }
        return std::make_pair(-1, -1);
    }

    std::pair<int, bool> findMaxSize(std::vector<std::vector<bool>>& mainArr, int x, int y)
    {
        for (int i = y + 1; i < N; i++)
        {
            if (mainArr[x][i])
            {
                if (N - x == i - y)
                    return std::make_pair(N - x, true);
                return std::make_pair((N - x > i - y) ? i - y : N - x, false);
            }
        }
        if (N - x == N - y)
            return std::make_pair(N - x, true);
        return std::make_pair((N - x > N - y) ? N - y : N - x, false);
    }

    void chooseBlock(std::vector<std::vector<bool>>& mainArr, std::vector<std::pair<int, std::pair<int, int>>>& tmpArr, int counter, int x, int y)
    {
        #ifdef INFO
        //for (int i = 0; i < depth; i++)
        //    std::cout << "  ";
        std::cout << "Поиск квадрата для вставки...\n";
        #endif
        std::pair<int, int> coord = findEmpty(mainArr);
        if (coord.first == -1)
        {
            if (tmpArr.size() < minCounter)
            {
                resArr = tmpArr;
                minCounter = tmpArr.size();
            }
            #ifdef INFO
            //for (int i = 0; i < depth; i++)
            //    std::cout << "  ";
            std::cout << "  Столешница закончилась!\n\n";
            #endif
            return;
        }
        if (counter + 1 >= minCounter)
        {
            return;
        }
        int tmpBestCounter = minCounter;
        //поиск места для вставки блока
        std::pair<int, bool> maxSize = findMaxSize(mainArr, coord.first, coord.second); //поиск максимального размера блока для вставки на найденное пустое место
        if (maxSize.second)                                                        //Если можно вставить сразу блок максимального размера
        {
            #ifdef INFO
            //for (int i = 0; i < depth; i++)
            //    std::cout << "  ";
            std::cout << "  Вставляем квадрат размера " << maxSize.first << " по координатам (" << coord.first+1 << ',' << coord.second+1 << ")\n\n";
            #endif
            tmpArr.push_back(make_pair(maxSize.first, coord));
            insertBlock(mainArr, maxSize.first, coord.first, coord.second);
            //depth++;
            chooseBlock(mainArr, tmpArr, counter + 1, x, y); //вставляем очередной блок
            removeBlock(mainArr, maxSize.first, coord.first, coord.second);
            #ifdef INFO
            std::cout << "  Удаляем квадрат размера " << maxSize.first << " по координатам (" << coord.first + 1 << ',' << coord.second + 1 << ")\n";
            #endif
            //depth--;
            tmpArr.pop_back();
        }
        else
        {
            for (int i = maxSize.first; i >= 1; i--)
            {
                if (tmpBestCounter > minCounter && i == 1)
                    continue;
                #ifdef INFO
                //for (int i = 0; i < depth; i++)
                //    std::cout << "  ";
                std::cout << "  Вставляем квадрат размера " << i << " по координатам (" << coord.first+1 << ',' << coord.second+1 << ")\n\n";
                #endif
                tmpArr.push_back(make_pair(i, coord));
                insertBlock(mainArr, i, coord.first, coord.second);
                //depth++;
                chooseBlock(mainArr, tmpArr, counter + 1, x, y); //вставляем очередной блок
                removeBlock(mainArr, i, coord.first, coord.second);
                #ifdef INFO
                std::cout << "  Удаляем квадрат размера " << i << " по координатам (" << coord.first + 1 << ',' << coord.second + 1 << ")\n";
                #endif
                //depth--;
                tmpArr.pop_back();
            }
        }
    }

    void primeNumber(std::vector<std::vector<bool>>& mainArr) //вставка начальных блоков и начало работы бэктрекинга
    {
        #ifdef INFO
        std::cout << "Заполнение столешницы можно начать со вставки 3-х квадратов.\n";
        std::cout << "Делается это для сокращения времени работы алгоритма.\n";
        std::cout << "Вставляем квадрат размера "<< N/2+1 << " на координаты (1,1)\n";
        std::cout << "и 2 других квадрата размера " << N/2 << " на координаты (" << N/2+2 << ",1) и (1," << N/2+2 << ").\n\n";
        #endif
        insertBlock(mainArr, N / 2 + 1, 0, 0);
        insertBlock(mainArr, N / 2, N / 2 + 1, 0);
        insertBlock(mainArr, N / 2, 0, N / 2 + 1);
        int counter = 3;
        int minCounter = N * N;
        std::vector<std::pair<int, std::pair<int, int>>> tmpArr;
        tmpArr.push_back(std::make_pair(N / 2 + 1, std::make_pair(0, 0)));
        tmpArr.push_back(std::make_pair(N / 2, std::make_pair(N / 2 + 1, 0)));
        tmpArr.push_back(std::make_pair(N / 2, std::make_pair(0, N / 2 + 1)));
        chooseBlock(mainArr, tmpArr, counter, N / 2, N / 2);
    }

    void division2()
    {
        #ifdef INFO
        std::cout << "Тривиальный случай, когда размер квадрата - чётный.\n";
        std::cout << "Минимальное количество квадратов всегда будет 4, а\n";
        std::cout << "их расположение - по углам столешницы.\n";
        #endif
        if (N % 2 == 0)
        {
            int N_div = N / 2;
            std::cout << 4 << '\n';
            std::cout << 1 << ' ' << 1 << ' ' << N_div << '\n';
            std::cout << N_div + 1 << ' ' << 1 << ' ' << N_div << '\n';
            std::cout << 1 << ' ' << N_div + 1 << ' ' << N_div << '\n';
            std::cout << N_div + 1 << ' ' << N_div + 1 << ' ' << N_div << '\n';
        }
    }

    void division3(std::vector<std::vector<bool>>& mainArr)
    {
        int realN = N;
        int scale = N / 3;
        N = 3;
        primeNumber(mainArr);
        printAnswer(scale);
    }

    void division5(std::vector<std::vector<bool>>& mainArr)
    {
        int realN = N;
        int scale = N / 5;
        N = 5;
        primeNumber(mainArr);
        printAnswer(scale);
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    Lab1 a;
    std::cin >> a.N;
    a.minCounter = a.N * a.N;
    if (a.N % 2 == 0)
    {
        a.division2();
        return 0;
    }
    //создание "карты" столешницы
    std::vector<std::vector<bool>> mainArr(a.N);
    for (int i = 0; i < a.N; i++)
        mainArr[i].resize(a.N);

    if (a.N % 3 == 0)
    {
        a.division3(mainArr);
    }
    else if (a.N % 5 == 0)
    {
        a.division5(mainArr);
    }
    else
    {
        a.primeNumber(mainArr);
        a.printAnswer();
    }

    return 0;
}