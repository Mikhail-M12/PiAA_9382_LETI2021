import sys
from typing import List, Optional
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
    def createGraph(lines: List[str]) -> dict:
        graph = dict()

        for line in lines:
            src, dest, length = line.split()
            length = float(length)

            if src not in graph:
                graph[src] = [(dest, length)]
            else:
                graph[src].append((dest, length))

        return graph


class Solver:
    def __init__(self, src: str, dest: str, graph: dict):
        self._src, self._dest = src, dest
        self._graph = graph

        self._visitedMap = dict()
        self._answer = ""

    @property
    def answer(self) -> Optional[str]:
        return self._answer

    def _findMinimalWay(self, fromVertex: str) -> Optional[str]:
        if fromVertex not in self._graph:
            return None

        if not self._graph[fromVertex]:
            return None

        minimalLength: Optional[int] = None
        minimalWay: Optional[str] = None

        # ищем ребро с минимальным весом из вершины fromVertex

        for nextVertex, length in self._graph[fromVertex]:

            if self._isVisitedEdge(fromVertex, nextVertex):
                continue

            if not minimalLength:
                minimalWay, minimalLength = nextVertex, length
            elif minimalLength > length:
                minimalWay, minimalLength = nextVertex, length
            elif minimalLength == length and nextVertex < minimalWay:
                minimalWay = nextVertex

        return minimalWay

    def _markEdgeAsVisited(self, sourceVertex: str, destinationVertex: str) -> None:
        if sourceVertex not in self._visitedMap:
            self._visitedMap[sourceVertex] = [destinationVertex]
        else:
            self._visitedMap[sourceVertex].append(destinationVertex)

    def _isVisitedEdge(self, sourceVertex: str, destinationVertex: str) -> bool:
        if sourceVertex in self._visitedMap:
            return destinationVertex in self._visitedMap[sourceVertex]
        return False

    def _solve(self, fromVertex: str) -> Optional[str]:
        if fromVertex == self._dest:
            logging.info("Текущая рассматриваемая вершина - конечная, завершение алгоритма!")
            return fromVertex

        toVertex = self._findMinimalWay(fromVertex)
        # ищем непосещенное ребро с минимальным весом и посещаем его
        while toVertex:
            logging.info(f"Начал рассматривать минимальный непосещенный путь {fromVertex} - {toVertex}")
            self._markEdgeAsVisited(fromVertex, toVertex)
            # вызываем алгоритм для вершины, где ребро имеет минимальный вес
            result = self._solve(toVertex)
            if result:
                # если в result что-то записано, значит мы уже нашли путь
                return fromVertex + result
            # если ничего нет, то смотрим следующие ребра
            toVertex = self._findMinimalWay(fromVertex)

    def solve(self) -> Optional[str]:
        self._answer = self._solve(self._src)
        return self._answer


if __name__ == "__main__":
    linesOfInput = InputReader.readLines()
    source, destination = linesOfInput[0].split()
    createdGraph = GraphCreator.createGraph(linesOfInput[1:])
    solver = Solver(source, destination, createdGraph)
    if not solver.solve():
        print("Решения нет")
    else:
        print(solver.answer)
