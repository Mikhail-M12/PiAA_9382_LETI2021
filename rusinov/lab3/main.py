from typing import Union


class GraphCreator:
    @staticmethod
    def readGraph():
        countEdges = int(input())
        source = input()
        destination = input()
        graph = dict()
        for _ in range(countEdges):
            src, dest, length = input().split()
            if src not in graph:
                graph[src] = []
            graph[src].append([dest, int(length), 0])
        return source, destination, graph


class Solver:
    def __init__(self, source: str, destination: str, graph: dict):
        self._source = source
        self._destination = destination
        self._graph = graph
        self._visited = []

        self._stepCounter = 1

        self._recursionLevel = 0

    def _solve(self, vertex: str, thread: Union[int, float]):

        print(f"{'*' * self._recursionLevel * 2}Вызов рекурсивной функции с вершиной {vertex},"
              f" текущий возможный поток {thread}")

        # выходим из рекурсии, если вершина конечная

        if vertex == self._destination:
            print(f"{'*' * self._recursionLevel * 2}Текущая рассматриваемая вершина - конечная,"
                  f" поэтому работа на текущем шагу завершается.")
            return thread

        # добавляем вершину в список посещенных

        self._visited.append(vertex)

        # проверяем, есть ли смежные вершины к рассматриваемой. Если нет, выходим из рекурсии

        if vertex not in self._graph:
            print(f"{'*' * self._recursionLevel * 2}Из данной вершины нет возможный путей,"
                  f" поэтому оканчиваем работу с ней")
            return 0

        # Сортируем список смежных вершин (требуется для верного ответа на Степике)

        children = sorted(self._graph[vertex], key=lambda x: x[0], reverse=True)

        print(f"{'*' * self._recursionLevel * 2}Вершина {vertex} имеет следующие смежные вершины"
              f" - {' '.join([child[0] for child in children])}")

        # Рассматриваем смежные вершины

        for i, child in enumerate(children):

            print(f"{'*' * self._recursionLevel * 2}[{i+1}] Рассматриваю смежную вершину {child[0]}")

            # Если смежная вершина еще не посещена, а также через нее возможно еще пропустить поток
            # То начинаем рассматривать возможный поток из смежной вершины до конечной

            if child[0] not in self._visited and child[2] < child[1]:

                print(f"{'*' * self._recursionLevel * 2}[{i+1}] Вершина {child[0]} еще не посещена,"
                      f" а также из нее еще возможно пропустить поток")
                print(f"{'*' * self._recursionLevel * 2}[{i+1}] Поэтому начинаем искать возможный поток, который можно"
                      f" пропустить из данной вершины до конечной")

                # текущий возможный поток, который можно пропустить через смежную вершину

                maxFlowFromChild = min(thread, child[1] - child[2])

                print(f"{'*' * self._recursionLevel * 2}[{i+1}] Текущий максимальный поток,"
                      f" который можно через нее пропустить {maxFlowFromChild}")

                self._recursionLevel += 1

                # вызов рекурсивной функции
                delta = self._solve(child[0], maxFlowFromChild)

                self._recursionLevel -= 1

                # если через смежную вершину можно пропустить поток до конечной, то delta будет > 0
                if delta > 0:
                    print(f"{'*' * self._recursionLevel * 2}[{i+1}] Поток, который можно пропустить"
                          f" через вершину {child[0]} до конечной вершины - {delta}")
                    # записываем пропущенный через эту вершину поток
                    child[2] += delta
                    return delta
                else:
                    print(f"{'*' * self._recursionLevel * 2}[{i+1}] Через вершину {child[0]} нельзя пропустить"
                          f" поток до конечной вершины")
            else:
                print(f"{'*' * self._recursionLevel * 2}[{i+1}] Вершина {child[0]} уже была"
                      f" посещена на текущем шагу, либо через нее пропущен максимально возможный поток")
        return 0

    def solve(self):
        result = 0

        print(f"Текущий шаг {self._stepCounter}")
        answer = self._solve(self._source, float("+inf"))
        print()

        while answer:
            print(f"На шаге {self._stepCounter} удалось пропустить поток размера {answer},"
                  f" поэтому переходим к следующему шагу")
            self._stepCounter += 1
            # запускаем следующий шаг для поиска пути, обнуляем посещенные вершины
            result += answer
            self._visited = []

            print(f"Текущий шаг {self._stepCounter}")
            answer = self._solve(self._source, float("+inf"))
            print()

        print(f"С шага {self._stepCounter} невозможно пропустить более поток до конечной вершины,"
              f" программа завершает работу")

        return result

    def __str__(self):
        text = ""
        for vertex in sorted(self._graph.keys()):
            for neighbor in sorted(self._graph[vertex], key=lambda x: x[0]):
                text += f"{vertex} {neighbor[0]} {neighbor[2]}\n"
        return text


if __name__ == "__main__":
    solver = Solver(*GraphCreator.readGraph())
    print(solver.solve())
    print(solver)
