class Graph:

    def __init__(self, graph):
        self.graph = graph
        self.row = len(graph)

    def searchBFS(self, source, outlet, parent):

        visited = self.row * [False] #list
        queue = [source]

        visited[source] = True

        while queue:
            vertex = queue.pop(0)

            for ind, value in enumerate(self.graph[vertex]):
                if visited[ind] == False and value > 0: #check vertice for find flow
                    queue.append(ind)
                    visited[ind] = True
                    parent[ind] = vertex
        if visited[outlet]:
        	return True
        return False	
          
    def findFlowSize(self, source, outlet, graph, parent):

        flow_size = 10000
        vertex = outlet
        while vertex != source:
            if self.graph[parent[vertex]][vertex] < flow_size:
                flow_size = self.graph[parent[vertex]][vertex]
            vertex = parent[vertex]

        return flow_size

    def algoFordFulkerson(self, source, outlet, flow_graph):

        parent = self.row * [-1]
        max_flow = 0

        while self.searchBFS(source, outlet, parent):
        
            #find flow size to this path	
            flow_size = self.findFlowSize(source, outlet, graph, parent)

            max_flow += flow_size

            vertex = outlet
            #update graph
            while vertex != source:
                u = parent[vertex]
                self.graph[u][vertex] -= flow_size
                self.graph[vertex][u] += flow_size
                flow_graph[u][vertex] += flow_size
                flow_graph[vertex][u] -= flow_size
                vertex = parent[vertex]
            print("\nCanging graphes...")
            print ("Current adjacency matrix")
            self.printGraph(graph)
            print("\nCurrent flow graph adjacency matrix")
            self.printGraph(flow_graph)    

        return max_flow    

    def printGraph(self, graph):
        for i in range(self.row):
            print(graph[i])      

count = int(input())
source = input()
outlet = input()
input_list = []
dict_ver = {}
list_of_vertice = []
size = 0

for i in range(count): # compute a count of defferent vertex
    input_list.append(input())
    b = input_list[i].split(" ")
    if b[0] not in list_of_vertice:
        list_of_vertice.append(b[0])
        size +=1
    if b[1] not in list_of_vertice:
        list_of_vertice.append(b[1])
        size += 1  

list_of_vertice.sort()
for i in range(size):
   dict_ver[list_of_vertice[i]] = i

graph = [[0 for x in range(size)] for y in range(size)] #incoming adjecency graph
flow_graph = [[0 for x in range(size)] for y in range(size)] # outcoming adjecency graph

for i in range(count):
    b = input_list[i].split(" ")
    graph[dict_ver[b[0]]][dict_ver[b[1]]] = int(b[2])
   
res_graph = Graph(graph)

flow = res_graph.algoFordFulkerson(dict_ver[source], dict_ver[outlet], flow_graph)

print("\nResult flow: \n") 
print(flow)

for ind, value in enumerate(input_list):
    # compute flow in this edge
    v = value.split(" ")
    a = int(flow_graph[dict_ver[v[0]]][dict_ver[v[1]]])
    if a < 0:
        a = 0    
    new_str = "{} {} {}".format(value[0], value[2], a)
    input_list[ind] = new_str
    
input_list.sort()

for i in input_list:
    print(i)