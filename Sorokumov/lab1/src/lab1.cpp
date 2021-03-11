#include <stack>
#include <ctime>
#include <iostream>
#include <cmath>

//Структура квадрата
struct Square{
    int x;
    int y;
    int length;
};

//Проверка кратнности
int multiplicity(int n){
    if(n % 2 == 0 && n != 2)
        return 2;
    if(n % 3 == 0 && n != 3)
        return 3;
    if(n % 5 == 0 && n != 5)
        return 5;
    return 1;
}

//Распечатка квадрата
void demonstration(int** squareArray, int n){
    std::cout << "Текущее состояние:" <<std::endl;
    for(int m = 0; m < n; m++){
        for(int l = 0; l < n; l++)
            std::cout << squareArray[m][l]<< ' ';
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

//Проверяет возможность вместить квадрат определенной длины
int moreSquare(int **squareArray, int x, int y, int lengthSquare){
    for(int i = 0; i < lengthSquare; i++)
        if(squareArray[x + lengthSquare - 1][y + i] || squareArray[x + i][y + lengthSquare - 1])
            return 1;
    return 0;
}

//Раскрашивает квадрат
void fillSquare(int **squareArray, int x , int y, int lengthSquare){
    for(int i = 0; i < lengthSquare; i++)
        for(int j = 0; j < lengthSquare; j++)
            squareArray[x + i][y + j] = lengthSquare;
}

//Копирвание стека
void stackCopy(std::stack <Square> * stackSquares, std::stack <Square> * stackSquaresCopy){
    while(!stackSquares->empty()){
        stackSquaresCopy->push(stackSquares->top());
        stackSquares->pop();
    }
}

// Нахождение пустой клетки
int findEmptyCell(int **squareArray, int &x, int &y, int n){
    for(x = 0; x < n; x++){
        for(y = 0; y < n - 1; y++)
            if(squareArray[x][y] == 0)
                break;
        if(squareArray[x][y] == 0)
            break;
    }
    if(x == n){
        x = n - 1;
    }
    if(squareArray[x][y] != 0)
        return 1;
    return 0;
}

//Опустащает стек
void emptyStack(std::stack <Square> *stackSquares){
    while(!stackSquares->empty())
        stackSquares->pop();
}

//Очищает квадрат
void clearSquare(int **squareArray, int x, int y, int lengthSquare){
    for(int i = 0; i < lengthSquare; i++)
        for(int j = 0; j < lengthSquare; j++)
            squareArray[x+i][y+j] = 0;
}

//Находит рекурсивно минимальное распледеление квадрата
int findMinSqrs(int limit, std::stack <Square> *stackSquares, int **squareArray, int n, int countRec, int scale){
    if(limit < countRec){ //Условие выхода за пределы
        std::cout << std::endl << std::endl << "Выход за пределы." <<std:: endl;
        return -1;
    } else{
        std::cout << "Поставим следующий квадрат." << std::endl;
    }
    demonstration(squareArray, n);//Вывод заполненности квадрата
    int x, y;
    if(findEmptyCell(squareArray, x, y, n)) // Нахождение пустой клетки
    {
        std::cout << "Пустых клеток не осталось." << std::endl;
        return 0;
    }else{
        std::cout << "Найдена пустая клетка: х = " << x << ", y = "<< y << std::endl;
    }
    std::stack <Square> stackSquaresTemp; //Временный стек квадратов
    std::stack <Square> stackSquaresMin;//Итоговый стек квадратов
    int distanceToTheBorder = (n - x > n - y) ? (n - y - !squareArray[0][0]) : (n - x - !squareArray[0][0]); //Хранит расстояние до границы
    std::cout << "Расстояние до границ = " << distanceToTheBorder << std::endl;
    int lengthSquare, minCount = limit + 1, k, need_length = 1;
    std::cout << "Перебер длин квадрата:" << std::endl;
    for(lengthSquare = 1; lengthSquare <= distanceToTheBorder; lengthSquare++){ //Пробегает по длине квадрата
        std::cout << "Текущая длина: " << lengthSquare << std::endl;
        std::cout << "Заполним квадрат. ";
        fillSquare(squareArray, x, y, lengthSquare);//Заполняет квадрат
        k = findMinSqrs(limit, &stackSquaresTemp, squareArray, n, countRec + 1, scale) + 1;
        if(k < minCount && k != 0){ //Если надено распределение с меньшим количеством квадратов сохраним стек квадратов
            std::cout << "Нашли распределение лучше, чем " << limit + 1 << ". Сохраним его." << std::endl;
            minCount = k; //Сохраняем минимальное распеределение
            need_length = lengthSquare;
            emptyStack(&stackSquaresMin);
            stackCopy(&stackSquaresTemp, &stackSquaresMin);
        }
        else {//Иначе очищаем временный стек
            std::cout << "Было найдено распределение по-лучше, очистим стек квадратов." << std::endl;
            emptyStack(&stackSquaresTemp);
        }
        std::cout << "Сотрем квадрат длинной = " << lengthSquare << " в точке х = " << x << ", y = " << y << "." << std::endl;
        clearSquare(squareArray, x, y, lengthSquare);//Стираем нарисованный квадрат
        demonstration(squareArray, n);

        if(distanceToTheBorder - lengthSquare - 1 < 0) break;
        if(moreSquare(squareArray, x, y, lengthSquare + 1)) // Если нельзя вместить квадрат побольше, выходим
        {
            std::cout << "Квадрат побольше вместить не получилось." << std::endl;
            break;
        }

    }
    emptyStack(stackSquares);
    stackCopy(&stackSquaresMin, stackSquares); //Копируем стек в основнй стек
    stackSquares->push({x * scale + 1, y * scale + 1, need_length * scale});
    return minCount;
}

int main(){
    int n, minCount;
    std::cout << "Введите длину квадрата, для которого вы хотите получить минимальное распределение" << std::endl;
    std::cin >> n;
    if(!(n>=2 && n<=40)){
        std::cout << "Ошибка, введено неверное число." << std::endl;
        return 0;
    }
    std::stack <Square> stackSquares;
    int mul = multiplicity(n);
    int limit = 6.37 * sqrt(sqrt((mul == 1) ? n : mul));//Задаем предельную величину для количества квадратов
    int **squareArray ;

    if(mul!=1){
        squareArray = new int *[mul];
        for(int i = 0; i < mul; i++)
            squareArray[i] = new int[mul]();
        minCount = findMinSqrs(limit, &stackSquares, squareArray, mul, 0, n / mul);
    }
    else {
        squareArray = new int *[n];
        for(int i = 0; i < n; i++)
            squareArray[i] = new int[n]();

        int half = n - n / 2;
        fillSquare(squareArray, 0, 0, half);
        fillSquare(squareArray, half, 0, half - 1);
        fillSquare(squareArray, 0, half, half - 1);

        minCount = findMinSqrs(limit - 3, &stackSquares, squareArray, n, 0, 1) + 3;
        stackSquares.push({half + 1, 1, half - 1});
        stackSquares.push({1, half + 1, half - 1});
        stackSquares.push({1, 1, half});
    }
    Square tmp;
    std::cout << "Для квадрата со стороной " << n << std::endl;
    std::cout << "Количество = " << minCount << std::endl;
    std::cout << "Минимальное распределение квадратов:" << std::endl;
    for(int j = 0; j < minCount; j++){
        tmp = stackSquares.top();
        std::cout << j + 1 << ") x = " << tmp.x << ", y = " << tmp.y << ", длина = " << tmp.length << std::endl;
        stackSquares.pop();
    }
    if(mul!=1){
        for(int i = 0; i < mul; i++)
            delete [] squareArray[i];
    }
    else{
        for(int i = 0; i < n; i++)
            delete [] squareArray[i];
    }
    delete [] squareArray;
    return 0;
}

extern "C"{
    int autoTest(int n){
        int  minCount;
        if(!(n>=2 && n<=40)){
            std::cout << "Ошибка, введено неверное число." << std::endl;
            return 0;
        }
        std::stack <Square> stackSquares;
        int mul = multiplicity(n);
        int limit = 6.37 * sqrt(sqrt((mul == 1) ? n : mul));//Задаем предельную величину для количества квадратов
        int **squareArray ;

        if(mul!=1){
            squareArray = new int *[mul];
            for(int i = 0; i < mul; i++)
                squareArray[i] = new int[mul]();
            minCount = findMinSqrs(limit, &stackSquares, squareArray, mul, 0, n / mul);
        }
        else {
            squareArray = new int *[n];
            for(int i = 0; i < n; i++)
                squareArray[i] = new int[n]();

            int half = n - n / 2;
            fillSquare(squareArray, 0, 0, half);
            fillSquare(squareArray, half, 0, half - 1);
            fillSquare(squareArray, 0, half, half - 1);

            minCount = findMinSqrs(limit - 3, &stackSquares, squareArray, n, 0, 1) + 3;
            stackSquares.push({half + 1, 1, half - 1});
            stackSquares.push({1, half + 1, half - 1});
            stackSquares.push({1, 1, half});
        }

        Square tmp;
        std::cout << "Для квадрата стороной " << n << std::endl;
        std::cout << "Количество = " << minCount << std::endl;
        std::cout << "Минимальное распределение квадратов:" << std::endl;
        for(int j = 0; j < minCount; j++){
            tmp = stackSquares.top();
            std::cout << j + 1 << ") x = " << tmp.x << ", y = " << tmp.y << ", длина = " << tmp.length << std::endl;
            stackSquares.pop();
        }
        if(mul!=1){
            for(int i = 0; i < mul; i++)
                delete [] squareArray[i];
        }
        else{
            for(int i = 0; i < n; i++)
                delete [] squareArray[i];
        }
        delete [] squareArray;
        return 0;
    }
}