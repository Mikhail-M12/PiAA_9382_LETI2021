import sys
from typing import List, Optional, Tuple
import logging

# logging.basicConfig(level=logging.INFO, format="%(message)s")


class InputReader:
    @staticmethod
    def readLines() -> List[str]:
        result = list()
        line = sys.stdin.readline()

        while line != "":
            result.append(line.strip())
            line = sys.stdin.readline()

        return result


class GraphCreator:
    @staticmethod
    def _inputHeuristics(setOfVertexes: set):
        heuristics = dict()
        for vertex in setOfVertexes:
            while vertex not in heuristics:
                value = int(input(f"Введите эвристическую функцию для вершины {vertex}: "))
                if value >= 0:
                    heuristics[vertex] = value
                else:
                    print("Эвристическая функция должна принимать неотрицательное число!"
                          " Повторите ввод.")
        return heuristics

    @staticmethod
    def createGraph(lines: List[str]) -> Tuple[dict, dict]:
        graph = dict()
        setOfVertexes = set()

        for line in lines:
            src, dest, length = line.split()
            length = float(length)

            if src not in graph:
                graph[src] = [(dest, length)]
            else:
                graph[src].append((dest, length))

            setOfVertexes.add(src)
            setOfVertexes.add(dest)

        return graph, GraphCreator._inputHeuristics(setOfVertexes)


class Vertex:
    def __init__(self, name: str, cost: int, value: int, parent: str):
        self.name: str = name
        self.cost: int = cost
        self.value: int = value
        self.parent: str = parent

    def __str__(self):
        return f"Вершина - {self.name} Стоимость - {self.cost} " \
               f"Значение - {self.value} " \
               f"Родитель - {self.parent if self.parent else 'отсутсвует'}"


class VertexStorage(List[Vertex]):
    def getMinimalVertex(self) -> Optional[Vertex]:
        if not self:
            return None

        return min(self, key=lambda vertex: vertex.value)

    def getVertexByName(self, name: str) -> Optional[Vertex]:
        for vertex in self:
            if vertex.name == name:
                return vertex

    def __str__(self):
        return str([str(vertex) for vertex in self])


class Logger:
    @staticmethod
    def logStorage(nameOfStorage: str, storage: VertexStorage):
        logging.info(f"Текущие элементы хранилища '{nameOfStorage}':")
        for vertex in storage:
            logging.info(f"\t{vertex}")


class Solver:
    def __init__(self, src: str, dest: str, graph: dict, heuristics: dict):
        self._src: str = src
        self._dest: str = dest
        self._graph: dict = graph

        self._queue = VertexStorage()
        self._visited = VertexStorage()

        self._heuristics = heuristics

    @property
    def answer(self) -> str:

        # Здесь мы восстанавливаем путь по полю parent

        dest = self._visited.getVertexByName(self._dest)
        result = f""

        while dest.name != self._src:
            result = dest.name + result
            dest = self._visited.getVertexByName(dest.parent)

        return self._src + result

    def _calculateHeuristic(self, fromVertex: str) -> int:
        return self._heuristics[fromVertex]

    def solve(self) -> bool:

        start = Vertex(
            name=self._src, cost=0, value=self._calculateHeuristic(self._src),
            parent=""
        )
        self._queue.append(start)

        while self._queue:
            # Извлечение минимальной вершины из очереди
            minimalVertex = self._queue.getMinimalVertex()

            Logger.logStorage('Очередь', self._queue)

            self._queue.pop(self._queue.index(minimalVertex))
            self._visited.append(minimalVertex)

            logging.info("Из очереди была извлечена следующая вершина:")
            logging.info(f"\t{minimalVertex}")

            if minimalVertex.name == self._dest:
                logging.info(f"Данная вершина является конечной, "
                             f"поэтому работа алгоритма завершается")
                return True

            # Может быть такое, что из минимальной вершины нет путей
            if minimalVertex.name not in self._graph:
                logging.info(f"Данная вершина не имеет дальнейших путей!")
                continue

            # Обработка смежных вершин
            logging.info(f"Рассматриваем смежные вершины выбранной вершины")
            for name, length in self._graph[minimalVertex.name]:
                cost = minimalVertex.cost + length
                vertex = self._visited.getVertexByName(name)

                if vertex and cost >= vertex.cost:
                    logging.info(f"\tСмешная вершина {vertex}\n\tЕе текущая стоимость"
                                 f" <= возможной стоимости {cost}, поэтому "
                                 f"рассматривать этот путь не имеет смысла")
                    continue

                if not vertex:
                    vertex = Vertex(
                        name=name, cost=cost, value=cost + self._calculateHeuristic(name),
                        parent=minimalVertex.name
                    )
                    logging.info(f"\tВ очередь была добавлена вершина {vertex}")
                else:
                    vertex.parent = minimalVertex.name
                    vertex.cost = cost
                    vertex.value = cost + self._calculateHeuristic(name)
                    logging.info(f"\tБыли обновлены параметры вершины {vertex}, "
                                 f"она была вновь добавлена в очередь")

                if vertex not in self._queue:
                    self._queue.append(vertex)

        return False


if __name__ == "__main__":
    linesOfInput = InputReader.readLines()
    source, destination = linesOfInput[0].split()
    createdGraph, vertexHeuristics = GraphCreator.createGraph(linesOfInput[1:])
    solver = Solver(source, destination, createdGraph, vertexHeuristics)
    if not solver.solve():
        print("Решения нет")
    else:
        print(solver.answer)
