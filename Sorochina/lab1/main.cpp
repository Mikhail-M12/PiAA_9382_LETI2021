#include <iostream>
#include <vector>
#include <stack>
#include <cmath>

#define NOTSTEPIK //выводит начальную фразу и кол-во операций
//#define INFO  //выводит сообщения, когда квадрат полон и рекурсия дошла до места, где нет смысла рассматривать дальше
//#define MOREINFO  //выводит сообщения, когда вставляется и удаляеься квадрат
//#define MOREINFOARR // "Расширение" для MOREINFO: выводит квадрат после изменений

struct Square
{          // структура для хранения координат и длины стороны
    int x; // x и y - координаты левой верхней точки
    int y;
    int w; // длина стороны
    Square(int a, int b, int c) : x(a), y(b), w(c) {}
};

//объявление функций
int divider(int);                                                    //функция для поиска делителя
void outputAns(int, int, std::vector<Square>);                       //функция вывода ответа
int isFull(std::vector<std::vector<int>>, int &, int &, int, int &); //функция проверки наличия свободных мест
void printArr(std::vector<std::vector<int>>);                        //функция вывода квадрата
int maxWidth(int, std::vector<std::vector<int>>, int, int, int &);   //функция поиска максимального размера квадрата для заданных координат
void fill(std::vector<std::vector<int>> &, Square);                  //функция заполняющая квадрат значением его длины
int squares(int &, std::vector<std::vector<int>> &, std::vector<Square> &, int, int, int &);
//рекурсивная функция поиска решений

int main()
{
#ifdef NOTSTEPIK
    std::cout << "Please enter n\n";
#endif
    int n, countOperations = 0;
    std::cin >> n;
    if (n < 2 || n > 40)
    {
        std::cout << "incorreccountert input\n";
        return 0;
    }
    std::vector<Square> sqrs;
    int minDiv = divider(n), secondDiv = n, minimal;
    int size = (minDiv) == 1 ? (n) : (minDiv), numOfSqrs;
    std::vector<std::vector<int>> arr(size, std::vector<int>(size, 0));
    if (minDiv != 1)
    {
        minimal = 2 * size + 1;
        secondDiv /= minDiv;
    }
    else
    {
        minimal = n - n / 2 + 4;
    }
    fill(arr, {0, 0, size - size / 2});
    fill(arr, {size - size / 2, 0, size / 2});
    fill(arr, {0, size - size / 2, size / 2});
    int x;
    x = squares(minimal, arr, sqrs, size, 0, countOperations) + 3;
    sqrs.push_back({0, 0, size - size / 2});
    sqrs.push_back({size - size / 2, 0, size / 2});
    sqrs.push_back({0, size - size / 2, size / 2});
    outputAns(size, n / size, sqrs);
#ifdef NOTSTEPIK
    std::cout << "Number of Operations is " << countOperations << "\n";
#endif
    return 0;
}

int divider(int n)
{
    //функция для поиска делителя
    //возвращает наименьший простой делитель, иначе 1
    if (n % 2 == 0)
    {
        return 2;
    }
    if (n % 3 == 0)
    {
        return 3;
    }
    if (n % 5 == 0)
    {
        return 5;
    }
    return 1;
}

void outputAns(int n, int m, std::vector<Square> sqrs)
{
    //функция вывода ответа
    //n - размер, m - второй делитель (1, если простое)
    std::cout << sqrs.size() << '\n';
    for (auto i : sqrs)
    {
        std::cout << i.x * m + 1 << ' ' << i.y * m + 1 << ' ' << i.w * m << '\n';
    }
}

int isFull(std::vector<std::vector<int>> arr, int &y, int &x, int n, int &countOperations)
{
    //функция проверки наличия свободных мест
    //1 - нет мест, 0 - есть место
    //если есть ноль, то его координаты будут записаны в x и y
    for (x = n / 2; x < n; x++)
    {
        for (y = n / 2; y < n; y++)
        {
            countOperations++;
            if (arr[x][y] == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

void printArr(std::vector<std::vector<int>> arr)
{
    //функция вывода квадрата
    for (auto i : arr)
    {
        for (auto j : i)
        {
            std::cout << j << " ";
            if (j < 10)
            {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int maxWidth(int size, std::vector<std::vector<int>> arr, int i, int j, int &countOperations)
{
    //функция поиска максимального размера квадрата для заданных координат i и j (y и x)
    int count = 1, xcount = 1, ycount = 1;
    while (j + xcount < size && arr[i][j + xcount] == 0)
    {
        xcount++;
    }
    while (i + ycount < size && arr[i + ycount][j] == 0)
    {
        ycount++;
    }
    countOperations += (xcount + ycount);
    count = (xcount) < (ycount) ? (xcount) : (ycount);
    return count;
}

void fill(std::vector<std::vector<int>> &arr, Square sqr)
{
    //функция заполняющая квадрат значением его длины
    for (int i = sqr.y; i < sqr.y + sqr.w; i++)
    {
        for (int j = sqr.x; j < sqr.x + sqr.w; j++)
        {
            arr[i][j] = sqr.w;
        }
    }
}

int squares(int &maxCount, std::vector<std::vector<int>> &arr, std::vector<Square> &sqrs, int n, int recLevel, int &countOperations)
{
    //рекурсивная функция поиска лучшего решения
    //возвращает кол-во квадратов
    countOperations++;
    if (maxCount < recLevel) //сравнение уровня рекурсии с макс. кол-вом квадратов, чтобы отбросить заведомо ненужные варианты
    {
#ifdef INFO
        std::cout << "Recursion level: " << recLevel << " maxCount = " << maxCount << ". Нет смысла дальше рассматривать.\n";
#endif
        return -1;
    }
    int x, y;
    if (isFull(arr, x, y, n, countOperations))
    {
        if (recLevel < maxCount)
        {
            maxCount = recLevel;
        }
#ifdef INFO
        std::cout << "Recursion level: " << recLevel << " maxCount = " << maxCount << ". Arr full.\n";
        printArr(arr);
        countOperations += (n * n);
#endif
        return 0;
    }
    std::vector<Square> current, best;
    Square max(x, y, maxWidth(n, arr, y, x, countOperations));
    Square curr = max;
    int count, minCount = maxCount - recLevel, goodWidth = max.w;
    for (int width = max.w; width >= 1; width--) //цикл перебора размеров квадрата для заданных координат пустой точки
    {
        countOperations++;
        fill(arr, {max.x, max.y, width});
        countOperations += (width * width); // количество проходов по циклу в fill
#ifdef MOREINFO
        std::cout << "Recursion level: " << recLevel << " Сторона вставленного квадрата: " << width << ". Координаты: x = " << x << " y = " << y << ".\n";
#ifdef MOREINFOARR
        printArr(arr);
        countOperations += (n * n);
#endif
#endif
        count = squares(maxCount, arr, current, n, recLevel + 1, countOperations) + 1; //рекурсивный вызов
        if (minCount > count && count > 0)
        {
            //если насчитали меньше чем было, то перезаписываем сторону квадрата и вектор, хранящий разбиение
            minCount = count;
            goodWidth = width;
            best.clear();
            for (auto i : current)
            {
                best.push_back(i);
            }
        }
        else
        {
            //иначе очищаем текущий вектор
            current.clear();
        }
        for (int i = 0; i < width; i++) //удаление вставленного квадрата
        {
            for (int j = 0; j < width; j++)
            {
                arr[max.y + i][max.x + j] = 0;
            }
        }
        countOperations += (width * width); // количество проходов по циклу удаления
#ifdef MOREINFO
        std::cout << "Recursion level: " << recLevel << " Сторона удалённого квадрата: " << width << ". Координаты: x = " << x << " y = " << y << ".\n";
#ifdef MOREINFOARR
        printArr(arr);
        countOperations += (n * n);
#endif
#endif
    }
    sqrs.clear();
    for (auto i : best)
    {
        sqrs.push_back(i);
    }
    sqrs.push_back({max.x, max.y, goodWidth}); //добавляем в точку с координатами х, у квадрат длины goodWidth
    return minCount;
}