import networkx as nx
from pylab import show as pylabShow
from math import inf as mathInf


class Graph:
    def __init__(self):
        self.graph = {}

    def addEdge(self, root, leaf, value):
        if root not in self.graph:
            self.graph[root] = {}
        self.graph[root][leaf] = value
        print("Adding {} and {} with weight {}".format(root, leaf, value))
        print("Current graph state: {}".format(self.graph))

    def greedy(self, start, end):
        handeledNodes = []
        while True:
            key = start
            ans = []
            while key in self.graph and any(self.graph[key]):
                ans.append(key)
                print("Current node: {}".format(key))
                min = mathInf
                next = None
                print("Checking all children of: {}".format(key))
                for i in self.graph[key]:
                    print("Handeling node: {} with weight: {}, child of: {}".format(i, self.graph[key][i], key))
                    if self.graph[key][i] < min and i not in handeledNodes:
                        if i in self.graph:
                            print("The weight is smaller\nGoing to new node: {}".format(i))
                            next = i
                            min = self.graph[key][i]
                        elif i == end:
                            next = i 
                            min = self.graph[key][i]
                key = next 
                print("List of handeled nodes: {}".format(handeledNodes))
                handeledNodes.append(key)
                if key is not None:
                    print("Adding node {} to the list of handeled ones".format(key))
                print()
                if key == end:
                    ans.append(key)
                if ans[-1] == end:
                    return ans

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
    inp = list(iter(input, ''))
    numberOfLines = int(inp[0])
    print('Number of tests is', numberOfLines, '\n')
    for i in range(numberOfLines):
        inputList = list(iter(input, ''))
        print(inputList)
        proccesedInput = inputHandler(inputList)
        graph = proccesedInput[0]
        start = proccesedInput[1]
        end = proccesedInput[2]

        ans = graph.greedy(start, end)

        print("\nAnswer: ", end='')
        for i in ans:
            print(i, end='')
        print('\n\n\n')