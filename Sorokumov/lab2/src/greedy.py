import sys
import networkx as nx
import numpy.random as rnd
import matplotlib.pyplot as plt
import pylab


# Класс графа
class Graph:
    def __init__(self):
        """
        Конструктор класса, не принимает ничего, создает поля: где хранится граф и необходимые для него.
        """
        self.graph = {}
        self.g = nx.DiGraph()
        self.node_in_graph = []

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

    def greedy(self, start, end):
        """
        Метод класса, отвечающий за жирный алгоритм
        :param start: вершина с которой начинается алгоритм
        :param end: вершина на которой закончится алгоритм
        :param done: массив вершин
        :return:
        """
        check = False
        done = []
        while not check:
            key = start
            ans = []
            while key in self.graph and any(self.graph[key]):

                ans.append(key)
                print("Текущая вершина: {}".format(key))
                min = 999999999999
                next = None
                print("Проходимся по всем вершинам от вершины: {}".format(key))
                for i in self.graph[key]:
                    print("Рассматриваем вершину: {} с расстоянием: {} от вершины {}".format(i, self.graph[key][i], key))
                    # если длина меньше чем минимальное значение и ключ не находится в пройденных вершинах
                    if min > self.graph[key][i] and i not in done:
                        # если ключ в графе следующее значение ключа
                        if i in self.graph:
                            print("Расстояние меньше\nПереходим на вершину {}".format(i))
                            next = i # задаём следующее значение ключа
                            min = self.graph[key][i] # задаём минимальное расстояние
                        # если ключ равен конечной вершине
                        elif i == end:
                            next = i # задаём следующее значение ключа
                            min = self.graph[key][i] # задаём минимальное расстояние
                key = next # задаём следующее значение ключа
                print("Список пройденных вершин равен: {}".format(done))
                done.append(key) # добавляем вершину в список пройденных
                if key is not None:
                    print("Добавляем вершину {} в список пройденных".format(key))
                print()
                if key == end:
                    ans.append(key)
            if ans[-1] == end:
                return ans # возвращаем ответ

    def get_graph(self):
        return self.graph

    def draw(self):
        # метод для отрисовки графа
        print("Начинаем построение графа для отрисовки")
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
        nx.draw(self.g, pos, node_size=500, with_labels=True )
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
    print("\nПостроение графа завершено, переходим к жадному алгоритму\n")
    ans = tree.greedy(a_lst[0][0], a_lst[0][1])
    # Вывод ответа

    # отрисовка графа
    tree.draw()
    print("\nОтвет на задание:", end='')
    for i in ans:
        print(i, end='')
