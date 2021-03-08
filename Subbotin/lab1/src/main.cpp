#include <iostream>
#include <vector>

using namespace std;

//структура для квадратиков
struct Square
{
    int size;
    int x;
    int y;
};

//глобальные переменные
int bigSquare[40][40];
vector<Square> squareStack;
vector<Square> resultStack;

//функция для вывода квадрата
void printSquare(int size){
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            printf("%2i ", bigSquare[i][j]);
        }
        printf("\n");
    }
}

//функция для вывода стека
void printStack(const vector<Square>& st){
    for(auto &square : st){
        cout << "{" << square.x << ", " << square.y << ", " << square.size << "}, ";
    }
    cout << endl;
}


//функция для вставки основных квадратов
inline void insertSquare(Square mainSquare)
{
    for (int i = 0; i < mainSquare.size; ++i)
        for (int j = 0; j < mainSquare.size; ++j)
            bigSquare[mainSquare.y + i][mainSquare.x + j] = squareStack.size()+1;
}

// функция для вставки квадратов, про которые мы точно не знаем какой их размер подойдет
inline Square insertSquare(int bigSquareSize, Square square)
{
    square.size--;

    // находим максимальный размер, который подойдет квадрату
    int biggestSize = 1;
    while (biggestSize < square.size && (square.x + biggestSize) < bigSquareSize && (square.y + biggestSize) < bigSquareSize && !bigSquare[square.y][square.x + biggestSize])
        biggestSize++;
    square.size = biggestSize;

    // вставляем квадрат
    for (int i = 0; i < square.size; ++i)
        for (int j = 0; j < square.size; ++j)
            bigSquare[square.y + i][square.x + j] = squareStack.size()+1;

    return square;
}

// функция по нахождению места под новый квадрат
inline Square emptyCell(int bigSquareSize)
{
    for (int i = 0; i < bigSquareSize; ++i)
        for (int j = 0; j < bigSquareSize; ++j)
            //существует не занятое место квадратом
            if (bigSquare[i][j] == 0)
                return {0, j, i};

    // квадрат заполнен
    return {-1, -1, -1};
}

//функция по добавлению основных квадратов
inline void initialization(Square &square, int size)
{
    square = {(size + 1) / 2, 0, 0};
    insertSquare(square);
    squareStack.push_back(square);

    square = {size - (size + 1) / 2, 0, (size + 1) / 2};
    insertSquare(square);
    squareStack.push_back(square);

    square = {size - (size + 1) / 2, (size + 1) / 2, 0};
    insertSquare(square);
    squareStack.push_back(square);

    square = emptyCell(size);
    square.size = size - 1;
}

//функция по удалению квадрата из стека и массива
inline Square BackWard(const int &size)
{
    // вытаскиваем удаляемый квадрат из стека
    Square lastSquare = squareStack.back();
    squareStack.pop_back();

    // очищаем квадрат
    for (int i = 0; i < lastSquare.size; i++)
        for (int j = 0; j < lastSquare.size; j++)
            bigSquare[lastSquare.y + i][lastSquare.x + j] = 0;

    return lastSquare;
}

// основная функция бектрекинга
void doBackTracking(int size)
{
    Square tempSquare;
    initialization(tempSquare, size);

    cout << "Запускаем поиск: " << endl;
    while (true)
    {
        // если удалим один из трех основных квадратов то можно заканчивать перебор
        if (squareStack.size() == 2){
            cout << "Элементов в текущем стеке два, значит один из основных квадратов пришлось удалить," <<
                    "заканчиваем поиск." << endl;
            break;
        }
        cout << "В текущем стеке " << squareStack.size() << " элементов" << endl;


        bool full = false;

        //loop до тех пор, пока не заполнится квадрат
        while (!full)
        {
            // вставляем квадрат
            squareStack.push_back(insertSquare(size, tempSquare));
            cout << "Вставили квадрат размера " << squareStack.back().size << " на x: " << squareStack.back().x << " y: " << squareStack.back().y << endl;
            printSquare(size);

            // если в текущем стеке уже столько же элементов, сколько и в результирующем, то дальше смысла двигаться нет
            if (!resultStack.empty() && squareStack.size() >= resultStack.size())
            {
                cout << "В текущем стеке элементов больше чем в лучшем, дальше идти смысла нет" << endl;
                break;
            }

            // ищем место под следующий квадрат
            tempSquare = emptyCell(size);
            if(tempSquare.size > -1) {
                cout << "Нашли свободное место x:" << tempSquare.x << " y:" << tempSquare.y << " под следующий квадрат"
                     << endl;
            }
            else {
                cout << "Квадрат полностью заполнен." << endl;
            }
            full = tempSquare.size == -1;
            tempSquare.size = size - (size + 1) / 2;
        }

        cout << "Минимальное количество квадратов: ";
        (resultStack.empty() ? cout << "еще нет" : cout << resultStack.size()) << endl;
        cout << "Текущее количество квадратов: " << squareStack.size() << endl;
        // если квадрат заполнен и текущих элементов меньше лучших, изменим это
        if (full && squareStack.size() < resultStack.size() || resultStack.empty())
        {
            cout << "Текущее количество квадратов оказалось меньше, получаем новый минимум равный: " << squareStack.size() << endl;
            resultStack = squareStack;
        }


        /*
         * здесь мы оказываемся в двух случаях:
         * 1)когда заканчивается место в большом квадрате
         * 2)когда путь длиннее чем текущий лучший результат
         * в обоих случаях надо удалить последний квадрат
         * т.к. единичные квадраты в конце расставляются едиственным  способом, поэтому
         * можно удалить их, и следующий за ними квадрат
         */
        cout << "Будем удалять элементы из стека {x,y,size}: " << endl;
        printStack(squareStack);
        do
        {
            tempSquare = BackWard(size);
        } while (squareStack.size() > 2 && tempSquare.size == 1);
        cout << "Стек после удаления: " << endl;
        printStack(squareStack);
    }
}



int main()
{
    int size;
    cout << "Введите сторону квадрата: ";
    cin >> size;

    //квадраты с четными сторонами всегда можно разделить на 4 равных
    if (size % 2 == 0)
    {
        cout << 4 << endl;
        cout << 1 << " " << 1 << " " << size / 2 << endl;
        cout << size / 2 + 1 << " " << 1 << " " << size / 2 << endl;
        cout << 1 << " " << size / 2 + 1 << " " << size / 2 << endl;
        cout << size / 2 + 1 << " " << size / 2 + 1 << " " << size / 2;
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

    doBackTracking(size);

    cout << endl << resultStack.size() << endl;
    for (auto &square : resultStack)
        cout << square.x * divider + 1 << " " << square.y * divider + 1 << " " << square.size * divider << endl;
    return 0;
}