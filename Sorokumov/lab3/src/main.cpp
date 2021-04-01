#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

int INF = (int) 1e9;

struct Connect                                                            //Список связей
{
    char value;
    int weight;
    bool real;
};
struct Edge                                                                //Ребро
{
    char from;
    char to;
    int weight;
};

class Vertex                                                            //Вершина
{
public:
    char key;
    bool visited = false;
    Connect *connects;
    size_t size_v = 0;

    void
    resize_con()                                                    //Функция изменения размера списка инцидентных ребер
    {
        if (size_v % 5 != 0 && size_v) return;
        Connect *tmp = new Connect[size_v + 5];
        for (int i = 0; i < size_v; i++)
            tmp[i] = connects[i];
        if (size_v) delete[] connects;
        connects = tmp;
    };

    size_t find_con(const char litter)                                    //Функция нахождения вершины
    {
        for (int i = 0; i < size_v; i++)
            if (connects[i].value == litter)
                return i;
        return 0;
    };
};

int comp(const Edge &val1, const Edge &val2)                            //Функция сравнения ребер
{
    if (val1.from == val2.from)
        return val1.to < val2.to;
    return val1.from < val2.from;
};

class Directed_Graph {
private:
    Vertex *list;
    size_t size_l = 0;
public:
    ~Directed_Graph() {
        for (size_t i = 0; i < size_l; i++)
            delete[] list[i].connects;
        delete[] list;
    }

    Directed_Graph()
            : list(new Vertex[3]) {};

    void print()                                                        //Функция вывода графа
    {
        for (int i = 0; i < size_l; i++) {
            std::cout << list[i].key << " : ";
            for (int j = 0; j < list[i].size_v; j++)
                if (list[i].connects[j].real) {
                    std::cout << list[i].connects[j].value << "(" << list[i].connects[j].weight << ") ";
                    std::cout << std::endl;
                }
        }
    };

    void push(const char key, const char value, const int weight, bool real)//Функция добавляет новое ребро
    {
        find_or_add(value);
        Vertex &v = list[find_or_add(key)];
        v.resize_con();
        v.connects[v.size_v].value = value;
        v.connects[v.size_v].weight = weight;
        v.connects[v.size_v].real = real;
        v.size_v++;
    };

    size_t
    find_or_add(const char litter)                                //Функция либо находит, либо добавляет новую вершину
    {
        for (int i = 0; i < size_l; i++)
            if (list[i].key == litter)
                return i;
        resize_list();
        list[size_l].key = litter;
        return size_l++;
    };

    void
    resize_list()                                                    //Функция изменения размера списка вершин с их ребрами
    {
        if (size_l % 3 != 0 || !size_l) return;

        Vertex *tmp = new Vertex[size_l + 3];

        for (int i = 0; i < size_l; i++) {
            tmp[i] = list[i];
            tmp[i].connects = new Connect[list[i].size_v];
            tmp[i].connects = list[i].connects;
        }
        list = tmp;
        for (int i = 0; i < size_l; i++)
            list[i].connects = tmp[i].connects;
    };

    void unvisit()                                                        //Функция обращает все ребра в непройденные
    {
        for (int i = 0; i < size_l; i++)
            list[i].visited = false;
    };

    int find_flow(size_t start, size_t finish, int flow, int count_rec)                //Функция нахождения потока
    {
        std::cout << "Проходим в вершину (" << list[start].key << ")." << std::endl;
        if (start == finish) {
            for (int k = 0; k < count_rec; k++) {
                std::cout << "\t";
            }
            std::cout << "Дошли до конца." << std::endl;
            return flow;                                                //возвращяем полученный минимум на пути
        }
        list[start].visited = true;                                        //помечаем ребро посещенным
        for (int k = 0; k < count_rec; k++) {
            std::cout << "\t";
        }
        std::cout << "Перебираем все инцидентные вершины для (" << list[start].key << "):" << std::endl;
        for (size_t edge = 0;
             edge < list[start].size_v; edge++)            //Перебираем все инцидентные ребра вершине start
        {
            size_t to = find_or_add(list[start].connects[edge].value);    //Находим индекс инцидентной вершины в списке
            for (int k = 0; k < count_rec; k++) {
                std::cout << "\t";
            }
            std::cout << edge + 1 << ") вершина (" << list[to].key << ") с остаточной пропускной способностью - "
                 << list[start].connects[edge].weight << std::endl;
            if (!list[to].visited && list[start].connects[edge].weight >
                                     0)//Если вершина не посещена и остаточный вес ребра не нулевой, проходим
            {
                int min_result = find_flow(to, finish, std::min(flow, list[start].connects[edge].weight),
                                           count_rec + 1);//Полученный максимальный поток через минимальное ребро
                for (int k = 0; k < count_rec; k++) {
                    std::cout << "\t";
                }
                std::cout << "Полученный поток - " << min_result << std::endl;
                if (min_result > 0)                                        //И если поток не нулевой, проходим
                {
                    for (int k = 0; k < count_rec; k++) {
                        std::cout << "\t";
                    }
                    std::cout << "Вычитаем из текущей пропускной способности реального ребра поток: "
                         << list[start].connects[edge].weight
                         << " - " << min_result << " = " << list[start].connects[edge].weight - min_result << std::endl;
                    list[start].connects[edge].weight -= min_result;    //Вычитаем поток из пропускной способности для реального ребра
                    //Обратное
                    size_t con = list[to].find_con(list[start].key);    //Находим индекс обратного ребра
                    for (int k = 0; k < count_rec; k++) {
                        std::cout << "\t";
                    }
                    std::cout << "Добавляем к мощности мнимого ребра поток: " << list[to].connects[con].weight
                         << " + " << min_result << " = " << list[to].connects[con].weight + min_result << std::endl;
                    list[to].connects[con].weight += min_result;        //И к обратному прибавляем поток
                    return min_result;                                    //Возвращаем результат
                }
            } else {
                for (int k = 0; k < count_rec; k++) {
                    std::cout << "\t";
                }
                if (list[to].visited) {
                    std::cout << "Вершина (" << list[to].key << ") уже посещена." << std::endl;
                } else {
                    std::cout << "Недостаточная пропускная способность." << std::endl;
                }
            }
        }
        return 0;                                                        //если не нашли поток из этой вершины вернем 0
    }

    void print_flow()                                                    //Функция печати ребер с их потоком
    {
        std::vector<Edge> vec;                                                //Контейнер для ребер с потоком
        for (int i = 0; i < size_l; i++)
            for (int j = 0; j < list[i].size_v; j++)
                if (!list[i].connects[j].real)                            //Добавляем, если ребро мниное
                    vec.push_back({list[i].connects[j].value, list[i].key,
                                   abs(list[i].connects[j].weight < 0 ? 0 : list[i].connects[j].weight)});
        sort(vec.begin(), vec.end(),
             comp);                                //Сортируем содержимое контейнера с помощью comp
        for_each(vec.begin(), vec.end(),
                 [](const Edge &obj) { std::cout << obj.from << " " << obj.to << " " << obj.weight << std::endl; });//Выводим
        if (!vec.empty()) vec.clear();                                                    //Очистка контейнера
    };
};

int main() {
    setlocale(LC_ALL, "rus");

    int N;
    char start;
    char finish;
    std::cin >> N >> start
        >> finish;                                        // считываем количество ребер, начальную и конечную вершины

    Directed_Graph graf;
    char from, to;
    int weight;
    for (size_t i = 0; i < N; i++) {
        std::cin >> from >> to >> weight;
        graf.push(from, to, weight, true);                                //Добавляем реальное ребро
        graf.push(to, from, 0, false);                                    //Добавляем мнимое ребро
    }
    std::cout << "Представление графа в виде списков:" << std::endl;
    graf.print();                                                        //Вывод графа
    std::cout << std::endl;


    std::cout << "Нахождение максимального потока." << std::endl;
    int max_flow = 0;
    int iter_res;
    int i = 1;
    std::cout << "____Круг #" << i++ << std::endl;
    while ((iter_res = graf.find_flow(graf.find_or_add(start), graf.find_or_add(finish), INF, 0)) >
           0) //Пока есть путь в графе
    {
        std::cout << "____Круг #" << i++ << std::endl;
        graf.unvisit();                                                    //Пометим ребра непройденными
        max_flow += iter_res;                                            //К итоговому потоку добавим найденный
    }
    std::cout << max_flow << std::endl;                                            //Выводим максимальный поток

    graf.print_flow();                                                    //Выводим ребра с их потоками
    return 0;
}
