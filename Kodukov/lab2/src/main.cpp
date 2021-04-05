#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// flag for input from file
#define FILE_MODE 0

// vertex type
typedef char type;

// vertex + some number
typedef std::pair<type, double> path_to;
// comparator for priority queue
template <typename K, typename V>
struct comparator {
  bool operator()(const path_to &lhs, const path_to &rhs) {
    return lhs.second == rhs.second ? lhs.first < rhs.first
                                    : lhs.second > rhs.second;
  }
};

// queue with priorities for edges of vertex
typedef std::priority_queue<path_to, std::vector<path_to>,
                            comparator<type, double>> edges_end;
// vertex and it's edges
typedef std::map<type, edges_end> edges_type;
// stack for current path
typedef std::vector<type> path_stack;

class path_finder {
 private:
  edges_type edges;                  // all edges
  std::map<type, double> heuristic;  // vertrex's heuristics
  type start, end;                   // start and end of path
  std::set<type> visited;            // visited vertices

 public:
  path_finder(std::istream &input, bool input_mode) {  // 0 - handle, 1 - file
    if (input_mode == 0) {
      std::cout << "Input two vertices to find a path between (<start> <finish>):\n";
    }
    // read strart and end of path
    input >> start >> end;
    if (input_mode == 0)
      std::cout << "Input heuristics for vertices (<name> <heuristic>), Enter "
                   "to stop:\n";

    type elem;
    double value;

    // read heuristics
    while (input >> elem && elem != '+') {
      input >> value;
      heuristic[elem] = value;
    }
    if (input_mode == 0)
      std::cout << "Input edges (<first> <second> <weight>), Enter to stop:\n";
    type first, second;
    double weight;

    // read edges
    while (input >> first && first != '+') {
      input >> second >> weight;

      if (edges.find(first) == edges.end())
        edges.emplace(std::make_pair(first, edges_end()));
      edges[first].push(std::make_pair(second, weight));
    }
  }

  // print edges of one vertex
  void print_edges_vert(const edges_end &q, const type &vert) {
    edges_end tmp = q;
    std::vector<path_to> arr;
    while (!tmp.empty()) {
      arr.push_back(tmp.top());
      tmp.pop();
    }
    for (auto &el : arr)
      std::cout << vert << "-" << el.first << ": " << el.second << "\n";
  }

  // print a* frontier
  void print_frontier(const edges_end &f) {
    edges_end tmp = f;
    std::vector<path_to> arr;
    while (!tmp.empty()) {
      arr.push_back(tmp.top());
      tmp.pop();
    }
    for (auto &el : arr) std::cout << el.first << ": " << el.second << " ";
    std::cout << "\n";
  }

  // print all edges of graph
  void print_all_edges(const edges_type &e) {
    for (auto &el : e) {
      print_edges_vert(el.second, el.first);
    }
  }

  // print graph info
  void print_graph() {
    for (int i = 0; i < 15; i++) std::cout << "*";
    std::cout << "\n";
    std::cout << "Graph:\n";
    std::cout << "Vertices and heuristics:\n";
    for (auto &h : heuristic) std::cout << h.first << ": " << h.second << " ";
    std::cout << "\nStart: " << start << "\nEnd :" << end;
    std::cout << "\nEdges:\n";
    print_all_edges(edges);
    for (int i = 0; i < 15; i++) std::cout << "*";
    std::cout << "\n";
  }

  // Greedy alghotithm
  void greedy_find() {
    bool path_found = false;  // is search ended
    type cur = start;         // current vertex
    edges_end cur_pathes;     // current edges to check
    path_stack stack;         // path

    std::cout << "\n\nStarting greedy search...\n\n";

    visited.clear();
    while (!path_found) {
      std::cout << "Current vertex: " << cur << "\n";
      bool no_way = false;  // true - impossible to go furter from current vertex
      path_to res;          // current neighbour
      auto edge_iter = edges.find(cur);
      std::cout << "Current edges: \n";
      if (edge_iter != edges.end()) { // if vertex has some edges
        cur_pathes = edge_iter->second;
        print_edges_vert(cur_pathes, cur);
      } else {
        cur_pathes = edges_end();
        std::cout << "No edges\n";
      }
      auto iter_visited = visited.end();
      do {
        iter_visited = visited.end();
        if (!cur_pathes.empty()) {
          res = cur_pathes.top();  // possible next vertex in path and weight of
                                   // its edge
          std::cout << "Checking path " << cur << "-" << res.first << "\n";
          cur_pathes.pop();
          if ((iter_visited = visited.find(res.first)) != visited.end())
            std::cout << "  It was visited earlier\n";
          else
            std::cout << "  It wasn't visited earlier, go there\n";
        } 
        else { // all edges was visited
          no_way = true;
          break;
        }
        if (iter_visited == visited.end() && res.first == '\0') { // no edges to check
          no_way = true;
        }
      } while (iter_visited != visited.end());  // continue if this vertex was visited earlier
      visited.emplace(cur);     // visit vertex
      if (cur == end) {         // path found
        std::cout << "Current vertex is finish, path was found!\n\n";
        path_found = true;
        stack.push_back(cur);
        continue;
      }
      if (no_way) {  // go back
        std::cout << "  Cant go further from this vertex, return to previous\n";
        cur = stack.back();
        stack.pop_back();
        continue;
      }
      stack.push_back(cur);
      std::cout << "Current stack: ";
      for (auto &ch : stack)  // print current path
        std::cout << ch << " ";
      std::cout << "\n";
      cur = res.first;
    }
    std::cout << "Path: ";
    for (auto &ch : stack)  // print path
      std::cout << ch;
    std::cout << "\n";
  }

  // A* alghorithm
  void astar_find() {
    bool path_found = false; // is search ended
    type cur = start;        // current vertex
    edges_end cur_pathes;    // current edges

    visited.clear();
    edges_end frontier;              // unvisited vertices with heuristic
    std::map<type, double> cost_to;  // cost of path to vertices
    std::map<type, type>
        came_from;  // key - vertex, value - previous vertex on path

    std::cout << "\n\nStarting A* search...\n\n";

    while (!path_found) {
      std::cout << "Current vertex: " << cur << "\n";
      std::cout << "Current edges: \n";
      if (edges.find(cur) != edges.end()) { // vertex has some edges
        cur_pathes = edges.find(cur)->second;
        print_edges_vert(cur_pathes, cur);
      } else { // no edges
        std::cout << "No edges\n";
      }
      auto iter_visited = visited.end();
      int num = (int)cur_pathes.size(); 
      // add all unvisited neighbours to frontier
      for (size_t i = 0; i < num; i++) {
        // get one neighbour
        path_to vert = cur_pathes.top();
        cur_pathes.pop();
        std::cout << "Check neighbour " << vert.first << "\n";
        // compute length of path to neighbour
        double new_cost = cost_to[cur] + vert.second;
        if (cost_to.find(vert.first) == cost_to.end() ||
            new_cost < cost_to[vert.first]) {
          // compute heuristic for neighbour
          std::cout << "  Vertex wasnt visited or new path is better\n";
          std::cout << "  New path to it: " << new_cost
                    << ", Heuristic: " << heuristic[vert.first] << "\n";
          cost_to[vert.first] = new_cost;
          frontier.emplace(
              std::make_pair(vert.first, new_cost + heuristic[vert.first])); // add neighbour to frontier
          came_from[vert.first] = cur; // write previous vertex in path for 'vert'
        } else {
          std::cout
              << "  Current path to this vertex is better, no changes needed\n";
        }
      }
      std::cout << "Current frontier (<vertex> - <path + heuristic>):\n  ";
      print_frontier(frontier);
      std::cout << "Current pathes:\n  ";
      for (auto &p : cost_to) std::cout << p.first << ": " << p.second << " ";
      std::cout << "\n";
      // get next vertex in queue
      cur = frontier.top().first;
      frontier.pop();
      if (cur == end) { // path was found
        std::cout << "Current vertex is finish, path was found!\n\n";
        path_found = true;
      }
      if (frontier.empty() && !path_found) {
        std::cout << "No path\n";
        break;
      }
    }
    // print path
    std::cout << "Path: ";
    type tracker = end;
    path_stack path;
    while (tracker != start) {
      path.push_back(tracker);
      tracker = came_from[tracker];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    for (auto &v : path) std::cout << v;
    std::cout << "\n";
  }
};

int main() {
  bool mode = 1, finish = false;
  std::string fn;
  std::ifstream file;

  if (FILE_MODE) {
    std::cin >> fn;
    file.open(fn);
    if (!file.is_open()) {
      std::cout << "Impossible to open file!\n";
      return 0;
    }
  }
  path_finder pf((FILE_MODE ? file : std::cin), mode);
  if (file.is_open()) file.close();
  pf.print_graph();
  pf.greedy_find();
  pf.astar_find();
  return 0;
}