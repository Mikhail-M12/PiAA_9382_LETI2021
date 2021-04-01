#include <iostream>
#include <vector>
#include <ctime>

// строчку ниже можно комментировать/раскомментировать для включения/отключения промежуточного вывода
//#define ADDINFO

// отвечает за подсчет и вывод количества выполненных операций
//#define COUNTINFO

#ifdef COUNTINFO
int operationsCount = 0;
#endif

class Square // класс НЕ "видимых" квадратов
{
public:
    int x;
    int y;
    int size; // длина стороны квадрата
};

bool isPossibleToAddSquare(std::vector<std::vector<int>>& visSqr, int x, int y, int sizeOfSmallSqr) 
{
    #ifdef COUNTINFO
    ++operationsCount;
    #endif

    // проверка на выход за пределы квадрата
    if ((x + sizeOfSmallSqr) > visSqr.size() || (y + sizeOfSmallSqr) > visSqr.size())
    {
        return false;
    }

    // проверка пустоты выбранной области
    for (int i = y; i < y + sizeOfSmallSqr; i++) 
    {
        for (int j = x; j < x + sizeOfSmallSqr; j++) 
        {
            if (visSqr[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

void addToVisibleSquare(std::vector<std::vector<int>>& visSqr, int x, int y, int sizeOfSmallSqr) // не делает проверок, сразу "красит"
{
    #ifdef COUNTINFO
    ++operationsCount;
    #endif

    for (int i = y; i < y + sizeOfSmallSqr; i++) 
    {
        for (int j = x; j < x + sizeOfSmallSqr; j++) 
        {
            visSqr[i][j] = sizeOfSmallSqr;
        }
    }
}

void printVisibleSquare(std::vector <std::vector <int>>& visSqr, int compression, int sideLenOfMainSquare) 
{
    for (int i = 0; i < sideLenOfMainSquare * compression; i++) 
    {
        for (int j = 0; j < sideLenOfMainSquare * compression; j++) 
        {
            std::cout.width(3); // для красивого вывода
            std::cout << visSqr[i][j];
        }
        std::cout << std::endl;
    }
}

void fillVisibleSquareWithZeros(std::vector<std::vector<int>>& visSqr, int sideLenOfMainSquare) // заполнен нулями == не поставлено ни одного квадрата
{
    #ifdef COUNTINFO
    ++operationsCount;
    #endif

    visSqr.resize(sideLenOfMainSquare);
    for (int i = 0; i < sideLenOfMainSquare; i++) 
    {
        visSqr[i].resize(sideLenOfMainSquare);

        for (int j = 0; j < sideLenOfMainSquare; j++) 
        {
            visSqr[i][j] = 0;
        }
    }
}

void delLastAddedSquare(std::vector<std::vector<int>>& visSqr, std::vector<Square>& currentArrOfAddedSquares, int spacesCount) 
{
    #ifdef COUNTINFO
    ++operationsCount;
    #endif

    Square removableSquare = currentArrOfAddedSquares.back(); 
    currentArrOfAddedSquares.pop_back();

    // "обнуление" ранее занятых клеток в видимом квадрате
    for (int i = removableSquare.y; i < removableSquare.y + removableSquare.size; i++) 
    {
        for (int j = removableSquare.x; j < removableSquare.x + removableSquare.size; j++) 
        {
            visSqr[i][j] = 0;
        }
    }

    #ifdef ADDINFO
    for(int l = 0; l < spacesCount; l++)
    {
        std::cout << " ";
    }
    std::cout << "Удаление кв. со стороной " << removableSquare.size << " (x = " <<  removableSquare.x + 1 << ", y = " << removableSquare.y + 1 << ')' << std::endl;
    #endif
}


void makeTaskPreparations(int &compression, std::vector<std::vector<int>>& visSqr, std::vector<Square>& currentArrOfAddedSquares, int& freeAreaOfMainSquare, int& sideLenOfMainSquare, int& bestSquaresCount) 
{
    #ifdef COUNTINFO
    ++operationsCount;
    #endif

    fillVisibleSquareWithZeros(visSqr, sideLenOfMainSquare); // инициализация пустого "видимого" квадрата

    // "сжатие" квадрата, чтобы облегчить дальнейшие расчеты
    if (sideLenOfMainSquare % 2 == 0) 
    {
        compression = sideLenOfMainSquare / 2;
        sideLenOfMainSquare = 2;
    }
    else if (sideLenOfMainSquare % 3 == 0) 
    {
        compression = sideLenOfMainSquare / 3;
        sideLenOfMainSquare = 3;
    }
    else if (sideLenOfMainSquare % 5 == 0) 
    {
        compression = sideLenOfMainSquare / 5;
        sideLenOfMainSquare = 5;
    }
    // так можно продолжать для всех простых чисел


    bestSquaresCount = 2 * sideLenOfMainSquare + 1;

    // выставление первых трех квадратов
    currentArrOfAddedSquares.push_back({ 0, 0, (sideLenOfMainSquare + 1) / 2 });
    currentArrOfAddedSquares.push_back({ 0, (sideLenOfMainSquare + 1) / 2, sideLenOfMainSquare / 2 });
    currentArrOfAddedSquares.push_back({ (sideLenOfMainSquare + 1) / 2, 0, sideLenOfMainSquare / 2 });

    addToVisibleSquare(visSqr, 0, 0, (sideLenOfMainSquare + 1) / 2);
    addToVisibleSquare(visSqr, 0, (sideLenOfMainSquare + 1) / 2, sideLenOfMainSquare / 2);
    addToVisibleSquare(visSqr, (sideLenOfMainSquare + 1) / 2, 0, sideLenOfMainSquare / 2);

    #ifdef ADDINFO
    std::cout << "Вставка кв. со стороной " << (sideLenOfMainSquare + 1) / 2 << " (x = " <<  1 << ", y = " << 1 << ")" << std::endl;
    std::cout << "Вставка кв. со стороной " << sideLenOfMainSquare / 2 << " (x = " <<  1 << ", y = " << (sideLenOfMainSquare + 1) / 2 + 1 << ")" << std::endl;
    std::cout << "Вставка кв. со стороной " << sideLenOfMainSquare / 2 << " (x = " <<  (sideLenOfMainSquare + 1) / 2 + 1 << ", y = " << 1 << ")" << std::endl;
    #endif

    // обновление к-ва пустого пространства
    freeAreaOfMainSquare = sideLenOfMainSquare * sideLenOfMainSquare - ((sideLenOfMainSquare + 1) / 2) * ((sideLenOfMainSquare + 1) / 2) - 2 * (sideLenOfMainSquare / 2) * (sideLenOfMainSquare / 2);
}

void findTaskAnswerWithRecursion(std::vector<std::vector<int>>& visSqr, int freeAreaOfMainSquare, int currentSizeOfAddedSquare, int currentCountOfAddedSquares, std::vector<Square>& currentArrOfAddedSquares, int spacesCount, int sideLenOfMainSquare, int& bestSquaresCount, std::vector<Square>& bestArrOfAddedSquares) 
{
    #ifdef COUNTINFO
    ++operationsCount;
    #endif

    // даже если квадрат заполнит все пустоты - результат будет не лучше нынешнего
    if ( (currentCountOfAddedSquares == (bestSquaresCount - 1)) && (freeAreaOfMainSquare) ) 
    {
        #ifdef ADDINFO
        for(int l = 0; l < spacesCount; l++)
        {
            std::cout << " ";
        }
        std::cout << "Разложение не минимально, выход из рекурсии" << std::endl;
        #endif

        return;
    }

    // первый добавленный квадрат (после начальных 3)
    if ( ((currentSizeOfAddedSquare + 1) <= (sideLenOfMainSquare / 2)) && (currentCountOfAddedSquares == 3) ) 
    {
        // рекурсивный вызов этой же функции
        findTaskAnswerWithRecursion(visSqr, freeAreaOfMainSquare, (currentSizeOfAddedSquare+1), currentArrOfAddedSquares.size(), currentArrOfAddedSquares, 0, sideLenOfMainSquare, bestSquaresCount, bestArrOfAddedSquares);
    }


    bool possibleToAddSuchSquare = false;
    for (int y = 0; y < sideLenOfMainSquare; y++) 
    {
        for (int x = 0; x < sideLenOfMainSquare; x++) 
        {
            // для каждой пустой клетки происходит попытка вставить квадрат текущего размера
            if (visSqr[y][x] == 0) 
            {
                if (isPossibleToAddSquare(visSqr, x, y, currentSizeOfAddedSquare))
                {
                    possibleToAddSuchSquare = true;
                    addToVisibleSquare(visSqr, x, y, currentSizeOfAddedSquare);
                    freeAreaOfMainSquare -= currentSizeOfAddedSquare * currentSizeOfAddedSquare;
                    currentArrOfAddedSquares.push_back({ x, y, currentSizeOfAddedSquare });

                    #ifdef ADDINFO
                    for(int l=0; l < spacesCount; l++)
                    {
                        std::cout << " ";
                    }
                    std::cout << "Вставка кв. со стороной  " << currentSizeOfAddedSquare<< " (x = " <<  x + 1 << ", y = " << y + 1 << ")" << std::endl;
                    #endif

                    break;
                }
                else
                {
                    #ifdef ADDINFO
                    for(int l=0; l < spacesCount; l++)
                    {
                        std::cout << " ";
                    }
                    std::cout << "Нельзя поставить кв. со стороной " << currentSizeOfAddedSquare<< " (x = " <<  x + 1 << ", y = " << y + 1 << ")" << std::endl;
                    #endif

                    return;
                }
            }
            else 
            {
                x += (visSqr[y][x] - 1);
            }
        }

        // выход из цикла
        if(possibleToAddSuchSquare) 
        {
            break;
        }
    }

    // нет смысла обновлять показатели лучшего результата при таком же количестве обрезков
    if ( (currentCountOfAddedSquares + 1) == bestSquaresCount) 
    {
        #ifdef ADDINFO
        for(int l = 0; l < spacesCount; l++)
        {
            std::cout << " ";
        }
        std::cout << "Разложение не минимально, выход из рекурсии" << '\n';
        #endif

        delLastAddedSquare(visSqr, currentArrOfAddedSquares, spacesCount);
        return;
    }

    // минимальное заполнение
    if ( ((currentCountOfAddedSquares + 1) < bestSquaresCount) && (freeAreaOfMainSquare == 0)) 
    {
        bestSquaresCount = currentCountOfAddedSquares + 1;
        bestArrOfAddedSquares.assign(currentArrOfAddedSquares.begin(), currentArrOfAddedSquares.end());

        #ifdef ADDINFO
        std::cout << "*Получено новое минимальное к-во кв.: " << bestSquaresCount << std::endl;
        #endif

        delLastAddedSquare(visSqr, currentArrOfAddedSquares, spacesCount);
        return;
    }

    // рекурсивный вызов этой же функции
    for (int i = sideLenOfMainSquare / 2; i > 0; i--) 
    {
        if (i * i <= freeAreaOfMainSquare) 
        {
            #ifdef ADDINFO
            for (int l=0; l < spacesCount+2; l++)
            {
                std::cout << " ";
            }
            std::cout << "Вызов рекурсии для кв. со стороной " << i << '\n';
            #endif

            findTaskAnswerWithRecursion(visSqr, freeAreaOfMainSquare, i, currentCountOfAddedSquares + 1, currentArrOfAddedSquares, spacesCount+2, sideLenOfMainSquare, bestSquaresCount, bestArrOfAddedSquares);
        }
    }

    delLastAddedSquare(visSqr, currentArrOfAddedSquares, spacesCount);
}

int main() 
{
    setlocale(LC_ALL, "Russian");

    std::vector <std::vector<int>> visibleSquare; // изображение квадрата и его занятой площади в виде цифр
    std::vector <Square> currentArrOfAddedSquares;
    std::vector <Square> bestArrOfAddedSquares; // для хранения лучшей попытки

    int compression = 1; // коэффициент сжатия квадрата
    int freeAreaOfMainSquare;
    int sideLenOfMainSquare;
    int bestSquaresCount; // минимальное к-во квадратов, которым можно покрыть основной

    #ifdef ADDINFO
    std::cout << "Введите размер стороны квадрата (от 2 до 20):" << std::endl;
    #endif
    std::cin >> sideLenOfMainSquare;


    makeTaskPreparations(compression, visibleSquare, currentArrOfAddedSquares, freeAreaOfMainSquare, sideLenOfMainSquare, bestSquaresCount);

    #ifdef ADDINFO
    clock_t start = clock();
    #endif

    // 1 - это начальный размер добавляемого квадрата,  0 - это глубина рекурсии для отладочных сообщений 
    findTaskAnswerWithRecursion(visibleSquare, freeAreaOfMainSquare, 1, currentArrOfAddedSquares.size(), currentArrOfAddedSquares, 0, sideLenOfMainSquare, bestSquaresCount, bestArrOfAddedSquares);

    #ifdef ADDINFO
    clock_t end = clock();

    std::cout << "\nВремя выполнения: " << (double) (end - start) / CLOCKS_PER_SEC << "\n\n";
    #endif
    std::cout << bestSquaresCount << std::endl;
    

    #ifdef COUNTINFO
    std::cout << "\nКоличество операций = " << ::operationsCount << std::endl;
    #endif

    // вывод ответа и загрузка лучшего найденного разложения в "видимый" квадрат
    for (int i = 0; i < bestArrOfAddedSquares.size(); i++)
    {
        std::cout << bestArrOfAddedSquares[i].x * compression + 1 << " " << bestArrOfAddedSquares[i].y * compression + 1 << " " << bestArrOfAddedSquares[i].size * compression << std::endl;

        #ifdef ADDINFO
        addToVisibleSquare(visibleSquare, bestArrOfAddedSquares[i].x * compression, bestArrOfAddedSquares[i].y * compression, bestArrOfAddedSquares[i].size * compression);
        #endif
    }

    #ifdef ADDINFO
    std::cout << std::endl;
    printVisibleSquare(visibleSquare, compression, sideLenOfMainSquare);
    #endif

    return 0;
}