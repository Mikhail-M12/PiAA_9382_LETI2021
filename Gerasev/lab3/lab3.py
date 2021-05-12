import copy
from operator import itemgetter

from typing import List, Optional
import typing
import sys
    
class Graph:
    def __init__(self, inputGraph = {}):
        self.graph = inputGraph

    def copy(self):
        return Graph(copy.deepcopy(self.graph))

    def print(self):
        list = []
        for i in self.graph:
            for j in self.graph[i]:
                res = int(self.graph[i][j][1])
                list.append("{} {} {}".format(i,j, res))
                # list.append("{} {} {}".format(i,j,self.graph[i][j]))
        res = sorted(list)
        for i in res:
            print(i)

    def addEdge(self, root, leaf, value):
        if root not in self.graph:
            self.graph[root] = {}  
        if leaf not in self.graph[root]:
          self.graph[root][leaf] = [value, 0]


    def useFlow(self, a, b, flow):
      if a not in self.graph or b not in self.graph:
        return
      if self.graph[b][a][1] > 0:
        dif = flow - self.graph[b][a][1]
        self.graph[b][a][1] = 0
        self.graph[b][a][0] += flow - dif

        self.graph[a][b][0] -= dif
        self.graph[a][b][1] += dif
      
      else:  
        self.graph[a][b][0] -= flow
        self.graph[a][b][1] += flow

    def givePath(self, start, end):
        currentPath = []
        stack = []
        handeledNodes = {}
        if start not in self.graph or end not in self.graph:
            return currentPath
        stack.append(start)
        while stack != []:
            currentNode = stack[-1]
            if currentNode not in handeledNodes:
                currentPath.append(currentNode)
                for i in self.graph[currentNode]:
                    if (self.graph[currentNode][i][0] > 0 or self.graph[i][currentNode][1] > 0) and i not in handeledNodes:
                        stack.append(i)
                        if i == end:
                            currentPath.append(i)
                            return currentPath
                handeledNodes.update({currentNode : 0})
            else:
                stack.pop()
                if currentPath != []:
                    currentPath.pop()
        return currentPath

    def usePath(self, path):
        values = []
        for i in range(len(path)-1):
            first = path[i]
            second = path[i+1]
            res = max(self.graph[first][second][0], self.graph[second][first][1])
            values.append(res)
        flow = min(values)
        if flow > 0 :
            for i in range(len(path)-1):
                first = path[i]
                second = path[i+1]
                self.useFlow(first, second, flow)
                
        return flow

    def addPath(self, path, flow):
        for i in range(len(path)-1):
            first = path[i]
            second = path[i+1]
            if first not in self.graph:
                graph.addEdge(first, second, flow)
            else:
                self.useFlow(self, first, second, flow)

def fordsAlg(graph : Graph, source, runoff):
    totalFlow = 0
    graphCopy = graph.copy()
    for i in graph.graph:
        for j in graph.graph[i]:
            graphCopy.addEdge(j, i, 0)

    path = graphCopy.givePath(source, runoff)
    while path != []:
        flow = graphCopy.usePath(path)
        totalFlow += flow
        path = graphCopy.givePath(source, runoff)
        
    return (graphCopy, totalFlow)

def inputHandler(inputList):
    graph = Graph()
    source = inputList[0]
    runoff = inputList[1]
    inputList = inputList[2:]
    
    for line in inputList:
        line = line.split(" ")
        graph.addEdge(line[0], line[1], float(line[2]))

    return (graph, source, runoff)

if __name__ == '__main__':
    lines = list(iter(input, ''))
    inputRaw = inputHandler(lines[1:])
    source = inputRaw[1]
    runoff = inputRaw[2]
    graph = inputRaw[0]

    res = fordsAlg(graph, source, runoff)
    
    for i in graph.graph:
        for j in graph.graph[i]:
            graph.graph[i][j] = res[0].graph[i][j]
    print(int(res[1]))
    graph.print()
