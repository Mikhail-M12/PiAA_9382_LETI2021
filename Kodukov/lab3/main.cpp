#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

bool file;

// vertex type
typedef char type;

// vertex + some number
typedef std::pair<type, double> path_to;
// comparator for priority queue
int compare(const void *lhs, const void *rhs) {
  return ((path_to *)lhs)->second - ((path_to *)rhs)->second;  ///
}

// edges of one vertex
typedef std::map<type, double> edges_end;
// array of neighbours for bfs to sort
typedef std::vector<path_to> bfs_neighbours;
// vertex and it's edges
typedef std::map<type, edges_end> edges_type;
// stack for current path
typedef std::vector<type> path_stack;

class stream_finder {
 private:
  edges_type edges;        // all edges with residual capacities
  edges_type capacity;     // all edges with their current flows
  type start, end;         // source and drain
  std::set<type> visited;  // visited vertices
  path_stack path;

 public:
  stream_finder() {}
  stream_finder(std::istream &input, std::ostream &output, bool file) {
    std::string str;
    int num;

    std::cin.clear();  // на случай, если предыдущий ввод завершился с ошибкой
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (!file) std::cout << "Input num of edges:\n";
    input >> num;
    // read source and drain
    if (!file) std::cout << "Input source and drain:\n";
    input >> start >> end;
    type first, second;
    double weight;
    // read edges
    if (!file) std::cout << "Input edges (<first> <second> <capacity>):\n";
    for (int i = 0; i < num; i++) {
      input >> first >> second >> weight;
      edges[first][second] = weight;
    }
  }

  void print_edges_vert(const edges_end &e, const type &vert, std::ostream &output) {
    for (auto &el : e)
      output << vert << "-" << el.first << ": " << el.second << "\n";
  }

  void print_frontier(const std::queue<path_to> &f, std::ostream &output) {
    std::queue<path_to> tmp = f;
    std::vector<path_to> arr;
    while (!tmp.empty()) {
      arr.push_back(tmp.front());//top());
      tmp.pop();
    }
    for (auto &el : arr) output << el.first << ": " << el.second << " ";
    output << "\n";
  }

  void print_all_edges(const edges_type &e, std::ostream &output) {
    for (auto &el : e) {
      print_edges_vert(el.second, el.first, output);
    }
  }

  void print_graph(std::ostream &output) {
    for (int i = 0; i < 15; i++) output << "*";
    output << "\n";
    output << "Graph:\n";
    output << "\nStart: " << start << "\nEnd :" << end;
    output << "\nEdges:\n";
    print_all_edges(edges, output);
    for (int i = 0; i < 15; i++) output << "*";
    output << "\n";
  }

  // print flows of edges
  void print_capacity(std::ostream &output) { 
    for (auto &a : capacity)
      for (auto &b : a.second)
        output << a.first << " " << b.first << " " << b.second << "\n";
  }
  
  // Find path
  void update_path(std::ostream &output) {
    type cur = start;          // current vertex
    bool path_found = false;   // searching end flag

    std::queue<path_to> frontier;  // unvisited vertices with edge capacity
    std::map<type, type> came_from;  // key - vertex, value - previous vertex on path

    path.clear();
    visited.clear();

    output << "\n\nBegin path finding...\n\n";

    // search cycle
    while (!path_found) {
      bfs_neighbours cur_pathes; // edges of current vertex
      output << "\nCurrent vertex: " << cur << "\n";
      // get edges
      if (edges.find(cur) != edges.end()) {
        auto found = edges.find(cur)->second;
        for (auto &el : found)
          cur_pathes.push_back(std::make_pair(el.first, el.second));
        print_edges_vert(found, cur, output);
      } else {
        cur_pathes = bfs_neighbours();
        output << "No edges\n";
      }
      output << "Current edges: \n";
      auto iter_visited = visited.end();
      int num = (int)cur_pathes.size();
      // sort neighbours by capacity
      std::qsort(cur_pathes.data(), num, sizeof(path_to), compare);
      // add all unvisited neighbours to frontier
      for (auto &vert : cur_pathes){
        output << "Checking path " << cur << "-" << vert.first << "\n";
        // check if capacity of edge > 0 and it wasnt visited
        if (vert.second > 0 && (visited.find(vert.first) == visited.end())){
          output << "  It wasn't visited earlier and capacity > 0, add to frontier\n";
          // add to frontier
          frontier.push(vert);
          came_from[vert.first] = cur;
          // check if path found
          if (vert.first == end) {
            output << "Current vertex is finish, path was found!\n";
            path_found = true;
            break;
          }
          visited.emplace(vert.first);
        } else {
          output << "  It was visited earlier or capacity == 0\n";
        }
      }
      if (!path_found) {
        if (frontier.empty()) {
          output << "No more pathes\n";
          break;
        }
        // get next vertex from frontier
        output << "Frontier:\n";
        print_frontier(frontier, output);
        cur = frontier.front().first;
        frontier.pop();
      }
    }
    // Get path
    if (path_found) {
      type tracker = end;
      while (tracker != start) {
        path.push_back(tracker);
        tracker = came_from[tracker];
      }
      path.push_back(start);
      std::reverse(path.begin(), path.end());
      output << "Path: ";
      for (auto &v: path)
        output << v;
      output << "\n";
    }
  }

  // count max flow of net
  double find_max_flow(std::ostream &output) { 
    double flow = 0;

    // set all capacities to 0
    for (auto &v1: edges)
      for (auto &v2: v1.second)   
        capacity[v1.first][v2.first] = 0;
    // continue if path exists
    while (update_path(output), !path.empty()) {
      type a = start, b;
      // count min capacity of path
      output << "Capacity of path:\n  "; 
      double min_capacity = std::numeric_limits<double>::max();
      for (int i = 1; i < path.size(); i++) {
        b = path[i];
        double cur_capacity = edges[a][b];
        if (cur_capacity < min_capacity) 
          min_capacity = cur_capacity;
        output << a << "-(" << cur_capacity << ")-" << b << " ";
        a = b;
      }
      output << "\nMin capacity: " << min_capacity << "\n";
      // update graph
      a = start;
      for (int i = 1; i < path.size(); i++) {
        b = path[i];
        edges[a][b] -= min_capacity;
        edges[b][a] += min_capacity;
        a = b;
      }
      output << "Residual capacities:\n";
      print_all_edges(edges, output);
      // update current max flow
      flow += min_capacity;
      // update capacities
      a = start;
      for (int i = 1; i < path.size(); i++) {
        b = path[i];
        if (capacity.find(a) != capacity.end() &&
            capacity[a].find(b) != capacity[a].end())
          capacity[a][b] += min_capacity;
        else
          capacity[b][a] -= min_capacity;
        a = b;
      }
    }
    return flow;
  }
};

int main() {
  stream_finder sf;

  std::cout << "File input (from input.txt)? 1 - yes, other - no:";
  char ch;
  std::cin >> ch;

  std::ofstream output("output.txt");
  if (ch == '1') {
    std::ifstream input("input.txt");
    sf = stream_finder(input, output, true);
  } else {
    sf = stream_finder(std::cin, output, false);
  }

  sf.print_graph(output);
  double res = sf.find_max_flow(output);

  output << "Result:\n";
  output << "\n" << res << "\n";
  sf.print_capacity(output);
  std::cout << "Full log in output.txt\n";

  std::cout << "Result:\n";
  std::cout << "\n" << res << "\n";
  sf.print_capacity(std::cout);
  return 0;
}