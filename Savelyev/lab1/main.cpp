#include <vector>
#include <ctime>
#include <iostream>


using namespace std;

// Структура описывающая квадрат
struct Square {
    int x;
    int y;
    int size;
};

// Проверяет возможность вставить квадрат
bool can_insert_sqr(vector<vector<int>>& sqr, int x, int y, int size) {
    // Проверет, что вставляемый квадрат не выйдет за пределы поля
    if ((x + size) > sqr.size() || (y + size) > sqr.size())
        return 0;
    // Проверяет, что область, в которую вставляют квадрат не занята
    for (int i = y; i < y + size; i++) {
        for (int j = x; j < x + size; j++) {
            if (sqr[i][j] != 0)
                return 0;
        }
    }
    return 1;
}

// Вставляет квадрат
void insert_sqr(vector<vector<int>>& sqr, int x, int y, int size) {
    for (int i = y; i < y + size; i++) {
        for (int j = x; j < x + size; j++) {
            sqr[i][j] = size;
        }
    }

}

// Выводит квадрат
void print_sqr(vector <vector <int>>& sqr, int k, int N) {
    for (int i = 0; i < N * k; i++) {
        for (int j = 0; j < N * k; j++) {
            cout.width(3);
            cout << sqr[i][j];
        }
        cout << "\n";
    }
}

// Заполняет начальный массив нулями
void init_sqr(vector<vector<int>>& sqr, int N) {
    sqr.resize(N);
    for (int i = 0; i < N; i++) {
        sqr[i].resize(N);
        for (int j = 0; j < N; j++) {
            sqr[i][j] = 0;
        }
    }
}

// Удаляет последний вставленный квадрат
void remove_last_sqr(vector<vector<int>>& sqr, vector<Square>& sqr_mas, int tabb) {
    Square tmp = sqr_mas.back();
    sqr_mas.pop_back();
    for(int l=0; l < tabb; l++)
        std::cout << " ";
    std::cout << "Удален " << tmp.size << " (" <<  tmp.x + 1 << " " << tmp.y + 1 << ")" <<'\n';
    for (int i = tmp.y; i < tmp.y + tmp.size; i++) {
        for (int j = tmp.x; j < tmp.x + tmp.size; j++) {
            sqr[i][j] = 0;
        }
    }
}

void Start(int &k, vector<vector<int>>& sqr, vector<Square>& sqr_arr, int& S, int& N, int& min_num) {
    init_sqr(sqr, N);
    // Если N кратно 2
    if (N % 2 == 0) {
        k = N / 2;
        N = 2;
    }
    // Если N кратно 3
    else if (N % 3 == 0) {
        k = N / 3;
        N = 3;
    }
    // Если N кратно 5
    else if (N % 5 == 0) {
        k = N / 5;
        N = 5;
    }

    min_num = 2 * N + 1;
    // Заполняем начальный квадрат
    sqr_arr.push_back({ 0, 0, (N + 1) / 2 });
    sqr_arr.push_back({ 0, (N + 1) / 2, N / 2 });
    sqr_arr.push_back({ (N + 1) / 2, 0, N / 2 });

    insert_sqr(sqr, 0, 0, (N + 1) / 2);
    insert_sqr(sqr, 0, (N + 1) / 2, N / 2);
    insert_sqr(sqr, (N + 1) / 2, 0, N / 2);
    std::cout << "Поставил " << (N + 1) / 2 << " (" <<  1 << " " << 1 << ")" <<'\n';
    std::cout << "Поставил " << N / 2 << " (" <<  1 << " " << (N + 1) / 2 + 1 << ")" <<'\n';
    std::cout << "Поставил " << N / 2 << " (" <<  (N + 1) / 2 + 1 << " " << 1 << ")" <<'\n';
    // Вычисляем площадь после заполнения
    S = N * N - ((N + 1) / 2) * ((N + 1) / 2) - 2 * (N / 2) * (N / 2);

}

// Рекурсивная функция
void recursive_function(vector<vector<int>>& sqr, int cur_space, int cur_size, int sqr_counter, vector<Square>& sqr_arr, int tabb, int N, int& min_num, vector<Square>& min_sqr_arr) {
    // Если текущее кол-во обрезков отличается на 1 от минимального и вставляемый квадрат не покроет всю площадь
    if (sqr_counter == min_num - 1 && cur_space > cur_size*cur_size) {
        for(int l=0; l < tabb; l++)
            std::cout << " ";
        std::cout << "Текущий вариант не минимальный, выход из рекурсии" << '\n';
        return;
    }
    if ((cur_size + 1) <= (N / 2) && sqr_counter == 3) {
        recursive_function(sqr, cur_space, (cur_size+1), sqr_arr.size(), sqr_arr, 0, N, min_num, min_sqr_arr);
    }
    // Поиск свободного места и вставка квадрата
    bool flag = false;
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            // Если нашли свободное место
            if (sqr[y][x] == 0) {
                // Попытка вставить квадрат размера cur_size
                if (can_insert_sqr(sqr, x, y, cur_size)){
                    insert_sqr(sqr, x, y, cur_size);
                    flag = true;
                    cur_space -= cur_size * cur_size;
                    sqr_arr.push_back({ x, y, cur_size });
                    for(int l=0; l < tabb; l++)
                        std::cout << " ";
                    std::cout << "Поставил " << cur_size<< " (" <<  x + 1 << " " << y + 1 << ")" <<'\n';
                    break;
                }
                else {
                    for(int l=0; l < tabb; l++)
                        std::cout << " ";
                    std::cout << "Не удалось поставить " << cur_size<< " (" <<  x + 1 << " " << y + 1 << ")" <<'\n';
                    return;
                }
            }
            else {
                x += sqr[y][x] - 1;
            }
        }
        // Выход из внешнего цикла
        if(flag) {
            break;
        }
    }

    // Нашли заполнение равное минимальному на данный момент
    if (sqr_counter + 1 == min_num) {
        for(int l=0; l < tabb; l++)
            std::cout << " ";
        std::cout << "Текущий вариант не минимальный, выход из рекурсии" << '\n';
        // Удаляем последний вставленный квадрат
        remove_last_sqr(sqr, sqr_arr, tabb);
        return;
    }

    // Нашли минимальное заполнение
    if (sqr_counter + 1 <= min_num && cur_space == 0) {
        min_num = sqr_counter + 1;
        std::cout << "Найденно новое мин кол-во квадратов: " << min_num << '\n';
        min_sqr_arr.assign(sqr_arr.begin(), sqr_arr.end());
        // Удаляем последний вставленный квадрат
        remove_last_sqr(sqr, sqr_arr, tabb);
        return;
    }

    // Вызываем рекурсивную функцию
    for (int i = N / 2; i > 0; i--) {
        if (i * i <= cur_space) {
            for (int l=0; l < tabb+2; l++)
                std::cout << " ";
            std::cout << "Вызвал рекурсию со стороной квадрата: " << i << '\n';
            recursive_function(sqr, cur_space, i, sqr_counter + 1, sqr_arr, tabb+2, N, min_num, min_sqr_arr);
        }
    }
    // Удаляем последний вставленный квадрат
    remove_last_sqr(sqr, sqr_arr, tabb);
}

int main() {
    vector <vector<int>> sqr;
    vector <Square> sqr_arr;
    vector <Square> min_sqr_arr;
    int k = 1;
    int S;
    int N;
    int min_num;

    //cout << "Введите число от 2 до 40: ";
    cin >> N;

    Start(k, sqr, sqr_arr, S, N, min_num);

    // Инициируем переменную хранящую время начала выполнения
    clock_t start = clock();
    // Вызываемм рекурсивную функцию и передаем ей размер квадрата который хотим вставить(i)

    recursive_function(sqr, S, 1, sqr_arr.size(), sqr_arr, 0, N, min_num, min_sqr_arr);


    // Инициируем переменную хранящую время завершения выполнения
    clock_t end = clock();
    // Выводим время выполнения
    cout << "\nВремя выполнения: " << (double) (end - start) / CLOCKS_PER_SEC << "\n\n";

    cout << min_num << "\n";

    for (int i = 0; i < min_sqr_arr.size(); i++) {
        cout << min_sqr_arr[i].x * k + 1 << " " << min_sqr_arr[i].y * k + 1 << " " << min_sqr_arr[i].size * k << "\n";
        insert_sqr(sqr, min_sqr_arr[i].x * k, min_sqr_arr[i].y * k, min_sqr_arr[i].size * k);
    }
    std::cout << '\n';
    print_sqr(sqr, k, N);
    return 0;
}


