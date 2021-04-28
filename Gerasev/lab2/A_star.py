import networkx as nx
from operator import itemgetter
from pylab import show as pylabShow


class Graph:
    def __init__(self):
        self.graph = {}

    def addEdge(self, root, leaf, value):
        if root not in self.graph:
            self.graph[root] = {}
        self.graph[root][leaf] = value
        print("Adding {} and {} with weight {}".format(root, leaf, value))
        print("Current graph state: {}".format(self.graph))

    def a_star(self, start, end):
        shortPath = {}
        queue = []
        queue.append((start, 0))
        queue.sort(key=itemgetter(1), reverse=True)
        vector = [start]
        shortPath[start] = (vector, 0)
        while not queue == []:
            if queue[-1][0] == end:
                return shortPath[end][0]
            topOfQueue = queue[-1]
            print("Top of the queue is {}".format(queue[-1]))
            print("Current node {}".format(topOfQueue[0]))
            queue.pop()
            if topOfQueue[0] in self.graph:
                for i in list(self.graph[topOfQueue[0]].keys()):
                    currentPathLength = shortPath[topOfQueue[0]][1] + self.graph[topOfQueue[0]][i]
                    print("Current path from {} to {} = {}".format(start, i, currentPathLength))
                    if i not in shortPath or shortPath[i][1] > currentPathLength:
                        path = []
                        for j in shortPath[topOfQueue[0]][0]:
                            path.append(j)
                        path.append(i)
                        print("Current path is shorter")
                        shortPath[i] = (path, currentPathLength)
                        evristic = abs(ord(end) - ord(i))
                        print("Computing evristic function {}".format(evristic))
                        queue.append((i, evristic + shortPath[i][1]))
                        queue.sort(key=itemgetter(1), reverse=True)
                print()
        return shortPath[end][0]

    def draw(self):
        print("Initialisation of graph drawing procces...")
        g = nx.DiGraph()

        for i in self.graph:
            for j in self.graph[i]:
                print("Adding nodes {} {} with weight {}".format(i, j, self.graph[i][j]))
                g.add_edges_from([(i, j)], weight=self.graph[i][j])

        edge_labels = dict([((u, v,), d['weight'])
        for u, v, d in g.edges(data=True)])
        pos = nx.spring_layout(g, scale=100, k=10)
        nx.draw_networkx_edge_labels(g, pos, edge_labels=edge_labels)
        print("Drawing graph")
        nx.draw(g, pos, node_size=500, with_labels=True )
        pylabShow()


def inputHandler(inputList):
    graph = Graph()

    for i in range(len(inputList)):
        inputList[i] = inputList[i].split(" ")
        if i > 0:
            graph.addEdge(inputList[i][0], inputList[i][1], float(inputList[i][2]))

    start = inputList[0][0]
    end = inputList[0][1]

    return (graph, start, end)

if __name__ == '__main__':
    numberOfLines = list(iter(input, ''))
    for i in range(int(numberOfLines[0])):
        inputList = list(iter(input, ''))
        print('\n\n\n')
        proccesedInput = inputHandler(inputList)
        graph = proccesedInput[0]
        start = proccesedInput[1]
        end = proccesedInput[2]

        ans = graph.a_star(start, end)

        graph.draw()

        print("\nAnswer: ", end='')
        for i in ans:
            print(i, end='')
        print()