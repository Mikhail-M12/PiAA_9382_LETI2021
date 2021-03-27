import sys
import networkx as nx
import numpy.random as rnd
import matplotlib.pyplot as plt
import pylab


class Queue:
    def __init__(self):
        print("Инициализация очереди выполнена успешно")
        self.__data = []

    def __compare(self, a, b):
        if a[1] == b[1]:
            return a[0] < b[0]
        else:
            return a[1] > b[1]

    def top(self):
        # Возврат верхнего элемента очереди
        return self.__data[-1]

    def push(self, el):
        # добавление элемента в очердь
        print("Добавление элемента {} в очерредь".format(el))
        self.__data.append(el)
        self.__sort()

    def __sort(self):
        # Сортировка очереди
        print("Сортировка очереди")
        for i in range(len(self.__data) - 1):
            for j in range(len(self.__data) - i - 1):
                if not self.__compare(self.__data[j], self.__data[j + 1]):
                    self.__data[j], self.__data[j + 1] = self.__data[j + 1], self.__data[j]

    def pop(self):
        # удаление верхнего элемента из очереди
        print("Удаляем элемент {} из очереди".format(self.__data[-1]))
        self.__data.pop()

    def empty(self):
        # проверка пустая ли очередь
        if len(self.__data) == 0:
            print("Очередь пуста")
            return True
        else:
            print("Очередь не пуста")
            return False


class Graph:
    def __init__(self):
        self.graph = {}
        self.g = nx.DiGraph()

    def add_adge(self, head, leave, value):
        """
        Функция добавления вершины с ребром
        :param head: вершина из которой будет проведено ребро
        :param leave: вершина к которой будет проведено ребро
        :param value: вес ребра
        :return: None
        """
        if head not in self.graph:
            self.graph[head] = {}
        self.graph[head][leave] = value
        print('Добавляем вершины {} и {} и расстояние между ними равное {}'.format(head, leave, value))
        print("текущее значение графа: {}".format(self.graph))

    def print_graph(self):
        """
        Печатает словарь графа
        :return: None
        """
        print(self.graph)


    def a_star(self, start, end):
        shortPath = {}
        # Инициализации очереди с приоритетом
        queue = Queue()
        queue.push((start, 0))
        vector = [start]
        shortPath[start] = (vector, 0)
        # Пока очередь не пуста
        while not queue.empty():
            if queue.top()[0] == end:
                # ]если верхний элемент очереди равен итоговой вершине возвращаем результат
                return shortPath[end][0]
            temp = queue.top()
            print("Верхний элемент очереди равен {}".format(queue.top()))
            print("Текущая вершина {}".format(temp[0]))
            queue.pop()
            # Проходимся по всем верщинам от текущей
            if temp[0] in self.graph:
                for i in list(self.graph[temp[0]].keys()):
                    # Считаем текущее рассстояние
                    currentPathLength = shortPath[temp[0]][1] + self.graph[temp[0]][i]
                    print("Текущий путь равен {}".format(currentPathLength))
                    # Если текущее расстояние меньше
                    if i not in shortPath or shortPath[i][1] > currentPathLength:
                        # Изменяем данный
                        path = []
                        for j in shortPath[temp[0]][0]:
                            path.append(j)
                        path.append(i)
                        print("Текущий путь оказался короче")
                        print("Текущий путь из вершин {}".format(path))
                        shortPath[i] = (path, currentPathLength)
                        # Подсчет эвристической функции
                        evristic = abs(ord(end) - ord(i))
                        print("Считаем эвристическую функцию, которая равна {}".format(evristic))
                        queue.push((i, evristic + shortPath[i][1]))
                print()
        return shortPath[end][0]

    def draw(self):
        # метод для отрисовки графа
        print("\nНачинаем построение графа для отрисовки")
        # Инициализируем вершины и ребра графа
        for i in self.graph:
            for j in self.graph[i]:
                print("Добавляем вершины {} {} с расстоянием {}".format(i, j, self.graph[i][j]))
                self.g.add_edges_from([(i, j)], weight=self.graph[i][j])

        edge_labels = dict([((u, v,), d['weight'])
                            for u, v, d in self.g.edges(data=True)])
        pos = nx.spring_layout(self.g, scale=100, k=10)  # вычисление позиций
        nx.draw_networkx_edge_labels(self.g, pos, edge_labels=edge_labels)  # отрисовывем граф
        print("Рисуем граф")
        nx.draw(self.g, pos, node_size=500, with_labels=True)
        pylab.show()


if __name__ == '__main__':
    a_lst = []
    # считываем данные
    line = input()
    while line:
        a_lst.append(line.strip())
        line = input()
    # создание объекта графа
    tree = Graph()
    # заполнение графа
    for i in range(len(a_lst)):
        a_lst[i] = a_lst[i].split(" ")
        if i > 0:
            tree.add_adge(a_lst[i][0], a_lst[i][1], float(a_lst[i][2]))
    print("\nПостроение графа завершено, переходим к A* алгоритму\n")
    ans = tree.a_star(a_lst[0][0], a_lst[0][1])
    # Вывод ответа

    # отрисовка графа
    tree.draw()
    print("\nОтвет на задание:", end='')
    for i in ans:
        print(i, end='')
