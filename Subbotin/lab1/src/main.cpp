#include <iostream>
#include <vector>
#include <array>


//структура для квадратиков
struct Square
{
    int size;
    int x;
    int y;
};

struct States
{
    std::array<std::array<int,40>,40> bigSquare;
    std::vector<Square> squareStack;
    std::vector<Square> resultStack;
    States(int size)
    {
        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                bigSquare[i][j] = 0;
            }
        }
    }
};

//функция для вывода квадрата
void printSquare(int size, States& states)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            std::cout.width(3);
            std::cout << states.bigSquare[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

//функция для вывода стека
void printStack(const std::vector<Square>& st)
{
    for(auto &square : st)
    {
        std::cout << "{" << square.x << ", " << square.y << ", " << square.size << "}, ";
    }
    std::cout << std::endl;
}


//функция для вставки основных квадратов
inline void insertSquare(Square mainSquare, States &states)
{
    for (int i = 0; i < mainSquare.size; ++i)
        for (int j = 0; j < mainSquare.size; ++j)
            states.bigSquare[mainSquare.y + i][mainSquare.x + j] = states.squareStack.size()+1;
}

// функция для вставки квадратов, про которые мы точно не знаем какой их размер подойдет
inline Square insertSquare(int bigSquareSize, Square square, States &states)
{
    square.size--;

    // находим максимальный размер, который подойдет квадрату
    int biggestSize = 1;
    while ( biggestSize < square.size &&
            (square.x + biggestSize) < bigSquareSize &&
            (square.y + biggestSize) < bigSquareSize &&
            !states.bigSquare[square.y][square.x + biggestSize] )
        biggestSize++;
    square.size = biggestSize;

    // вставляем квадрат
    for (int i = 0; i < square.size; ++i)
        for (int j = 0; j < square.size; ++j)
            states.bigSquare[square.y + i][square.x + j] = states.squareStack.size()+1;

    return square;
}

// функция по нахождению места под новый квадрат
inline Square emptyCell(int bigSquareSize, States &states)
{
    for (int i = 0; i < bigSquareSize; ++i)
        for (int j = 0; j < bigSquareSize; ++j)
            //существует не занятое место квадратом
            if (states.bigSquare[i][j] == 0)
                return {0, j, i};

    // квадрат заполнен
    return {-1, -1, -1};
}

//функция по добавлению основных квадратов
inline void initialization(Square &square, int size, States &states)
{
    square = {(size + 1) / 2, 0, 0};
    insertSquare(square, states);
    states.squareStack.push_back(square);

    square = {size - (size + 1) / 2, 0, (size + 1) / 2};
    insertSquare(square, states);
    states.squareStack.push_back(square);

    square = {size - (size + 1) / 2, (size + 1) / 2, 0};
    insertSquare(square, states);
    states.squareStack.push_back(square);

    square = emptyCell(size, states);
    square.size = size - 1;
}

//функция по удалению квадрата из стека и массива
inline Square BackWard(States &states)
{
    // вытаскиваем удаляемый квадрат из стека
    Square lastSquare = states.squareStack.back();
    states.squareStack.pop_back();

    // очищаем квадрат
    for (int i = 0; i < lastSquare.size; i++)
        for (int j = 0; j < lastSquare.size; j++)
            states.bigSquare[lastSquare.y + i][lastSquare.x + j] = 0;

    return lastSquare;
}

// основная функция бектрекинга
void doBackTracking(int size, States &states)
{
    Square tempSquare;
    initialization(tempSquare, size, states);

    std::cout << "Запускаем поиск: " << std::endl;
    while (true)
    {
        // если удалим один из трех основных квадратов то можно заканчивать перебор
        if (states.squareStack.size() == 2)
        {
            std::cout << "Элементов в текущем стеке два, значит один из основных квадратов пришлось удалить," <<
                      "заканчиваем поиск." << std::endl;
            break;
        }
        std::cout << "В текущем стеке " << states.squareStack.size() << " элементов" << std::endl;


        bool full = false;

        //loop до тех пор, пока не заполнится квадрат
        while (!full)
        {
            // вставляем квадрат
            states.squareStack.push_back(insertSquare(size, tempSquare, states));
            std::cout << "Вставили квадрат размера " << states.squareStack.back().size << " на x: " << states.squareStack.back().x << " y: " << states.squareStack.back().y << std::endl;
            printSquare(size, states);

            // если в текущем стеке уже столько же элементов, сколько и в результирующем, то дальше смысла двигаться нет
            if (!states.resultStack.empty() && states.squareStack.size() >= states.resultStack.size())
            {
                std::cout << "В текущем стеке элементов больше чем в лучшем, дальше идти смысла нет" << std::endl;
                break;
            }

            // ищем место под следующий квадрат
            tempSquare = emptyCell(size, states);
            if(tempSquare.size > -1)
            {
                std::cout << "Нашли свободное место x:" << tempSquare.x << " y:" << tempSquare.y << " под следующий квадрат"
                          << std::endl;
            }
            else
            {
                std::cout << "Квадрат полностью заполнен." << std::endl;
            }
            full = tempSquare.size == -1;
            tempSquare.size = size - (size + 1) / 2;
        }

        std::cout << "Минимальное количество квадратов: ";
        (states.resultStack.empty() ? std::cout << "еще нет" : std::cout << states.resultStack.size()) << std::endl;
        std::cout << "Текущее количество квадратов: " << states.squareStack.size() << std::endl;
        // если квадрат заполнен и текущих элементов меньше лучших, изменим это
        if (full && states.squareStack.size() < states.resultStack.size() || states.resultStack.empty())
        {
            std::cout << "Текущее количество квадратов оказалось меньше, получаем новый минимум равный: " << states.squareStack.size() << std::endl;
            states.resultStack = states.squareStack;
        }


        /*
         * здесь мы оказываемся в двух случаях:
         * 1)когда заканчивается место в большом квадрате
         * 2)когда путь длиннее чем текущий лучший результат
         * в обоих случаях надо удалить последний квадрат
         * т.к. единичные квадраты в конце расставляются едиственным  способом, поэтому
         * можно удалить их, и следующий за ними квадрат
         */
        std::cout << "Удалим поставленные последними единичные квадраты -- т.к. их можно поставить единственным способом" << std::endl;
        std::cout << "Также в независимости есть ли ед.квадратики удалим следующий за ними не единичный квадрат" << std::endl;
        std::cout << "Стек{x,y,size} до удаления: " << std::endl;
        printStack(states.squareStack);
        do
        {
            tempSquare = BackWard(states);
        }
        while (states.squareStack.size() > 2 && tempSquare.size == 1);
        std::cout << "Стек после удаления: " << std::endl;
        printStack(states.squareStack);
    }
}



int main()
{
    int size;
    std::cout << "Введите сторону квадрата: " << std::endl;
    std::cin >> size;


    //квадраты с четными сторонами всегда можно разделить на 4 равных
    if (size % 2 == 0)
    {
        std::cout << 4 << std::endl;
        std::cout << 1 << " " << 1 << " " << size / 2 << std::endl;
        std::cout << size / 2 + 1 << " " << 1 << " " << size / 2 << std::endl;
        std::cout << 1 << " " << size / 2 + 1 << " " << size / 2 << std::endl;
        std::cout << size / 2 + 1 << " " << size / 2 + 1 << " " << size / 2;
        return 0;
    }

    // находим, если есть делитель стороны квадрата
    // т.к. в квадрат с простыми сторонами точно можем вписать три квадрата
    int divider = 0;
    for (int i = 2; i <= size; i++)
    {
        if (size % i == 0)
        {
            divider = size / i;
            size = i;
            break;
        }
    }

    States states(size);

    doBackTracking(size, states);

    std::cout << std::endl << states.resultStack.size() << std::endl;
    for (auto &square : states.resultStack)
        std::cout << square.x * divider + 1 << " " << square.y * divider + 1 << " " << square.size * divider << std::endl;
    return 0;
}