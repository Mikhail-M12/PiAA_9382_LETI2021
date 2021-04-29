from math import inf

class Graph:
    def __init__(self):
        self.numberOfEdges = int(input())
        self.source = input()
        self.drain = input()
        self.edges = {} # рёбра
        self.edgeList = [] # для вывода
        self.accFlow = 0 # суммарный поток
        for i in range(self.numberOfEdges):
            edge = input().split()
            key, target, capacity = edge[0], edge[1], int(edge[2])
            print("В граф добавлено ребро ", key, target, capacity)
            self.edgeList.append((key, target, capacity))

            # Рёбра в исток и из стока не добавляются
            if key != self.drain and target != self.source:
                if key not in self.edges.keys():
                    self.edges[key] = {}
                if target not in self.edges.keys():
                    self.edges[target] = {}
                if key not in self.edges[target].keys():
                    self.edges[target][key] = 0
                if target not in self.edges[key].keys():
                    self.edges[key][target] = capacity
                else:
                    self.edges[key][target] += capacity
        self.edgeList.sort(key=lambda x: (x[0], x[1]))
        print ("\nСостояние исходных рёбер в начале выполнения:")
        for i in self.edgeList:
            print(i[0], i[1], i[2])
        print("\n")

    def out(self):
        print(self.accFlow)
        for e in self.edgeList:
            finalFlow = e[2] - self.edges[e[0]][e[1]]
            if finalFlow < 0:
                finalFlow = 0
            print(e[0], e[1], finalFlow)

    def solve(self):
        openList, closedList = [[0, self.source, "", inf, 0]], []

        # Когда из истока уже некуда идти, цикл заканчивается
        while openList:
            print("Из открытого списка:", openList, "была взята вершина", end=" ")
            cur = openList.pop()
            print(cur)
            print("Её дистанция: ", cur[0], "; возраст: ", cur[4],
                  ", а также она является самой старшей по алфавиту при прочих подобных вершинах")

            # Если дошли до стока, строим путь по строке с предыдущими вершинами, возвращаемся в начальное состояние
            # При этом к суммарному потоку прибавляется максимально возможный поток в этом пути
            if cur[1] == self.drain:
                print(cur[1], "- это сток, запись результата и сброс в начальное состояние")
                path = cur[2] + cur[1]
                print("Поток в пути", " -> ".join(path), "уменьшен на", cur[3], end = "\n\n")
                for i in range(len(path) - 1):
                    self.edges[path[i]][path[i+1]] -= cur[3]
                    self.edges[path[i+1]][path[i]] += cur[3]
                self.accFlow += cur[3]
                closedList.clear()
                openList = [[0, self.source, "", inf, 0]]
                continue
            closedList.append(cur[1])

            # Если вершина уже была пройдена на этом пути, её не посещаем
            toOpen = list(filter(lambda x: x[0] not in closedList and x[1] > 0, self.edges[cur[1]].items()))
            maxFlow = cur[3]

            # Добавляем все доступные вершины в открытый список
            print("Вершины, доступные для посещения:")
            for i in toOpen:
                print("\t", i[0], ", расстояние: ", i[1], sep="")
                if i[1] < maxFlow:
                    maxFlow = i[1]
                openList.append([i[1], i[0], cur[2]+cur[1], maxFlow, 0])
                maxFlow = cur[3]
            # Увеличиваем "возраст" вершин в списке, сортируем (подобно очереди с приоритетом)
            for i in openList:
                i[4] += 1
            openList.sort(key=lambda x: (x[0], x[4], x[2]))
            print("Вершины были добавлены в открытый список\n")
        print("\nПоиск закончен, завершение работы\n")

gra = Graph()
gra.solve()
gra.out()
print("\nВыполнение завершено. Введите любой символ, чтобы выйти из программы.")
input()